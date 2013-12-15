/**
  ******************************************************************************
  * @file    usb_prop.c
  * @author  MCD Application Team
  * @version V3.4.0
  * @date    29-June-2012
  * @brief   All USBprocessings related to Latency Tester
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

#include "hw_config.h"
#include "usb_lib.h"
#include "usb_conf.h"
#include "usb_prop.h"
#include "usb_desc.h"
#include "usb_pwr.h"
#include "feature_reports.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
typedef enum _HID_REPORTS
{
  HID_INPUT   = 1,
  HID_OUTPUT  = 2,
  HID_FEATURE = 3
} HID_REPORTS;
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t ProtocolValue;
uint8_t Request = 0;

DEVICE Device_Table =
  {
    EP_NUM,
    1
  };

DEVICE_PROP Device_Property =
  {
    Latency_init,
    Latency_Reset,
    Latency_Status_In,
    Latency_Status_Out,
    Latency_Data_Setup,
    Latency_NoData_Setup,
    Latency_Get_Interface_Setting,
    Latency_GetDeviceDescriptor,
    Latency_GetConfigDescriptor,
    Latency_GetStringDescriptor,
    0,                    /*DFU_EP0Buffer*/
    bMaxPacketSize0       /*Max Packet size*/
  };

USER_STANDARD_REQUESTS User_Standard_Requests =
  {
    Latency_GetConfiguration,
    Latency_SetConfiguration,
    Latency_GetInterface,
    Latency_SetInterface,
    Latency_GetStatus,
    Latency_ClearFeature,
    Latency_SetEndPointFeature,
    Latency_SetDeviceFeature,
    Latency_SetDeviceAddress
  };

ONE_DESCRIPTOR Device_Descriptor =
  {
    (uint8_t*)Latency_DeviceDescriptor,
    LATENCY_SIZ_DEVICE_DESC
  };

ONE_DESCRIPTOR Config_Descriptor =
  {
    (uint8_t*)Latency_ConfigDescriptor,
    LATENCY_SIZ_CONFIG_DESC
  };

ONE_DESCRIPTOR Latency_Report_Descriptor =
  {
    (uint8_t *)Latency_ReportDescriptor,
    LATENCY_SIZ_REPORT_DESC
  };

ONE_DESCRIPTOR Latency_Hid_Descriptor =
  {
    (uint8_t*)Latency_ConfigDescriptor + LATENCY_OFF_HID_DESC,
    LATENCY_SIZ_HID_DESC
  };

ONE_DESCRIPTOR Latency_String_Descriptor[4] =
  {
    {(uint8_t*)Latency_StringLangId,          LATENCY_SIZ_STRING_LANGID       },
    {(uint8_t*)Latency_StringVendor,          LATENCY_SIZ_STRING_VENDOR       },
    {(uint8_t*)Latency_StringProduct,         LATENCY_SIZ_STRING_PRODUCT      },
    {(uint8_t*)Latency_StringSerial,          LATENCY_SIZ_STRING_SERIAL       },
  };

/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : Latency_init.
* Description    : Latency init routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Latency_init(void)
{
  /* Update the serial number string descriptor with the data from the unique ID*/
  Get_SerialNum();

  pInformation->Current_Configuration = 0;
  /* Connect the device */
  PowerOn();

  /* Perform basic device initialization operations */
  USB_SIL_Init();

  bDeviceState = UNCONNECTED;
}

/*******************************************************************************
* Function Name  : Latency_Reset.
* Description    : Latency reset routine
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Latency_Reset(void)
{
  /* Set DFU_DEVICE as not configured */
  pInformation->Current_Configuration = 0;
  pInformation->Current_Interface = 0;/*the default Interface*/

  /* Current Feature initialization */
  pInformation->Current_Feature = Latency_ConfigDescriptor[7];

#ifdef STM32F10X_CL
  /* EP0 is already configured in DFU_Init by OTG_DEV_Init() function
      No Other endpoints needed for this firmware */
