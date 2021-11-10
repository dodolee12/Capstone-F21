/* --COPYRIGHT--,BSD
 * Copyright (c) 2017, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
/* Standard Defines */
#include <stdint.h>

/* TI Defines */
#include <ti/sysbios/BIOS.h>
#include <ti/display/Display.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/Power.h>
#include <ti/drivers/Timer.h>
#include <ti/drivers/UART.h>
#include <ti/devices/msp432e4/driverlib/driverlib.h>

/* Local Defines */
#include "Board.h"
#include "project_zero.h"
#include "Profile/data_service.h"
#include "test_task.h"
#include "coordinate_processing_task.h"

/*
 * The following (weak) function definition is needed in applications
 * that do *not* use the NDK TCP/IP stack:
 */
#if defined(__IAR_SYSTEMS_ICC__)
__weak void NDK_hookInit(int32_t id) {}
#elif defined(__GNUC__) && !defined(__ti__)
void __attribute__((weak)) NDK_hookInit(int32_t id) {}
#else
#pragma WEAK (NDK_hookInit)
void NDK_hookInit(int32_t id) {}
#endif

/* Output display handle that will be used to print out all debug/log
 * statements
 */
Display_Handle displayOut;

void test_callback(uint_least8_t index)
{
    /* Toggle an LED */
    GPIO_toggle(Board_LED0);
}

/*
 * This callback is called every 1,000,000 microseconds, or 1 second. Because
 * the LED is toggled each time this function is called, the LED will blink at
 * a rate of once every 2 seconds.
 */
void timerCallback(Timer_Handle myHandle, int_fast16_t status)
{
    GPIO_toggle(Board_PH0);
}


int main(void)
{
    /* Call board initialization functions */
    Power_init();
    GPIO_init();
    PWM_init();
    UART_init();
    Timer_init();

    //setup gpio interrupt
    /* Install Button callback */
    GPIO_setCallback(Board_PQ1, test_callback);

    /* Enable interrupts */
    GPIO_enableInt(Board_PQ1);

    //timer interrupt

    Timer_Handle timer0;
    Timer_Params params;

    Timer_Params_init(&params);
    params.period = 1000000;
    params.periodUnits = Timer_PERIOD_US;
    params.timerMode = Timer_CONTINUOUS_CALLBACK;
    params.timerCallback = timerCallback;

    timer0 = Timer_open(MSP_EXP432E401Y_TIMER3, &params);

    if (timer0 == NULL) {
        /* Failed to initialized timer */
        while (1) {}
    }

    if (Timer_start(timer0) == Timer_STATUS_ERROR) {
        /* Failed to start timer */
        while (1) {}
    }

    /* Open the display for output */
    displayOut = Display_open(Display_Type_UART | Display_Type_HOST, NULL);
    if (displayOut == NULL)
    {
        /* Failed to open display driver */
        while (1);
    }

    /* ProjectZero Task */
    ProjectZero_createTask();

    /* Update SNP Task */
    updateSNP_createTask();

    /* Button Handler Task */
    buttonTask_createTask();

    /* Coordinate Processing Task */
    coordinate_processing_create_task();

    /* Test Task */
    //test_create_task();

    /* Enable interrupts and start SYS/BIOS */
    BIOS_start();

}
