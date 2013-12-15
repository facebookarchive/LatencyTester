/**
  ******************************************************************************
  * @file    usb_desc.h
  * @author  MCD Application Team
  * @version V3.4.0
  * @date    29-June-2012
  * @brief   Descriptor Header for Latency Tester
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_DESC_H
#define __USB_DESC_H
#include "platform_config.h"

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define USB_DEVICE_DESCRIPTOR_TYPE          0x01
#define USB_CONFIGURATION_DESCRIPTOR_TYPE   0x02
#define USB_STRING_DESCRIPTOR_TYPE          0x03
#define USB_INTERFACE_DESCRIPTOR_TYPE       0x04
#define USB_ENDPOINT_DESCRIPTOR_TYPE        0x05

#define HID_DESCRIPTOR_TYPE                 0x21
#define LATENCY_SIZ_HID_DESC                  0x09
#define LATENCY_OFF_HID_DESC                  0x12

#define LATENCY_SIZ_DEVICE_DESC                18
#define LATENCY_SIZ_CONFIG_DESC                34
#define LATENCY_SIZ_REPORT_DESC                142
#define LATENCY_SIZ_STRING_LANGID              4
#define LATENCY_SIZ_STRING_VENDOR              32
#define LATENCY_SIZ_STRING_PRODUCT             30
#define LATENCY_SIZ_STRING_SERIAL              26

extern const uint8_t Latency_DeviceDescriptor[LATENCY_SIZ_DEVICE_DESC];
extern const uint8_t Latency_ConfigDescriptor[LATENCY_SIZ_CONFIG_DESC];
extern const uint8_t Latency_ReportDescriptor[LATENCY_SIZ_REPORT_DESC];
extern const uint8_t Latency_StringLangId[LATENCY_SIZ_STRING_LANGID];
extern const uint8_t Latency_StringVendor[LATENCY_SIZ_STRING_VENDOR];
extern const uint8_t Latency_StringProduct[LATENCY_SIZ_STRING_PRODUCT];
extern  uint8_t Latency_StringSerial[LATENCY_SIZ_STRING_SERIAL];

#define bMaxPacketSize0             0x40     /* bMaxPacketSize0 = 64 bytes   */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/* External variables --------------------------------------------------------*/

#endif /* __USB_DESC_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

