/**
  ******************************************************************************
  * @file    hw_config.c
  * @author  MCD Application Team
  * @version V3.4.0
  * @date    29-June-2012
  * @brief   Hardware Configuration & Setup
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
#include "usb_desc.h"
#include "usb_pwr.h"
#include "stm32f10x_dbgmcu.h"
#include "stm32f10x_adc.h"
#include <stdbool.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
ErrorStatus HSEStartUpStatus;
/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void GPIO_Configuration(void);
static void ADC_Configuration(void);
/* Private functions ---------------------------------------------------------*/

void Configure_Debug(void)
{
  // Shut off JTAG since we only use SWD, so we can re-use the pins
  // This needs to happen shortly after reset
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
  DBGMCU_Config(DBGMCU_SLEEP, ENABLE);
}

/*******************************************************************************
* Function Name  : Set_System.
* Description    : Configures Main system clocks & power.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Set_System(void)
{
  /*!< At this stage the microcontroller clock setting is already configured,
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f10x_xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f10x.c file
     */

  /* Enable the PWR clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

  /* Configure the used GPIOs*/
  GPIO_Configuration();

  ADC_Configuration();
}

/*******************************************************************************
* Function Name  : Set_USBClock.
* Description    : Configures USB Clock input (48MHz).
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Set_USBClock(void)
{
#if defined(STM32L1XX_MD) || defined(STM32L1XX_HD)|| defined(STM32L1XX_MD_PLUS)
  /* Enable USB clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);

#elif defined(STM32F10X_CL)
  /* Select USBCLK source */
  RCC_OTGFSCLKConfig(RCC_OTGFSCLKSource_PLLVCO_Div3);

  /* Enable the USB clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_OTG_FS, ENABLE) ;

#else
#ifdef USE_F102
  RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_Div1);
#else /* USE_F102 */
  RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);
#endif /* USE_F102 */

  /* Enable the USB clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);
#endif /* STM32F10X_CL */
}

void Enter_Stop(void)
{
    // Going into stop mode works, but the USB wakeup interrupt never fires.
    // Instead, just shut off the external resonator and PLL and go to
    // 8 MHz internal RC.  Gets us down to ~1.7 mA.
//    DBGMCU_Config(DBGMCU_SLEEP | DBGMCU_STANDBY | DBGMCU_STOP, ENABLE);
//    PWR_EnterSTOPMode(PWR_Regulator_ON, PWR_STOPEntry_WFI);
//
//    RCC_HSICmd(ENABLE);
//
//    /* Wait till HSI is ready */
//    while (RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET) {}
//
//    /* Select HSI as system clock source */
//    RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
//
//    /* Disable PLL1 */
//    RCC_PLLCmd(DISABLE);
//
//    /* Disable HSE */
//    RCC_HSEConfig(RCC_HSE_OFF);
//
//    // Shut off core clocks
//    __WFI();
}

// Restart clocks
void Wake_From_Stop(void)
{
//    /* Wait till HSE is ready */
//    HSEStartUpStatus = RCC_WaitForHSEStartUp();
//
//    /* Enable HSE */
//    RCC_HSEConfig(RCC_HSE_ON);
//
//    /* Wait till HSE is ready */
//    while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET) {}
//
//    /* Enable PLL1 */
//    RCC_PLLCmd(ENABLE);
//
//    /* Wait till PLL1 is ready */
//    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) {}
//
//    /* Select PLL as system clock source */
//    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
//
//    /* Wait till PLL is used as system clock source */
//    while (RCC_GetSYSCLKSource() != RCC_CFGR_SWS_PLL) {}
}

/*******************************************************************************
* Function Name  : Enter_LowPowerMode.
* Description    : Power-off system clocks and power while entering suspend mode.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Enter_LowPowerMode(void)
{
  /* Set the device state to suspend */
  bDeviceState = SUSPENDED;
  Enter_Stop();
}

/*******************************************************************************
* Function Name  : Leave_LowPowerMode.
* Description    : Restores system clocks and power while exiting suspend mode.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Leave_LowPowerMode(void)
{
  DEVICE_INFO *pInfo = &Device_Info;

  Wake_From_Stop();

  /* Set the device state to the correct state */
  if (pInfo->Current_Configuration != 0)
  {
    /* Device configured */
    bDeviceState = CONFIGURED;
  }
  else
  {
    bDeviceState = ATTACHED;
  }
}

/*******************************************************************************
* Function Name  : USB_Cable_Config.
* Description    : Software Connection/Disconnection of USB Cable.
* Input          : NewState: new state.
* Output         : None.
* Return         : None
*******************************************************************************/
void USB_Cable_Config (FunctionalState NewState)
{
#ifdef USB_DISCONNECT_PIN
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin = USB_DISCONNECT_PIN;

  if (NewState == DISABLE)
  {
    // Go high-Z to disconnect the usb pullup resistor
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(USB_DISCONNECT, &GPIO_InitStructure);
  }
  else
  {
    // pull up the pullup resistor
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIO_DISCONNECT, ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(USB_DISCONNECT, &GPIO_InitStructure);
    GPIO_SetBits(USB_DISCONNECT, USB_DISCONNECT_PIN);
  }
#endif /* USB_DISCONNECT_PIN */
}

