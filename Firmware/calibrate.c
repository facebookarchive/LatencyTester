/*****************************************************************************

Filename    :   calibrate.c
Content     :   Basic linear calibration of a pre-amplified photodiode
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

#include "calibrate.h"

typedef struct calibrate_s {
    uint16_t min_value[3];
    uint16_t max_value[3];
    float scale[3];
} calibrate_t;

// default values that work for the 7"
#ifdef USE_AVAGO
// TODO: figure out the Blue values
static calibrate_t cal = {{6, 6, 0}, {40, 71, 255}, {7.5f, 3.9f, 1.0f}};
#else
static calibrate_t cal = {{1, 1, 1}, {64, 111, 81}, {4.04761886f, 2.3181875f, 3.1875f}};
#endif /* USE_AVAGO */

void calibrate_update(float *sample, uint8_t *target)
{
    for (uint8_t i = 0; i < 3; i++) {
        // currently, we only care about 0 and 255
        if (target[i] == 0) {
            cal.min_value[i] = (uint16_t)sample[i];
        } else if (target[i] == 255) {
            cal.max_value[i] = (uint16_t)sample[i];
        }

        // update the scale if the numbers are valid
        if (cal.max_value[i] > cal.min_value[i])
            cal.scale[i] = 255.0f/((float)(cal.max_value[i] - cal.min_value[i]));
    }
}

void calibrate_apply(uint16_t *sample, uint8_t *output)
{
    for (uint8_t i = 0; i < 3; i++) {
        uint16_t val = 0;
        // clamp to 0
        if (sample[i] > cal.min_value[i])
            val = (uint16_t)(cal.scale[i]*(sample[i] - cal.min_value[i]));

        // clamp to 255
        output[i] = val > 255 ? 255 : val;
    }
}
