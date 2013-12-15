/*****************************************************************************

Filename    :   tick.c
Content     :   Millisecond timer
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

#include "tick.h"
#include "misc.h"

volatile static uint32_t counter = 0;

void tick_init(void)
{
    // CALIB is set for 1ms when using 9 MHz SysTick clock, so adjust it for
    // a 6 MHz clock
    uint32_t calib = (((SysTick->CALIB & SysTick_CALIB_TENMS_Msk) * 2)/3);
    SysTick->LOAD = calib & SysTick_CALIB_TENMS_Msk;
    // Enable the interrupt at low priority
    NVIC_SetPriority(SysTick_IRQn, (1 << __NVIC_PRIO_BITS) - 1);
    // Set the starting value at the top of the countdown
    SysTick->VAL = SysTick->LOAD - 1;
    // Turn it on with the external (9 MHz) clock and interrupts on
    SysTick->CTRL = SysTick_CTRL_TICKINT_Msk |
                    SysTick_CTRL_ENABLE_Msk;
}

void tick_deinit(void)
{
    SysTick->CTRL = 0;
    counter = 0;
}

uint32_t tick_get(void)
{
    return counter;
}

void tick_update(void)
{
    counter++;
}
