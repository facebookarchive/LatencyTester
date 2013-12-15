/*****************************************************************************

Filename    :   trigger_packet.h
Content     :   Trigger IN report generator
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

#ifndef _TRIGGER_PACKET_H_
#define _TRIGGER_PACKET_H_

#include <stdint.h>

void trigger_packet(uint16_t command_id, uint16_t timestamp, uint16_t elapsed, uint8_t *value, uint8_t *target);

void trigger_start_packet(uint16_t command_id, uint16_t timestamp, uint8_t *value);

void trigger_button_packet(uint16_t command_id, uint16_t timestamp);

#endif /* _TRIGGER_PACKET_H_ */