#else
  SetBTABLE(BTABLE_ADDRESS);

  /* Initialize Endpoint 0 */
  SetEPType(ENDP0, EP_CONTROL);
  SetEPTxStatus(ENDP0, EP_TX_NAK);
  SetEPRxAddr(ENDP0, ENDP0_RXADDR);
  SetEPTxAddr(ENDP0, ENDP0_TXADDR);
  Clear_Status_Out(ENDP0);
  SetEPRxCount(ENDP0, Device_Property.MaxPacketSize);
  SetEPTxCount(ENDP0, Device_Property.MaxPacketSize);
  SetEPRxValid(ENDP0);

  /* Initialize Endpoint 1 */
  SetEPType(ENDP1, EP_INTERRUPT);
  SetEPTxAddr(ENDP1, ENDP1_TXADDR);
  SetEPRxAddr(ENDP1, ENDP1_RXADDR);
  SetEPTxCount(ENDP1, 64);
  SetEPRxCount(ENDP1, 64);
  SetEPRxStatus(ENDP1, EP_RX_NAK);
  SetEPTxStatus(ENDP1, EP_TX_NAK);

  /* Set this device to response on default address */
  SetDeviceAddress(0);
#endif /* STM32F10X_CL */

  /* Set the new control state of the device to Attached */
  bDeviceState = ATTACHED;
}

/*******************************************************************************
* Function Name  : Latency_SetConfiguration.
* Description    : Update the device state to configured.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Latency_SetConfiguration(void)
{
  if (pInformation->Current_Configuration != 0)
  {
    /* Device configured */
    bDeviceState = CONFIGURED;
  }
}

/*******************************************************************************
* Function Name  : Latency_SetDeviceAddress.
* Description    : Update the device state to addressed.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Latency_SetDeviceAddress (void)
{
  bDeviceState = ADDRESSED;
}

/*******************************************************************************
* Function Name  : Latency_Status_In.
* Description    : Latency status IN routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Latency_Status_In(void)
{
  if (Request == SET_REPORT) {
    Request = 0;
    // Buffer has been filled, so parse it
    feature_reports_parse_report();
  }
}

/*******************************************************************************
* Function Name  : Latency_Status_Out.
* Description    : Latency status OUT routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Latency_Status_Out (void)
{
}

/*******************************************************************************
* Function Name  : Latency_Data_Setup.
* Description    : Handle the data class specific requests.
* Input          : RequestNb.
* Output         : None.
* Return         : USB_SUCCESS or USB_UNSUPPORT.
*******************************************************************************/
RESULT Latency_Data_Setup(uint8_t RequestNo)
{
  uint8_t *(*CopyRoutine)(uint16_t);

  CopyRoutine = NULL;

  if ((RequestNo == GET_DESCRIPTOR)
      && (Type_Recipient == (STANDARD_REQUEST | INTERFACE_RECIPIENT))
      && (pInformation->USBwIndex0 == 0))
  {

    if (pInformation->USBwValue1 == REPORT_DESCRIPTOR)
    {
      CopyRoutine = Latency_GetReportDescriptor;
    }
    else if (pInformation->USBwValue1 == HID_DESCRIPTOR_TYPE)
    {
      CopyRoutine = Latency_GetHIDDescriptor;
    }

  } /* End of GET_DESCRIPTOR */

  /*** GET_PROTOCOL ***/
  else if (Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT))
  {
    switch (RequestNo) {
      case GET_PROTOCOL:
        CopyRoutine = Latency_GetProtocolValue;
        break;

      case GET_REPORT:
        // We only handle feature reports here
        if (pInformation->USBwValue1 == HID_FEATURE)
          CopyRoutine = feature_reports_get_report;
        break;

      case SET_REPORT:
        if (pInformation->USBwValue1 == HID_FEATURE) {
          Request = SET_REPORT;
          CopyRoutine = feature_reports_set_report;
        }
        break;

      default:
        break;
    }
  }

  if (CopyRoutine == NULL)
  {
    return USB_UNSUPPORT;
  }

  pInformation->Ctrl_Info.CopyData = CopyRoutine;
  pInformation->Ctrl_Info.Usb_wOffset = 0;
  (*CopyRoutine)(0);
  return USB_SUCCESS;
}

