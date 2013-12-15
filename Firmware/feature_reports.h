/*****************************************************************************

Filename    :   feature_reports.h
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

#ifndef _FEATURE_REPORTS_H_
#define _FEATURE_REPORTS_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct feature_config_struct {
    bool send_samples;
    uint8_t threshold[3];
} feature_config_s, *feature_config_p;

typedef struct feature_dfu_struct {
    uint16_t command_id;
    bool use_dfu;
} feature_dfu_s, *feature_dfu_p;

typedef struct feature_calibrate_struct {
    uint8_t calibrate[3];
} feature_calibrate_s, *feature_calibrate_p;

typedef struct feature_trigger_struct {
    uint16_t command_id;
    uint8_t target[3];
} feature_trigger_s, *feature_trigger_p;

typedef struct feature_display_struct {
    uint8_t mode;
    uint8_t value[4];
} feature_display_s, *feature_display_p;

// Callbacks for USB
uint8_t *feature_reports_get_report(uint16_t length);
uint8_t *feature_reports_set_report(uint16_t length);

void feature_reports_parse_report(void);

void feature_config_set(feature_config_p config);

bool feature_got_report(void);
bool feature_config_get(feature_config_p config);
bool feature_dfu_get(feature_dfu_p dfu);
bool feature_calibrate_get(feature_calibrate_p calibrate);
bool feature_trigger_get(feature_trigger_p trigger);
bool feature_display_get(feature_display_p display);

#endif /* _FEATURE_REPORTS_H_ */
