/*****************************************************************************

Filename    :   main.c
Content     :   Initialization and main loop for the Latency Tester
Created     :
Authors     :   Nirav Patel
Copyright   :   Copyright 2013 Oculus VR, Inc. All Rights reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*****************************************************************************/

#include "hw_config.h"
#include "usb_lib.h"
#include "usb_conf.h"
#include "usb_prop.h"
#include "usb_pwr.h"
#include "tick.h"
#include "sample_packet.h"
#include "feature_reports.h"
#include "trigger_packet.h"
#include "calibrate.h"
#include "tcs3414.h"
#include "segment.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static volatile bool ep1_ready = 1;

static feature_config_s config = {0};
static feature_dfu_s dfu = {0};
static feature_calibrate_s calibrate = {{0}};
static feature_trigger_s trigger = {0};
static feature_display_s display = {0};
static float sample_avg[3] = {0.0f};

uint8_t g_startup_failed;

#define DEBOUNCE (100)
#define TIMEOUT_OPTION (500)
#define AVG_ALPHA (0.4f)

#ifdef USE_TRACKER_V1
#define NUM_COLORS 2
#else
#define NUM_COLORS 3
#endif /* USE_TRACKER_V1 */

enum {
    MODE_IDLE,
    MODE_TRIGGER
};

enum {
    FACTORY_NONE,
    FACTORY_FULL,
    FACTORY_EMPTY,
    FACTORY_RED,
    FACTORY_GREEN,
    FACTORY_BLUE,
    FACTORY_DONE
};

static bool check_button_pressed(uint16_t timestamp)
{
    static bool button_value = 0;
    static uint16_t last_press = 0;

    bool ret = 0;
    bool new_value = GPIO_ReadInputDataBit(BUTTON_PORT, BUTTON_PIN);
    if (new_value && !button_value && ((uint16_t)(timestamp - last_press) > DEBOUNCE)) {
        ret = 1;
        last_press = timestamp;
    }

    button_value = new_value;
    return ret;
}

static void update_average(uint16_t *sample)
{
    for (uint8_t i = 0; i < NUM_COLORS; i++)
        sample_avg[i] = (sample[i]*AVG_ALPHA) + (sample_avg[i]*(1.0f-AVG_ALPHA));
}

static bool update_sample(uint16_t timestamp, uint8_t *sample, uint8_t *target, uint8_t *threshold)
{
    if (config.send_samples)
        sample_packet_update(timestamp, sample);

    // return 0 if any of the values are outside the threshold
    for (uint8_t i = 0; i < NUM_COLORS; i++) {
        if (abs(sample[i] - target[i]) > threshold[i]) {
            return 0;
        }
    }

    return 1;
}

static void update_factory(uint8_t factory_mode, uint8_t *sample)
{
    static const uint8_t full[] = {0xFF, 0xFF, 0xFF, 0xFF};
    static const uint8_t empty[] = {0, 0, 0, 0};

    switch (factory_mode) {
        case FACTORY_FULL:
            // Start by lighting all of the segments
            segment_set_raw(full);
            break;

        case FACTORY_EMPTY:
            // Then blank the display
            segment_set_raw(empty);
            break;

        case FACTORY_RED:
        case FACTORY_GREEN:
        case FACTORY_BLUE:
            // Then make sure each color in the sensor works
            segment_set_value(sample[factory_mode-FACTORY_RED]);
            break;
    }
}

