/**
  ******************************************************************************
  * @file    usb_desc.c
  * @author  MCD Application Team
  * @version V3.4.0
  * @date    29-June-2012
  * @brief   Descriptors for Latency Tester
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


/* Includes ------------------------------------------------------------------*/
#include "usb_desc.h"
#include "platform_config.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
const uint8_t Latency_DeviceDescriptor[LATENCY_SIZ_DEVICE_DESC] =
  {
    0x12,   /* bLength */
    USB_DEVICE_DESCRIPTOR_TYPE,   /* bDescriptorType */
    0x00,   /* bcdUSB, version 2.00 */
    0x02,
    0x00,   /* bDeviceClass : See interface */
    0x00,   /* bDeviceSubClass : See interface*/
    0x00,   /* bDeviceProtocol : See interface */
    bMaxPacketSize0, /* bMaxPacketSize0 0x40 = 64 */
    // Oculus VR VID/PID
    0x33,   /* idVendor     (0x2833) */
    0x28,
    0x01,   /* idProduct (0x0101) Latency Tester PID*/
    0x01,

    0x05,   /* bcdDevice 0.5 */
    0x00,

    0x01,   /* iManufacturer : index of string Manufacturer  */
    0x02,   /* iProduct      : index of string descriptor of product*/
    0x03,   /* iSerialNumber : index of string serial number*/

    0x01    /*bNumConfigurations */
  };

const uint8_t Latency_ConfigDescriptor[LATENCY_SIZ_CONFIG_DESC] =
  {
    0x09,   /* bLength: Configuration Descriptor size */
    USB_CONFIGURATION_DESCRIPTOR_TYPE,   /* bDescriptorType: Configuration */
    LATENCY_SIZ_CONFIG_DESC, /* wTotalLength: Bytes returned */
    0x00,
    0x01,   /* bNumInterfaces: 1 interface */
    0x01,   /* bConfigurationValue: Configuration value */
    0x00,   /* iConfiguration: Index of string descriptor */
    0x80,   /* bmAttributes: bus powered */
    0x64,   /* MaxPower 200 mA */
    /* 09 */

    /************** Descriptor of Latency HID interface ****************/
    /* 09 */
    0x09,         /* bLength: Interface Descriptor size */
    USB_INTERFACE_DESCRIPTOR_TYPE,/* bDescriptorType: Interface descriptor type */
    0x00,         /* bInterfaceNumber: Number of Interface */
    0x00,         /* bAlternateSetting: Alternate setting */
    0x01,         /* bNumEndpoints */
    0x03,         /* bInterfaceClass: HID */
    0x00,         /* bInterfaceSubClass : 1=BOOT, 0=no boot */
    0x00,         /* nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse */
    0,            /* iInterface: Index of string descriptor */
    /******************** Descriptor of Latency HID ********************/
    /* 18 */
    0x09,         /* bLength: HID Descriptor size */
    HID_DESCRIPTOR_TYPE, /* bDescriptorType: HID */
    0x10,         /* bcdHID: HID Class Spec release number */
    0x01,
    0x00,         /* bCountryCode: Hardware target country */
    0x01,         /* bNumDescriptors: Number of HID class descriptors to follow */
    0x22,         /* bDescriptorType */
    LATENCY_SIZ_REPORT_DESC,/* wItemLength: Total length of Report descriptor */
    0x00,
    /******************** Descriptor of Latency HID endpoints ******************/
    /* 27 */
    0x07,          /* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESCRIPTOR_TYPE, /* bDescriptorType: */

    0x81,          /* bEndpointAddress: Endpoint Address (IN) */
    0x03,          /* bmAttributes: Interrupt endpoint */
    0x40,          /* wMaxPacketSize: 64 Bytes max */
    0x00,
    0x01,          /* bInterval: Polling Interval (1 ms) */
    /* 34 */
  };

