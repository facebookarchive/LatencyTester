/*****************************************************************************

Filename    :   feature_reports.c
Content     :   HID feature report handler
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

#include "feature_reports.h"
#include "platform_config.h"
#include "usb_core.h"
#include "usb_init.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

enum {
    FEATURE_CONFIG = 5,
    FEATURE_DFU = 6,
    FEATURE_CALIBRATE = 7,
    FEATURE_TRIGGER = 8,
    FEATURE_DISPLAY = 9,
    FEATURE_MAX = 10
};

static const uint16_t feature_report_size[FEATURE_MAX] = {0, 0, 0, 0, 0, 5, 4, 4, 6, 6};

feature_config_s global_config = {0};
bool global_config_set = 0;
feature_dfu_s global_dfu = {0};
bool global_dfu_set = 0;
feature_calibrate_s global_calibrate = {{0}};
bool global_calibrate_set = 0;
feature_trigger_s global_trigger = {0};
bool global_trigger_set = 0;
feature_display_s global_display = {0};
bool global_display_set = 0;
bool got_report = 0;

static uint8_t feature_buf[64];

uint8_t *feature_reports_get_report(uint16_t length)
{
    // If it is an unrecognized report, just return
    if (!pInformation->USBwValue0 || (pInformation->USBwValue0 >= FEATURE_MAX))
        return NULL;

    // If we are in the first step of supplying length, set the report length
    if (!length) {
        pInformation->Ctrl_Info.Usb_wLength = feature_report_size[pInformation->USBwValue0];
        return NULL;
    } else {
        feature_buf[0] = pInformation->USBwValue0;

        // Fill the buffer based on the report type
        switch (pInformation->USBwValue0) {
            case FEATURE_CONFIG:
                feature_buf[1] = global_config.send_samples;
                memcpy(feature_buf+2, global_config.threshold, 3);
                return feature_buf;
                break;

            case FEATURE_DFU:
                *(uint16_t *)(feature_buf+1) = global_dfu.command_id;
                feature_buf[3] = global_dfu.use_dfu;
                return feature_buf;
                break;

            case FEATURE_CALIBRATE:
                memcpy(feature_buf+1, global_calibrate.calibrate, 3);
                return feature_buf;
                break;

            case FEATURE_TRIGGER:
                *(uint16_t *)(feature_buf+1) = global_trigger.command_id;
                memcpy(feature_buf+3, global_trigger.target, 3);
                return feature_buf;
                break;

            case FEATURE_DISPLAY:
                feature_buf[1] = global_display.mode;
                memcpy(feature_buf+2, global_display.value, 4);
                return feature_buf;
                break;

            default:
                break;
        }
    }

    return NULL;
}

uint8_t *feature_reports_set_report(uint16_t length)
{
    // If it is an unrecognized report, just return
    if (!pInformation->USBwValue0 || (pInformation->USBwValue0 >= FEATURE_MAX))
        return NULL;

    // If we are in the first step of supplying length, set the report length
    if (!length) {
        pInformation->Ctrl_Info.Usb_wLength = feature_report_size[pInformation->USBwValue0];
        return NULL;
    } else {
        return feature_buf + pInformation->Ctrl_Info.Usb_wOffset;
    }
}

void feature_reports_parse_report(void)
{
    got_report = 1;

    switch (feature_buf[0]) {
        case FEATURE_CONFIG:
            global_config.send_samples = feature_buf[1] & 0x01;
            memcpy(global_config.threshold, feature_buf+2, 3);
            global_config_set = 1;
            break;

        case FEATURE_DFU:
            global_dfu.command_id = *(uint16_t *)(feature_buf+1);
            global_dfu.use_dfu = (bool)feature_buf[3];
            global_dfu_set = 1;
            break;

        case FEATURE_CALIBRATE:
            memcpy(global_calibrate.calibrate, feature_buf+1, 3);
            global_calibrate_set = 1;
            break;

        case FEATURE_TRIGGER:
            global_trigger.command_id = *(uint16_t *)(feature_buf+1);
            memcpy(global_trigger.target, feature_buf+3, 3);
            global_trigger_set = 1;
            break;

        case FEATURE_DISPLAY:
            global_display.mode = feature_buf[1];
            memcpy(global_display.value, feature_buf+2, 4);
            global_display_set = 1;
            break;

        default:
            break;
    }
}

void feature_config_set(feature_config_p config)
{
    memcpy(&global_config, config, sizeof(feature_config_s));
}

bool feature_got_report(void)
{
    bool ret = got_report;

    got_report = 0;

    return ret;
}

bool feature_config_get(feature_config_p config)
{
    if (global_config_set) {
        global_config_set = 0;
        memcpy(config, &global_config, sizeof(feature_config_s));
        return 1;
    }

    return 0;
}

bool feature_dfu_get(feature_dfu_p dfu)
{
    if (global_dfu_set) {
        global_dfu_set = 0;
        memcpy(dfu, &global_dfu, sizeof(feature_dfu_s));
        return 1;
    }

    return 0;
}

bool feature_calibrate_get(feature_calibrate_p calibrate)
{
    if (global_calibrate_set) {
        global_calibrate_set = 0;
        memcpy(calibrate, &global_calibrate, sizeof(feature_calibrate_s));

        return 1;
    }

    return 0;
}

bool feature_trigger_get(feature_trigger_p trigger)
{
    if (global_trigger_set) {
        global_trigger_set = 0;
        memcpy(trigger, &global_trigger, sizeof(feature_trigger_s));
        return 1;
    }

    return 0;
}

bool feature_display_get(feature_display_p display)
{
    if (global_display_set) {
        global_display_set = 0;
        memcpy(display, &global_display, sizeof(feature_display_s));
        return 1;
    }

    return 0;
}