int main(void)
{
    if (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET)
        g_startup_failed = 1;

    // Shut off JTAG and keep SWD on, needs to happen right after reset
    Configure_Debug();
    tick_init();

    // Initialize pins and interrupts
    Set_System();

    // Configure the USB hardware
    USB_Interrupts_Config();
    Set_USBClock();
    USB_Init();

#ifndef USE_AVAGO
    tcs_init();
#ifdef USE_7_SEG
    segment_init();
    if (g_startup_failed)
        segment_set_error();
    else
        segment_set_hello();
#endif /* USE_7_SEG */
#endif /* USE_AVAGO */

    uint32_t timestamp = tick_get();

    uint8_t sample[3] = {0};
    uint16_t raw[3] = {0};
    uint8_t sample_target[3] = {0};
    bool button_pressed = 0;
    bool got_trigger = 0;
    bool triggered = 0;
    bool got_report = 0;
    uint8_t mode = MODE_IDLE;
    uint8_t button_index = 0;
    uint32_t start_timestamp = 0;
    uint16_t command_id = 0;
    uint32_t trigger_timestamp = 0;
    uint32_t temp_timestamp = timestamp;

    // set the default values
    // TODO: for now, set the threshold to 128, 128, 255 since blue isn't connected
    config.threshold[0] = 128;
    config.threshold[1] = 128;
    config.threshold[2] = 255;

    feature_config_set(&config);

    // Check if we are in the programming/test jig
    uint8_t factory_mode = GPIO_ReadInputDataBit(FACTORY_PORT, FACTORY_PIN);

    while (1) {
        // Spin waiting for an event to happen
        do {
            temp_timestamp = tick_get();
            button_pressed |= check_button_pressed(temp_timestamp);
            got_report = feature_got_report();
        } while ((timestamp == temp_timestamp) &&
                 !button_pressed &&
                 !got_report);

        // Update feature reports if we got one
        if (got_report) {
            got_report = 0;
        }

        feature_config_get(&config);

        if (feature_calibrate_get(&calibrate)) {
            calibrate_update(sample_avg, calibrate.calibrate);
        }

        if (feature_dfu_get(&dfu)) {
            // TODO: Add HID bootloader support back to the Open Source release

            // Restart the device if we are told to
            if (dfu.use_dfu)
                Reset_Device();
        }

        if (feature_trigger_get(&trigger)) {
            got_trigger = 1;
            trigger_timestamp = timestamp;
        }

        if (feature_display_get(&display)) {
#ifdef USE_7_SEG
        	uint32_t value;

            // Update the display if we are in a mode where the PC sets it
            switch (display.mode) {
                case SEGMENT_MANUAL:
                	// Use memcpy to be safe about memory alignment
                	memcpy(&value, display.value, sizeof(uint32_t));
                    segment_set_value(value);
                    break;

                case SEGMENT_RAW:
                    segment_set_raw(display.value);
                    break;
            }
#endif /* USE_7_SEG */
        }

        // update the sample at every timestamp tick
        if (timestamp != temp_timestamp) {
            timestamp = temp_timestamp;
            // check if the color changed to the trigger value
#ifdef USE_AVAGO
            // TODO: connect blue
            raw[0] = ADC_GetConversionValue(ADC1);
            raw[1] = ADC_GetConversionValue(ADC2);
#else /* USE_AVAGO */
            tcs_end_measurement();
            tcs_start_measurement();
            tcs_read(raw);
#ifdef USE_7_SEG
            // only update the display if we are in auto mode and changes are happening
            if ((mode != MODE_IDLE) && (display.mode == SEGMENT_AUTO))
                segment_set_value(timestamp - start_timestamp);
            segment_update();
#endif /* USE_7_SEG */
#endif /* USE_AVAGO */

            update_average(raw);
            calibrate_apply(raw, sample);

            if (update_sample(timestamp, sample, sample_target, config.threshold) &&
                (mode != MODE_IDLE)) {
                triggered = 1;
            }
        }

        // Factory testing has several stages
        if (factory_mode) {
            if (button_pressed) {
                button_pressed = 0;
                factory_mode++;

                // Go into normal operation once testing is done
                if (factory_mode == FACTORY_DONE) {
                    factory_mode = FACTORY_NONE;
                    if (g_startup_failed)
                        segment_set_error();
                    else
                        segment_set_hello();
                }
            }

            update_factory(factory_mode, sample);

            continue;
        }

        if (triggered) {
            mode = MODE_IDLE;
            triggered = 0;
            // send the elapsed time if the trigger occurred
            uint32_t elapsed = timestamp - start_timestamp;
            trigger_packet(command_id, timestamp, elapsed > UINT16_MAX ? UINT16_MAX : elapsed, sample, sample_target);
            ep1_ready = 0;
        } else if (((mode == MODE_IDLE) || (timestamp - start_timestamp > TIMEOUT_OPTION)) && got_trigger) {
            // Only start the trigger if we aren't currently waiting for one
            mode = MODE_TRIGGER;
            got_trigger = 0;
            start_timestamp = trigger_timestamp;
            command_id = trigger.command_id;
            memcpy(sample_target, trigger.target, 3);
            trigger_start_packet(command_id, timestamp, sample_target);
            ep1_ready = 0;
        } else if (ep1_ready && button_pressed) {
            // increment the count used for the button each time
            button_index++;
            trigger_button_packet(button_index, timestamp);
            if ((display.mode == SEGMENT_AUTO))
                segment_set_waiting();
            button_pressed = 0;
            ep1_ready = 0;
        } else if (ep1_ready && config.send_samples) {
            // the normal sample packet has the lowest priority
            sample_packet_generate();
            sample_packet_send();
            ep1_ready = 0;
        }
    }
}

void EP1_IN_Callback(void)
{
    ep1_ready = 1;
}
