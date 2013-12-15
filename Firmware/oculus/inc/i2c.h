/*****************************************************************************

Filename    :   i2c.h
Content     :   Lightweight i2c wrapper for STM32
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

#ifndef _I2C_H_
#define _I2C_H_

#include <stdint.h>
#include "stm32f10x.h"

typedef struct i2c_struct {
    I2C_TypeDef *i2c_port;
    uint8_t address;
    uint32_t rcc;
} i2c_t, *i2c_p;

void i2c_init(i2c_p i2c, I2C_TypeDef *i2c_port, uint8_t address, uint32_t rcc, GPIO_TypeDef* gpio, uint32_t gpio_rcc, uint16_t scl, uint16_t sda);

void i2c_deinit(i2c_p i2c);

uint8_t i2c_write(i2c_p i2c, uint8_t reg, uint8_t *buf, uint8_t num_bytes);

uint8_t i2c_read(i2c_p i2c, uint8_t reg, uint8_t *buf, uint8_t num_bytes);

#endif /* _I2C_H_ */