/*******************************************************************************
* Function Name  : Latency_NoData_Setup.
* Description    : Handle the No data class specific requests.
* Input          : Request Nb.
* Output         : None.
* Return         : USB_SUCCESS or USB_UNSUPPORT.
*******************************************************************************/
RESULT Latency_NoData_Setup(uint8_t RequestNo)
{
  if ((Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT))
      && (RequestNo == SET_PROTOCOL)) {
    return Latency_SetProtocol();
  }

  return USB_UNSUPPORT;
}

/*******************************************************************************
* Function Name  : Latency_GetDeviceDescriptor.
* Description    : Gets the device descriptor.
* Input          : Length.
* Output         : None.
* Return         : The address of the device descriptor.
*******************************************************************************/
uint8_t *Latency_GetDeviceDescriptor(uint16_t Length)
{
  return Standard_GetDescriptorData(Length, &Device_Descriptor);
}

/*******************************************************************************
* Function Name  : Latency_GetConfigDescriptor.
* Description    : Gets the configuration descriptor.
* Input          : Length.
* Output         : None.
* Return         : The address of the configuration descriptor.
*******************************************************************************/
uint8_t *Latency_GetConfigDescriptor(uint16_t Length)
{
  return Standard_GetDescriptorData (Length, &Config_Descriptor);
}

/*******************************************************************************
* Function Name  : Latency_GetStringDescriptor.
* Description    : Gets the string descriptors according to the needed index.
* Input          : Length.
* Output         : None.
* Return         : The address of the string descriptors.
*******************************************************************************/
uint8_t *Latency_GetStringDescriptor(uint16_t Length)
{
  uint8_t wValue0 = pInformation->USBwValue0;
  // There are only 4 strings defined, so return error if asked for higher
  if (wValue0 >= 4)
  {
    return NULL;
  }
  else
  {
    return Standard_GetDescriptorData(Length, &Latency_String_Descriptor[wValue0]);
  }
}

/*******************************************************************************
* Function Name  : Latency_GetReportDescriptor.
* Description    : Gets the HID report descriptor.
* Input          : Length
* Output         : None.
* Return         : The address of the configuration descriptor.
*******************************************************************************/
uint8_t *Latency_GetReportDescriptor(uint16_t Length)
{
  return Standard_GetDescriptorData(Length, &Latency_Report_Descriptor);
}

/*******************************************************************************
* Function Name  : Latency_GetHIDDescriptor.
* Description    : Gets the HID descriptor.
* Input          : Length
* Output         : None.
* Return         : The address of the configuration descriptor.
*******************************************************************************/
uint8_t *Latency_GetHIDDescriptor(uint16_t Length)
{
  return Standard_GetDescriptorData(Length, &Latency_Hid_Descriptor);
}

/*******************************************************************************
* Function Name  : Latency_Get_Interface_Setting.
* Description    : tests the interface and the alternate setting according to the
*                  supported one.
* Input          : - Interface : interface number.
*                  - AlternateSetting : Alternate Setting number.
* Output         : None.
* Return         : USB_SUCCESS or USB_UNSUPPORT.
*******************************************************************************/
RESULT Latency_Get_Interface_Setting(uint8_t Interface, uint8_t AlternateSetting)
{
  if (AlternateSetting > 0)
  {
    return USB_UNSUPPORT;
  }
  else if (Interface > 0)
  {
    return USB_UNSUPPORT;
  }

  return USB_SUCCESS;
}

/*******************************************************************************
* Function Name  : Latency_SetProtocol
* Description    : Joystick Set Protocol request routine.
* Input          : None.
* Output         : None.
* Return         : USB SUCCESS.
*******************************************************************************/
RESULT Latency_SetProtocol(void)
{
  uint8_t wValue0 = pInformation->USBwValue0;
  ProtocolValue = wValue0;
  return USB_SUCCESS;
}

/*******************************************************************************
* Function Name  : Latency_GetProtocolValue
* Description    : get the protocol value
* Input          : Length.
* Output         : None.
* Return         : address of the protocol value.
*******************************************************************************/
uint8_t *Latency_GetProtocolValue(uint16_t Length)
{
  if (Length == 0)
  {
    pInformation->Ctrl_Info.Usb_wLength = 1;
    return NULL;
  }
  else
  {
    return (uint8_t *)(&ProtocolValue);
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
