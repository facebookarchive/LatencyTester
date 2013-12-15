/*****************************************************************************

Filename    :   segment.c
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

#include "segment.h"
#include "spi.h"
#include "stm32f10x.h"
#include "platform_config.h"
#include <stdbool.h>
#include <string.h>

#define NUM_DIGITS 4

typedef struct segment_struct {
	spi_t spi;
  bool suspended;
  uint8_t digits[NUM_DIGITS];
  uint8_t index;
} segment_t, *segment_p;

//                                      0     1     2     3     4     5     6     7     8     9
static const uint8_t bitmasks[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
#define COLON (0x03)
#define LETTER_H (0x76)
#define LETTER_I (0x06)
#define LETTER_E (0x79)
#define LETTER_DASH (0x40)
#define LETTER_EXCLAMATION (0x82)
static const uint16_t pins[NUM_DIGITS] = {DIGIT1_PIN, COLON_PIN, DIGIT2_PIN, DIGIT3_PIN};
static GPIO_TypeDef* ports[NUM_DIGITS] = {DIGIT1_PORT, COLON_PORT, DIGIT2_PORT, DIGIT3_PORT};
static segment_t g_segment = {{0}};

void segment_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    // Initialize the constant current sink driver
    spi_init(&g_segment.spi, TLC_SPI, TLC_SPI_PRESCALER, TLC_SPI_RCC, TLC_SCK_MISO_MOSI_PORT, TLC_SCK_MISO_MOSI_RCC, TLC_MISO_PIN, TLC_MOSI_PIN, TLC_SCK_PIN, TLC_SS_PIN);

    RCC_APB2PeriphClockCmd(DIGIT1_RCC | COLON_RCC | DIGIT2_RCC | DIGIT3_RCC | TLC_OE_RCC, ENABLE);

    // Initialize the digit select pins
    for (uint8_t i = 0; i < NUM_DIGITS; i++) {
        GPIO_InitStruct.GPIO_Pin = pins[i];
        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(ports[i], &GPIO_InitStruct);
        GPIO_ResetBits(ports[i], pins[i]);
    }

    GPIO_InitStruct.GPIO_Pin = TLC_OE_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(TLC_OE_PORT, &GPIO_InitStruct);
    GPIO_ResetBits(TLC_OE_PORT, TLC_OE_PIN);

    g_segment.suspended = 0;
}

void segment_deinit(void)
{
    // Turn off the segment
    GPIO_SetBits(ports[g_segment.index], pins[g_segment.index]);
    GPIO_SetBits(TLC_OE_PORT, TLC_OE_PIN);
    spi_deinit(&g_segment.spi);

    g_segment.suspended = 1;
}

void segment_update(void)
{
    if (g_segment.suspended)
        return;

    // Turn off the current digit
    GPIO_SetBits(ports[g_segment.index], pins[g_segment.index]);

    // Move to the next one
    g_segment.index++;
    g_segment.index %= NUM_DIGITS;

    spi_write_raw(&g_segment.spi, g_segment.digits[g_segment.index]);

    // Turn on the new digit
    GPIO_ResetBits(ports[g_segment.index], pins[g_segment.index]);
}

void segment_set_error(void)
{
    g_segment.digits[0] = LETTER_E;
    g_segment.digits[1] = 0;
    g_segment.digits[2] = LETTER_EXCLAMATION;
    g_segment.digits[3] = LETTER_EXCLAMATION;
}

void segment_set_hello(void)
{
    g_segment.digits[0] = LETTER_H;
    g_segment.digits[1] = 0;
    g_segment.digits[2] = LETTER_I;
    g_segment.digits[3] = LETTER_EXCLAMATION;
}

void segment_set_waiting(void)
{
    g_segment.digits[0] = LETTER_DASH;
    g_segment.digits[1] = 0;
    g_segment.digits[2] = LETTER_DASH;
    g_segment.digits[3] = LETTER_DASH;
}

void segment_set_raw(const uint8_t *raw)
{
    memcpy(g_segment.digits, raw, 4);
}

void segment_set_value(uint32_t value)
{
    if (value < 1000) {
        uint8_t ones = value % 10;
        uint8_t tens = value/10 % 10;
        uint8_t hundreds = value/100 % 10;
        // Only allow the ones digit to appear as a leading 0
        g_segment.digits[3] = bitmasks[ones];
        g_segment.digits[2] = (tens || hundreds) ? bitmasks[tens] : 0;
        g_segment.digits[1] = 0;
        g_segment.digits[0] = hundreds ? bitmasks[hundreds] : 0;
    } else {
        // switch to seconds mode once we are above 1 second
        uint16_t seconds = value/1000 % 60;
        uint16_t minutes = value/60000;

        g_segment.digits[3] = bitmasks[seconds % 10];
        g_segment.digits[2] = bitmasks[seconds/10];
        g_segment.digits[1] = COLON;
        g_segment.digits[0] = bitmasks[minutes % 10];
    }
}