const uint8_t Latency_ReportDescriptor[LATENCY_SIZ_REPORT_DESC] =
  {
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x00,                    // USAGE (Undefined) - Required before Collection
    0xa1, 0x01,                    // COLLECTION (Application)

    // Samples
    0x85, 0x01,                    //   REPORT_ID (1)
    0x09, 0x3b,                    //   USAGE (Byte Count)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x95, 0x3F,                    //   REPORT_COUNT (63)
    0x81, 0x03,                    //   INPUT (Cnst,Var,Abs)

    // Triggered
    0x85, 0x02,                    //   REPORT_ID (2)
    0x09, 0x3b,                    //   USAGE (Byte Count)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x95, 0x0C,                    //   REPORT_COUNT (12)
    0x81, 0x03,                    //   INPUT (Cnst,Var,Abs)

    // Start
    0x85, 0x03,                    //   REPORT_ID (3)
    0x09, 0x3b,                    //   USAGE (Byte Count)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x95, 0x07,                    //   REPORT_COUNT (7)
    0x81, 0x03,                    //   INPUT (Cnst,Var,Abs)

    // Button
    0x85, 0x04,                    //   REPORT_ID (4)
    0x09, 0x3b,                    //   USAGE (Byte Count)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x95, 0x04,                    //   REPORT_COUNT (4)
    0x81, 0x03,                    //   INPUT (Cnst,Var,Abs)

    // Configuration
    0x85, 0x05,                    //   REPORT_ID (5)
    0x09, 0x3b,                    //   USAGE (Byte Count)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x95, 0x04,                    //   REPORT_COUNT (4)
    0xb1, 0x03,                    //   FEATURE (Cnst,Var,Abs)

    // Bootload
    0x85, 0x06,                    //   REPORT_ID (6)
    0x09, 0x3b,                    //   USAGE (Byte Count)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x95, 0x03,                    //   REPORT_COUNT (3)
    0xb1, 0x03,                    //   FEATURE (Cnst,Var,Abs)

    // Calibrate
    0x85, 0x07,                    //   REPORT_ID (7)
    0x09, 0x3b,                    //   USAGE (Byte Count)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x95, 0x03,                    //   REPORT_COUNT (3)
    0xb1, 0x03,                    //   FEATURE (Cnst,Var,Abs)

    // Trigger
    0x85, 0x08,                    //   REPORT_ID (8)
    0x09, 0x3b,                    //   USAGE (Byte Count)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x95, 0x05,                    //   REPORT_COUNT (5)
    0xb1, 0x03,                    //   FEATURE (Cnst,Var,Abs)

    // Display
    0x85, 0x09,                    //   REPORT_ID (9)
    0x09, 0x3b,                    //   USAGE (Byte Count)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x95, 0x05,                    //   REPORT_COUNT (5)
    0xb1, 0x03,                    //   FEATURE (Cnst,Var,Abs)

    0xc0                          //   END_COLLECTION
  };

const uint8_t Latency_StringLangId[LATENCY_SIZ_STRING_LANGID] =
  {
    LATENCY_SIZ_STRING_LANGID,
    USB_STRING_DESCRIPTOR_TYPE,
    0x09,
    0x04    /* LangID = 0x0409: U.S. English */
  };

const uint8_t Latency_StringVendor[LATENCY_SIZ_STRING_VENDOR] =
  {
    LATENCY_SIZ_STRING_VENDOR,
    USB_STRING_DESCRIPTOR_TYPE,
    /* Manufacturer: "Oculus VR, Inc." */
    'O', 0, 'c', 0, 'u', 0, 'l', 0, 'u', 0, 's', 0, ' ', 0, 'V', 0,
    'R', 0, ',', 0, ' ', 0, 'I', 0, 'n', 0, 'c', 0, '.', 0
  };

const uint8_t Latency_StringProduct[LATENCY_SIZ_STRING_PRODUCT] =
  {
    LATENCY_SIZ_STRING_PRODUCT,
    USB_STRING_DESCRIPTOR_TYPE,
    /* Product: "Latency Tester" */
    'L', 0, 'a', 0, 't', 0, 'e', 0, 'n', 0, 'c', 0, 'y', 0, ' ', 0, 'T', 0,
    'e', 0, 's', 0, 't', 0, 'e', 0, 'r', 0
  };

uint8_t Latency_StringSerial[LATENCY_SIZ_STRING_SERIAL] =
  {
    LATENCY_SIZ_STRING_SERIAL,
    USB_STRING_DESCRIPTOR_TYPE,
    /* Serial number */
#if defined (USE_STM32L152_EVAL) || defined (USE_STM32L152D_EVAL)
    'S', 0, 'T', 0, 'M', 0, '3', 0, '2', 0, 'L', 0, '1', 0
#else
    'S', 0, 'T', 0, 'M', 0, '3', 0, '2', 0, '1', 0, '0', 0
#endif /* USE_STM32L152_EVAL */
  };

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
