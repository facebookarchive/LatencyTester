/*****************************************************************************

Filename    :   tcs3414.h
Content     :   TAOS TCS3414 color sensor configuration
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

#ifndef _TCS3414_H_
#define _TCS3414_H_

#include <stdint.h>
#include <stdbool.h>

void tcs_init(void);
void tcs_sleep(void);
bool tcs_start_measurement(void);
bool tcs_end_measurement(void);
bool tcs_read(uint16_t *sample);

#endif /* _TCS3414_H_ */
