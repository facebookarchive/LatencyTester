/*****************************************************************************

Filename    :   i2c.c
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

#include "i2c.h"
#include <stdbool.h>
#include "stm32f10x_i2c.h"
#include "stm32f10x_gpio.h"

// To make sure we never get stuck forever
#define I2C_TIMEOUT  (0xFFFF)

static bool i2c_send_start(i2c_p i2c, uint8_t direction)
{
    uint32_t timeout = I2C_TIMEOUT;
    // Wait for the bus if starting transmission
    if (direction == I2C_Direction_Transmitter) {
        while (I2C_GetFlagStatus(i2c->i2c_port, I2C_FLAG_BUSY))
            if (!timeout--) return 0;
    }

    I2C_GenerateSTART(i2c->i2c_port, ENABLE);
    // Wait for start success
    timeout = I2C_TIMEOUT;
    while (!I2C_CheckEvent(i2c->i2c_port, I2C_EVENT_MASTER_MODE_SELECT))
        if (!timeout--) return 0;

    I2C_Send7bitAddress(i2c->i2c_port, i2c->address, direction);
    timeout = I2C_TIMEOUT;
    if (direction == I2C_Direction_Transmitter) {
        while (!I2C_CheckEvent(i2c->i2c_port, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
            if (!timeout--) return 0;
    } else {
        while (!I2C_CheckEvent(i2c->i2c_port, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
            if (!timeout--) return 0;
    }

    return 1;
}

void i2c_init(i2c_p i2c, I2C_TypeDef *i2c_port, uint8_t address, uint32_t rcc, GPIO_TypeDef* gpio, uint32_t gpio_rcc, uint16_t scl, uint16_t sda)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    I2C_InitTypeDef I2C_InitStructure;

    i2c->i2c_port = i2c_port;
    i2c->address = address;
    i2c->rcc = rcc;

    // Configure the clocks
    RCC_APB2PeriphClockCmd(gpio_rcc, ENABLE);
    RCC_APB1PeriphClockCmd(rcc, ENABLE);

    // Configure the pins
    GPIO_InitStruct.GPIO_Pin = scl | sda;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(gpio, &GPIO_InitStruct);

    // Configure the i2c peripheral
    I2C_InitStructure.I2C_ClockSpeed = 400000; // 400kHz
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = 0;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;

    I2C_Cmd(i2c->i2c_port, ENABLE);
    I2C_Init(i2c->i2c_port, &I2C_InitStructure);
}

void i2c_deinit(i2c_p i2c)
{
    I2C_Cmd(i2c->i2c_port, DISABLE);
    I2C_DeInit(i2c->i2c_port);

    // Shut off the clock
    RCC_APB1PeriphClockCmd(i2c->rcc, DISABLE);
}

uint8_t i2c_write(i2c_p i2c, uint8_t reg, uint8_t *buf, uint8_t num_bytes)
{
    // Give up if we can't grab the bus
    if (!i2c_send_start(i2c, I2C_Direction_Transmitter))
        return 0;

    // Select the register, i2c always uses the Block transaction type
    I2C_SendData(i2c->i2c_port, reg);
    uint32_t timeout = I2C_TIMEOUT;
    while (!I2C_CheckEvent(i2c->i2c_port, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
        if (!timeout--) return 0;

    // Send the number of bytes to be written (probably ignored for i2c)
    I2C_SendData(i2c->i2c_port, num_bytes);
    timeout = I2C_TIMEOUT;
    while (!I2C_CheckEvent(i2c->i2c_port, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
        if (!timeout--) return 0;

    uint8_t i;
    for (i = 0; i < num_bytes; i++) {
        // Send the actual value
        I2C_SendData(i2c->i2c_port, buf[i]);
        timeout = I2C_TIMEOUT;
        while (!I2C_CheckEvent(i2c->i2c_port, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
            if (!timeout--) break;
    }

    I2C_GenerateSTOP(i2c->i2c_port, ENABLE);
    // Wait for the stop to succeed
    timeout = I2C_TIMEOUT;
    while(i2c->i2c_port->CR1 & I2C_CR1_STOP)
        if (!timeout--) break;

    return i;
}

uint8_t i2c_read(i2c_p i2c, uint8_t reg, uint8_t *buf, uint8_t num_bytes)
{
    // First go to write mode to write the address
    if (!i2c_send_start(i2c, I2C_Direction_Transmitter))
        return 0;

    // Select the register, i2c always uses the Block transaction type
    I2C_SendData(i2c->i2c_port, reg);
    uint32_t timeout = I2C_TIMEOUT;
    while (!I2C_CheckEvent(i2c->i2c_port, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
        if (!timeout--) return 0;

    // Then go into read mode
    if (!i2c_send_start(i2c, I2C_Direction_Receiver))
        return 0;

    // Enable acking of read bytes
    I2C_AcknowledgeConfig(i2c->i2c_port, ENABLE);

    uint8_t i;
    for (i = 0; i < num_bytes-1; i++) {
        timeout = I2C_TIMEOUT;
        while (!I2C_CheckEvent(i2c->i2c_port, I2C_EVENT_MASTER_BYTE_RECEIVED))
            if (!timeout--) break;
        buf[i] = I2C_ReceiveData(i2c->i2c_port);
    }

    // Nack the last read
    I2C_AcknowledgeConfig(i2c->i2c_port, DISABLE);
    // Queue a stop after this read
    I2C_GenerateSTOP(i2c->i2c_port, ENABLE);
    // Just wait for the final byte to arrive
    timeout = I2C_TIMEOUT;
    while (!I2C_GetFlagStatus(i2c->i2c_port, I2C_FLAG_RXNE))
        if (!timeout--) return i;
    buf[i++] = I2C_ReceiveData(i2c->i2c_port);
    // Wait for the stop to succeed
    timeout = I2C_TIMEOUT;
    while(i2c->i2c_port->CR1 & I2C_CR1_STOP)
        if (!timeout--) break;

    return i;
}
