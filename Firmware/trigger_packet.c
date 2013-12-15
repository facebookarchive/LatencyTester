/*****************************************************************************

Filename    :   trigger_packet.c
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

#include "trigger_packet.h"
#include "hw_config.h"
#include "usb_lib.h"
#include "usb_istr.h"
#include <string.h>

enum {
    TRIGGER_ID = 2,
    START_ID = 3,
    BUTTON_ID = 4,
    MAX_ID = 5
};

static const uint8_t report_size[MAX_ID] = {0, 64, 13, 8, 5};

static uint8_t packet_buf[64];

void trigger_packet(uint16_t command_id, uint16_t timestamp, uint16_t elapsed, uint8_t *value, uint8_t *target)
{
    packet_buf[0] = TRIGGER_ID;
    *(uint16_t *)(packet_buf+1) = command_id;
    *(uint16_t *)(packet_buf+3) = timestamp;
    *(uint16_t *)(packet_buf+5) = elapsed;
    memcpy(packet_buf+7, value, 3);
    memcpy(packet_buf+10, target, 3);

    USB_SIL_Write(EP1_IN, packet_buf, report_size[TRIGGER_ID]);
    SetEPTxValid(ENDP1);
}

void trigger_start_packet(uint16_t command_id, uint16_t timestamp, uint8_t *value)
{
    packet_buf[0] = START_ID;
    *(uint16_t *)(packet_buf+1) = command_id;
    *(uint16_t *)(packet_buf+3) = timestamp;
    memcpy(packet_buf+5, value, 3);

    USB_SIL_Write(EP1_IN, packet_buf, report_size[START_ID]);
    SetEPTxValid(ENDP1);
}

void trigger_button_packet(uint16_t command_id, uint16_t timestamp)
{
    packet_buf[0] = BUTTON_ID;
    *(uint16_t *)(packet_buf+1) = command_id;
    *(uint16_t *)(packet_buf+3) = timestamp;

    USB_SIL_Write(EP1_IN, packet_buf, report_size[BUTTON_ID]);
    SetEPTxValid(ENDP1);
}
