/*****************************************************************************

Filename    :   segment.h
Content     :   7 Segment display driver interface
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

#ifndef _SEGMENT_H_
#define _SEGMENT_H_

#include <stdint.h>

enum {
    SEGMENT_AUTO = 0,
    SEGMENT_MANUAL = 1,
    SEGMENT_RAW = 2
};

void segment_init(void);

void segment_deinit(void);

void segment_update(void);

void segment_set_error(void);

void segment_set_hello(void);

void segment_set_waiting(void);

void segment_set_raw(const uint8_t *raw);

void segment_set_value(uint32_t value);

#endif /* _SEGMENT_H_ */