/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  RCC_APB2PeriphClockCmd(BUTTON_RCC, ENABLE);
  GPIO_InitStructure.GPIO_Pin = BUTTON_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
  GPIO_Init(BUTTON_PORT, &GPIO_InitStructure);

  RCC_APB2PeriphClockCmd(FACTORY_RCC, ENABLE);
  GPIO_InitStructure.GPIO_Pin = FACTORY_PIN;
  GPIO_Init(FACTORY_PORT, &GPIO_InitStructure);

#ifdef USE_AVAGO
  RCC_APB2PeriphClockCmd(RED_RCC | GREEN_RCC, ENABLE);

  GPIO_InitStructure.GPIO_Pin = RED_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(RED_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GREEN_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GREEN_PORT, &GPIO_InitStructure);
#endif /* USE_AVAGO */
}

static void ADC_Configuration(void)
{
#ifdef USE_AVAGO
  ADC_InitTypeDef ADC_InitStructure;

  /* ADCCLK = PCLK2/8 */
  RCC_ADCCLKConfig(RCC_PCLK2_Div8);

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);

  /* ADC1 configuration ------------------------------------------------------*/
  ADC_InitStructure.ADC_Mode = ADC_Mode_RegSimult;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 1;
  ADC_Init(ADC1, &ADC_InitStructure);

  /* ADC2 configuration ------------------------------------------------------*/
  ADC_InitStructure.ADC_Mode = ADC_Mode_RegSimult;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 1;
  ADC_Init(ADC2, &ADC_InitStructure);

  ADC_RegularChannelConfig(ADC1, RED_CHANNEL, 1, ADC_SampleTime_239Cycles5);
  ADC_RegularChannelConfig(ADC2, GREEN_CHANNEL, 1, ADC_SampleTime_239Cycles5);

  ADC_Cmd(ADC1, ENABLE);
  ADC_Cmd(ADC2, ENABLE);

  ADC_ResetCalibration(ADC1);
  while(ADC_GetResetCalibrationStatus(ADC1));
  ADC_StartCalibration(ADC1);
  while(ADC_GetCalibrationStatus(ADC1));

  ADC_ResetCalibration(ADC2);
  while(ADC_GetResetCalibrationStatus(ADC2));
  ADC_StartCalibration(ADC2);
  while(ADC_GetCalibrationStatus(ADC2));

  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
  ADC_SoftwareStartConvCmd(ADC2, ENABLE);
#endif /* USE_AVAGO */
}

/*******************************************************************************
* Function Name  : USB_Interrupts_Config.
* Description    : Configures the USB interrupts.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void USB_Interrupts_Config(void)
{
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Configure the EXTI line 18 connected internally to the USB IP */
  EXTI_ClearITPendingBit(EXTI_Line18);
  EXTI_InitStructure.EXTI_Line = EXTI_Line18; // USB resume from suspend mode
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* 2 bit for pre-emption priority, 2 bits for subpriority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

  /* Enable the USB interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Enable the USB Wake-up interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USBWakeUp_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
* Function Name  : Reset_Device.
* Description    : Reset the device.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Reset_Device(void)
{
  USB_Cable_Config(DISABLE);
  NVIC_SystemReset();
}

static void IntToBase32(uint32_t value, char *buf, uint8_t len, bool unicode)
{
    uint8_t mult = unicode ? 2 : 1;

    for (uint8_t i = 0; i < len; i++) {
        // Select 5 bits
        uint8_t bits = value & 0x1F;

        if (bits < 0xA) {
            buf[i*mult] = bits + '0';
        } else {
            buf[i*mult] = bits + 'A' - 10;
        }

        if (unicode) buf[i*mult + 1] = 0;
        value = value >> 5;
    }
}

/*******************************************************************************
* Function Name  : Get_SerialNum.
* Description    : Create the serial number string descriptor.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Get_SerialNum(void)
{
    uint32_t Device_Serial0, Device_Serial1;

#if defined(STM32L1XX_MD) || defined(STM32L1XX_HD)|| defined(STM32L1XX_MD_PLUS)
    uint32_t serial0 = *(uint32_t*)(0x1FF80050);
    uint32_t serial1 = *(uint32_t*)(0x1FF80054);
    uint32_t serial2 = *(uint32_t*)(0x1FF80064);
#else
    uint32_t serial0 = *(volatile uint32_t*)(0x1FFFF7E8);
    uint32_t serial1 = *(volatile uint32_t*)(0x1FFFF7EC);
    uint32_t serial2 = *(volatile uint32_t*)(0x1FFFF7F0);
#endif /* STM32L1XX_XD */

    // Try to preserve some uniqueness by going to 60 bits
    // Most of the change appears to be happening in the final 32 bits
    Device_Serial0 = serial2 ^ ((serial2 & 0xC000) >> 2);
    Device_Serial1 = serial1 ^ serial0;
    Device_Serial1 = Device_Serial1 ^ ((Device_Serial1 & 0xC000) >> 2);

    IntToBase32(Device_Serial0, (char *)&Latency_StringSerial[2], 6, 1);
    IntToBase32(Device_Serial1, (char *)&Latency_StringSerial[14], 6, 1);
}
