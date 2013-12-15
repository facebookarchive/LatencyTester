/*****************************************************************************

Filename    :   sample_packet.c
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

#include "sample_packet.h"
#include "hw_config.h"
#include "usb_lib.h"
#include "usb_istr.h"
#include <string.h>

#define SAMPLE_PACKET_SIZE 64
#define SAMPLE_PACKET_HEADER 4
#define SAMPLE_SIZE 3
#define MAX_SAMPLES 20

static uint8_t packet_buf[SAMPLE_PACKET_SIZE];
static uint8_t num_samples = 0;

void sample_packet_update(uint16_t timestamp, uint8_t *sample)
{
    if (!num_samples)
        *(uint16_t *)(packet_buf+2) = timestamp;

    if (num_samples == MAX_SAMPLES) {
        // throw away the oldest sample if the buffer is full
        memmove(packet_buf+SAMPLE_PACKET_HEADER, packet_buf+SAMPLE_PACKET_HEADER+SAMPLE_SIZE, (MAX_SAMPLES-1)*SAMPLE_SIZE);
        memcpy(packet_buf+SAMPLE_PACKET_SIZE-SAMPLE_SIZE, sample, SAMPLE_SIZE);
        *(uint16_t *)(packet_buf+2) += 1;
    } else {
        memcpy(packet_buf+SAMPLE_PACKET_HEADER+num_samples*SAMPLE_SIZE, sample, SAMPLE_SIZE);
        num_samples++;
    }
}

void sample_packet_generate(void)
{
    packet_buf[0] = 1;
    packet_buf[1] = num_samples;
    num_samples = 0;
}

void sample_packet_send(void)
{
    USB_SIL_Write(EP1_IN, packet_buf, SAMPLE_PACKET_SIZE);

    // Mark that the endpoint has valid data
    SetEPTxValid(ENDP1);
}
