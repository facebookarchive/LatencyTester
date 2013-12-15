/*****************************************************************************

Filename    :   sample_packet.h
Content     :   HID sample packet report generator
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

#ifndef _SAMPLE_PACKET_H_
#define _SAMPLE_PACKET_H_

#include <stdint.h>

void sample_packet_update(uint16_t timestamp, uint8_t *sample);

void sample_packet_generate(void);

void sample_packet_send(void);

#endif /* _SAMPLE_PACKET_H_ */
