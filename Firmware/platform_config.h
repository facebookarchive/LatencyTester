/*****************************************************************************

Filename    :   platform_config.h
Content     :   Pin definitions for the Latency Tester
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

#ifndef _PLATFORM_CONFIG_H_
#define _PLATFORM_CONFIG_H_

#if defined(STM32L1XX_MD) || defined(STM32L1XX_HD)|| defined(STM32L1XX_MD_PLUS)
  #include "stm32l1xx.h"
#elif defined (STM32F10X_LD) || defined (STM32F10X_MD) || defined (STM32F10X_HD) || defined (STM32F10X_XL) || defined (STM32F10X_CL)
  #include "stm32f10x.h"
#endif

/* Define the STM32F10x hardware depending on the used evaluation board */
#if defined (USE_TRACKER_V1)

  #define USB_DISCONNECT                      GPIOA
  #define USB_DISCONNECT_PIN                  GPIO_Pin_2
  #define RCC_APB2Periph_GPIO_DISCONNECT      RCC_APB2Periph_GPIOA

#ifdef USE_AVAGO
  #define RED_PORT                            GPIOB
  #define RED_PIN                             GPIO_Pin_0
  #define RED_RCC                             RCC_APB2Periph_GPIOB
  #define RED_CHANNEL                         ADC_Channel_8
  #define GREEN_PORT                          GPIOB
  #define GREEN_PIN                           GPIO_Pin_1
  #define GREEN_RCC                           RCC_APB2Periph_GPIOB
  #define GREEN_CHANNEL                       ADC_Channel_9
  #define BLUE_PORT                           GPIOA
  #define BLUE_PIN                            GPIO_Pin_3
  #define BLUE_RCC                            RCC_APB2Periph_GPIOA
  #define BLUE_CHANNEL                        ADC_Channel_3
  #define BUTTON_PORT                         GPIOB
  #define BUTTON_PIN                          GPIO_Pin_10
  #define BUTTON_RCC                          RCC_APB2Periph_GPIOB
#else
  #define TCS_I2C                             I2C2
  #define TCS_I2C_RCC                         RCC_APB1Periph_I2C2
  #define TCS_SCL_PIN                         GPIO_Pin_10
  #define TCS_SDA_PIN                         GPIO_Pin_11
  #define TCS_SCL_SDA_PORT                    GPIOB
  #define TCS_SCL_SDA_RCC                     RCC_APB2Periph_GPIOB
  #define TCS_SYNC_PIN                        GPIO_Pin_5
  #define TCS_SYNC_PORT                       GPIOB
  #define TCS_SYNC_RCC                        RCC_APB2Periph_GPIOB
  #define BUTTON_PORT                         GPIOB
  #define BUTTON_PIN                          GPIO_Pin_0
  #define BUTTON_RCC                          RCC_APB2Periph_GPIOB
#endif /* USE_AVAGO */

#elif defined (USE_LATENCY_TESTER)

  #define USB_DISCONNECT                      GPIOA
  #define USB_DISCONNECT_PIN                  GPIO_Pin_10
  #define RCC_APB2Periph_GPIO_DISCONNECT      RCC_APB2Periph_GPIOA

  #define TCS_I2C                             I2C1
  #define TCS_I2C_RCC                         RCC_APB1Periph_I2C1
  #define TCS_SCL_PIN                         GPIO_Pin_6
  #define TCS_SDA_PIN                         GPIO_Pin_7
  #define TCS_SCL_SDA_PORT                    GPIOB
  #define TCS_SCL_SDA_RCC                     RCC_APB2Periph_GPIOB
  #define TCS_SYNC_PIN                        GPIO_Pin_5
  #define TCS_SYNC_PORT                       GPIOB
  #define TCS_SYNC_RCC                        RCC_APB2Periph_GPIOB

  #define BUTTON_PORT                         GPIOA
  #define BUTTON_PIN                          GPIO_Pin_8
  #define BUTTON_RCC                          RCC_APB2Periph_GPIOA

  #define FACTORY_PIN                         GPIO_Pin_9
  #define FACTORY_PORT                        GPIOB
  #define FACTORY_RCC                         RCC_APB2Periph_GPIOB

#ifdef USE_7_SEG
  #define TLC_SPI                             SPI1
  #define TLC_SPI_RCC                         RCC_APB2Periph_SPI1
  #define TLC_SPI_PRESCALER                   SPI_BaudRatePrescaler_4
  #define TLC_SS_PIN                          GPIO_Pin_4
  #define TLC_SS_PORT                         GPIOA
  #define TLC_SS_RCC                          RCC_APB2Periph_GPIOA
  #define TLC_SCK_PIN                         GPIO_Pin_5
  #define TLC_MISO_PIN                        GPIO_Pin_6
  #define TLC_MOSI_PIN                        GPIO_Pin_7
  #define TLC_SCK_MISO_MOSI_PORT              GPIOA
  #define TLC_SCK_MISO_MOSI_RCC               RCC_APB2Periph_GPIOA
  #define TLC_OE_PIN                          GPIO_Pin_0
  #define TLC_OE_PORT                         GPIOB
  #define TLC_OE_RCC                          RCC_APB2Periph_GPIOB

  #define DIGIT1_PIN                          GPIO_Pin_12
  #define DIGIT1_PORT                         GPIOB
  #define DIGIT1_RCC                          RCC_APB2Periph_GPIOB
  #define COLON_PIN                           GPIO_Pin_13
  #define COLON_PORT                          GPIOB
  #define COLON_RCC                           RCC_APB2Periph_GPIOB
  #define DIGIT2_PIN                          GPIO_Pin_14
  #define DIGIT2_PORT                         GPIOB
  #define DIGIT2_RCC                          RCC_APB2Periph_GPIOB
  #define DIGIT3_PIN                          GPIO_Pin_15
  #define DIGIT3_PORT                         GPIOB
  #define DIGIT3_RCC                          RCC_APB2Periph_GPIOB
#endif /* USE_7_SEG */

#endif /* USE_TRACKER_V1 */

#endif /* _PLATFORM_CONFIG_H_ */
