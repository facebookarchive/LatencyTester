/*****************************************************************************

Filename    :   tcs3414.c
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

#include "tcs3414.h"
#include "tcs3414_register_map.h"
#include "stm32f10x.h"
#include "platform_config.h"
#include "i2c.h"

#define TCS_ADDR (0x39 << 1)
#define TCS_COMMAND(a) ((a & 0x1F) | TCS_CMD | TRANSACTION_BLOCK)

typedef struct tcs_struct {
    i2c_t i2c;
    bool suspended;
} tcs_t, *tcs_p;

static tcs_t g_tcs = {{0}};

void tcs_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    // Configure the clock
    RCC_APB2PeriphClockCmd(TCS_SYNC_RCC, ENABLE);

    GPIO_InitStruct.GPIO_Pin = TCS_SYNC_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(TCS_SYNC_PORT, &GPIO_InitStruct);

    // Pull down the Sync line to start with
    GPIO_ResetBits(TCS_SYNC_PORT, TCS_SYNC_PIN);

    i2c_init(&g_tcs.i2c, TCS_I2C, TCS_ADDR, TCS_I2C_RCC, TCS_SCL_SDA_PORT, TCS_SCL_SDA_RCC, TCS_SCL_PIN, TCS_SDA_PIN);

    // Power on the device in pulse mode
    uint8_t regs[] = {TCS_POWER, INTEG_MODE_PULSE};
    i2c_write(&g_tcs.i2c, TCS_COMMAND(CONTROL), regs, 2);

    uint8_t gain = GAIN_64;
    i2c_write(&g_tcs.i2c, TCS_COMMAND(GAIN), &gain, 1);

    // Enable the ADCs
    uint8_t enable = ADC_EN | TCS_POWER;
    i2c_write(&g_tcs.i2c, TCS_COMMAND(CONTROL), &enable, 1);
}

void tcs_sleep(void)
{
    // Shut off the device
    uint8_t control = 0;
    i2c_write(&g_tcs.i2c, TCS_COMMAND(CONTROL), &control, 1);

    i2c_deinit(&g_tcs.i2c);
}

static void tcs_reinit(void)
{
    // reset the device if something terrible has happened
    tcs_sleep();
    tcs_init();
}

bool tcs_start_measurement(void)
{
    // Enable the ADCs
    uint8_t enable = ADC_EN | TCS_POWER;
    if (!i2c_write(&g_tcs.i2c, TCS_COMMAND(CONTROL), &enable, 1)) {
        tcs_reinit();
        return 0;
    }

    // Pull up the Sync line
    GPIO_SetBits(TCS_SYNC_PORT, TCS_SYNC_PIN);
    // TODO: pull the sync line back down on a timer

    return 1;
}

bool tcs_end_measurement(void)
{
    // Pull down the Sync line
    GPIO_ResetBits(TCS_SYNC_PORT, TCS_SYNC_PIN);

    // ADC needs to be disabled before the next measurement
    uint8_t enable = TCS_POWER;
    if (!i2c_write(&g_tcs.i2c, TCS_COMMAND(CONTROL), &enable, 1)) {
        tcs_reinit();
        return 0;
    }

    return 1;
}

bool tcs_read(uint16_t *sample)
{
    // the data is little-endian, so we can just read it directly
    if (!i2c_read(&g_tcs.i2c, TCS_COMMAND(DATA1LOW), (uint8_t *)sample, 6)) {
        tcs_reinit();
        return 0;
    }

    // the sensor is GRB, so swap red and green
    uint16_t temp = sample[0];
    sample[0] = sample[1];
    sample[1] = temp;

    return 1;
}
