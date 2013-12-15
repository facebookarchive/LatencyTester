/*****************************************************************************

Filename    :   tcs3414_register_map.h
Content     :   TAOS TCS3414 color sensor register map
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


#ifndef _TCS3414_REGISTER_MAP_
#define _TCS3414_REGISTER_MAP_

#define CONTROL 0x00
#define TIMING 0x01
#define INTERRUPT 0x02
#define INT_SOURCE 0x03
#define ID 0x04
#define GAIN 0x07
#define LOW_THRESH_LOW_BYTE 0x08
#define LOW_THRESH_HIGH_BYTE 0x09
#define HIGH_THRESH_LOW_BYTE 0x0A
#define HIGH_THRESH_HIGH_BYTE 0x0B
#define DATA1LOW 0x10
#define DATA1HIGH 0x11
#define DATA2LOW 0x12
#define DATA2HIGH 0x13
#define DATA3LOW 0x14
#define DATA3HIGH 0x15
#define DATA4LOW 0x16
#define DATA4HIGH 0x17

// Command register
#define TCS_CMD (1 << 7)
#define TRANSACTION_BYTE (0)
#define TRANSACTION_WORD (1 << 5)
#define TRANSACTION_BLOCK (2 << 5)
#define TRANSACTION_INTERRUPT (3 << 5)

// Control register
#define ADC_VALID (1 << 4)
#define ADC_EN (1 << 1)
#define TCS_POWER (1)

// Timing register
#define SYNC_EDGE (1 << 6)
#define INTEG_MODE_FREE (0)
#define INTEG_MODE_MANUAL (1 << 4)
#define INTEG_MODE_SYNC (2 << 4)
#define INTEG_MODE_PULSE (3 << 4)
#define PARAM_12MS (0)
#define PARAM_100MS (1)
#define PARAM_400MS (2)

// Interrupt register
#define INTR_STOP (1 << 6)
#define INTR_DISABLED (0)
#define INTR_LEVEL (1 << 4)
#define INTR_SMB (2 << 4)
#define INTR_BOTH (3 << 4)
#define PERSIST_EVERY (0)
#define PERSIST_SINGLE (1)
#define PERSIST_DECISEC (2)
#define PERSIST_SEC (3)

// Interrupt Source register
#define INT_SOURCE_GREEN (0)
#define INT_SOURCE_RED (1)
#define INT_SOURCE_BLUE (2)
#define INT_SOURCE_CLEAR (3)

// ID register
#define PARTNO (0xF0)
#define REVNO (0x0F)

// Gain register
#define GAIN_1 (0)
#define GAIN_4 (1 << 4)
#define GAIN_16 (2 << 4)
#define GAIN_64 (3 << 4)
// TODO: Prescaler values

#endif /* _TCS3414_REGISTER_MAP_ */
