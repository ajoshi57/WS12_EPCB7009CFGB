/*
 * uart_thread_safe
 *
 * Copyright (C) 2022 Texas Instruments Incorporated
 * 
 * 
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions 
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the   
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
*/

/******************************************************************************
 *
 * This project demonstrates how to use the UART peripheral in a thread safe
 * manner.  A mutex is created and used to guard the UART output function until
 * each string is fully sent.  One UART output function uses a basic UART
 * string output with no special character handling.  The other uses the
 * TivaWare UART standard IO module to output a message including a timestamp.
 *
 * main() creates one task.  It then starts the scheduler.
 *
 * vUARTTask handles the configuration of the UART peripheral and creates the
 * mutex that is used to guard the UART output functions.  If successful, it
 * then creates two tasks used to demonstrate sending UART data in a thread
 * safe manner.
 *
 * Open a terminal with 115,200 8-N-1 to see the output for this demo.
 *
 */

/* Standard includes. */
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

/* Hardware includes. */
#include "inc/hw_memmap.h"
#include "inc/hw_sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/eeprom.h"
#include "drivers/rtos_hw_drivers.h"
/*-----------------------------------------------------------*/
#include "f01-pinouts.h"
#include "f01-console.h"
#include "f01-i2clogic.h"
#include "f01-create-clitasks.h"
#include "f03-create-boottasks.h"
#include "utils/uartstdio.h"
#include "f04-sem-bootup.h"
#include "f01-default-task.h"
#include "f01-i2cmast.h"
#include "f02-uart7.h"
#include "f02-cntrtask.h"
//EPROM on I2C0
#include "f01-twi0mast.h"
#include "f02-ep24aa128.h"

/* The system clock frequency. */
uint32_t g_ui32SysClock;
//
uint32_t get_current_clock(void);

/* Set up the hardware ready to run @120MHZ with PLL using 25MHZ. */
static void prvSetupSystemClock( void );

/* Create UART task for Auto Completion */
extern void ajCreateCLITask( void );

//extern void vUARTTask( void );
/*-----------------------------------------------------------*/

int main( void )
{
    BaseType_t ret;
    int intret;
    //uint32_t e2size,e2block,returnCode;
    /* Prepare the hardware to run this demo. */
    prvSetupSystemClock();

    /* PIOUTS CONFIGURED HERE....*/
    PinoutSet_spi3_flash();
    //PinoutSet_i2c1_logic();...use TWI1 fx
    PinoutSet_UART0Console();
    set_pinout_uart7();
    init_uart7();
    PinoutSet_epcb7011_gpios();
    PinoutSet_i2c0_twi0();
    PinoutSet_i2c1_twi1();
    //init_spi3(); //Flash AT45DB161C
    //
    setup_gpio_defaults();
    /* Configure TWI0 for EPROM 24AA128 */
    init_ep24aa128();


    /* Configure the UART module and output messages to a terminal. */
    //vUARTTask();
    /*******************************/
    InitI2C1();


    /*******************************/
    /* EEPROM SETTINGS */
    init_eeprom();
#if 0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_EEPROM0); // EEPROM activate
    do
    {
        returnCode=EEPROMInit(); // EEPROM start
    }
    while(returnCode!=EEPROM_INIT_OK);
    UARTprintf("EEPROM Test Program\r\n");
    UART7Send("UART7 for Debug\r\n", strlen("UART7 for Debug\r\n"));


    e2size = EEPROMSizeGet(); // Get EEPROM Size
    UARTprintf("EEPROM Size %d bytes\n", e2size);

    e2block = EEPROMBlockCountGet(); // Get EEPROM Block Count
    UARTprintf("EEPROM Block Count: %d\n", e2block);
#endif
    /*******************************/
    debug_msg("UART7 for Debug\r\n");
    //
    ////create mutex access.................
    if(ajCreate_mutex_access_u0() != 0)
    {
      while (1)
      {
      }
    }
    //
    if(create_mutex_access_i2c() != 0)
    {
      while (1)
      {
      }
    }
    //
    if(create_mutex_access_bootstatus() != 0)
    {
        while (1)
        {
        }
    }
    if(create_mutex_access_tripcntrs() != 0)
    {
        while (1)
        {
        }
    }
    //
    //create semaphores.................
    if(create_sem_boot_to_postboot() != 0)
    {
        while (1)
        {
        }
    }
    if(create_sem_postboot_to_TaskCLI() != 0)
    {
        while (1)
        {
        }
    }
    if(create_sem_postboot_to_TaskCntr() != 0)
    {
        while (1)
        {
        }
    }
    if(create_sem_postboot_to_TaskFetchAlarm() != 0)
    {
        while (1)
        {
        }
    }

    /* */
    //ajCreateCLITask();
    ret = ajCreateAutoCompleteCLITaskUART0();
    if(ret != pdTRUE)
    {
        debug_msg("ERROR create, 'CLITaskUART0' !\r\n");
       while (1)
        {
        }
    }
    //
    intret = ajCreateUART0Queue();
    if(intret != 0)
    {
        debug_msg("ERROR create, 'UART0Queue' !\r\n");
       while (1)
        {
        }
    }
    //
    ret = ajCreateBootEPCB7011Task();
    if(ret != pdTRUE)
    {
        debug_msg("ERROR create, 'BootEPCB7011Task' !\r\n");
       while (1)
        {
        }
    }
    ret = ajCreatePostBootEPCB7011Task();
    if(ret != pdTRUE)
    {
        debug_msg("ERROR create, 'PostBootEPCB7011Task' !\r\n");
       while (1)
        {
        }
    }
    if(create_default_task() != pdTRUE)
    {
        debug_msg("ERROR, 'create_default_task' !\r\n");
        while (1)
        {
        }
    }
    ////////////////////////////////////////////////////////////////////////////
    if(create_tripcntr_queue() != 0)
    {
       debug_msg("ERROR, 'create_tripcntr_queue' !\r\n");
       while (1)
       {
       }
    }
    if(create_cntrtask() != pdTRUE)
    {
        debug_msg("ERROR, 'create_cntrtask' !\r\n");
        while (1)
        {
        }
    }
    ////////////////////////////////////////////////////////////////////////////
    if(create_fetchalarm_queue() != 0)
    {
       debug_msg("ERROR, 'create_fetchalarm_queue' !\r\n");
       while (1)
       {
       }
    }
    if(create_fetchalarmtask() != pdTRUE)
    {
        debug_msg("ERROR, 'create_fetchalarmtask' !\r\n");
        while (1)
        {
        }
    }
    ////////////////////////////////////////////////////////////////////////////
    //
    //uint32_t fx = MAP_SysCtlClockGet();
    debug_msg( "Starting EPCB-7009...\r\n ");
    debug_msg( "Sys Clock     = %d\r\n ", g_ui32SysClock);
    //reset_flash(); // **AJ added 12MAR2019
    /* Start the tasks and timer running. */
    vTaskStartScheduler();

    /* If all is well, the scheduler will now be running, and the following
    line will never be reached.  If the following line does execute, then
    there was insufficient FreeRTOS heap memory available for the idle and/or
    timer tasks to be created.  See the memory management section on the
    FreeRTOS web site for more details. */
    for( ;; );
}
/*-----------------------------------------------------------*/

static void prvSetupSystemClock( void )
{
    /* Run from the PLL at configCPU_CLOCK_HZ MHz. */
    g_ui32SysClock = MAP_SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
            SYSCTL_OSC_MAIN | SYSCTL_USE_PLL |
            SYSCTL_CFG_VCO_480), configCPU_CLOCK_HZ);

    /* Configure device pins. */
    PinoutSet(false, false);
}
/*-----------------------------------------------------------*/

void vApplicationMallocFailedHook( void )
{
    /* vApplicationMallocFailedHook() will only be called if
    configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
    function that will get called if a call to pvPortMalloc() fails.
    pvPortMalloc() is called internally by the kernel whenever a task, queue,
    timer or semaphore is created.  It is also called by various parts of the
    demo application.  If heap_1.c or heap_2.c are used, then the size of the
    heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
    FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
    to query the size of free heap space that remains (although it does not
    provide information on how the remaining heap might be fragmented). */
    IntMasterDisable();
    for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
    /* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
    to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
    task.  It is essential that code added to this hook function never attempts
    to block in any way (for example, call xQueueReceive() with a block time
    specified, or call vTaskDelay()).  If the application makes use of the
    vTaskDelete() API function (as this demo application does) then it is also
    important that vApplicationIdleHook() is permitted to return to its calling
    function, because it is the responsibility of the idle task to clean up
    memory allocated by the kernel to any task that has since been deleted. */
    //
    //=============================================================
#if 0
    int32_t freebytes;
    uint8_t loop = 0;
    while(1)
    {
      if(loop == 8)
      {
          freebytes = xPortGetFreeHeapSize();
          debug_msg("\r\nFH %d !\r\n",freebytes);
          loop = 0;
      }
      vTaskDelay(500);
      loop++;
      //
      //iwdg_strobpulse();
    }
#endif
    //=============================================================


}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
    ( void ) pcTaskName;
    ( void ) pxTask;

    /* Run time stack overflow checking is performed if
    configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
    function is called if a stack overflow is detected. */
    debug_msg("Fatal FreeRTOS error - STACK OVERFLOW !\r\nname :");
     debug_msg(pcTaskName);
     debug_msg("\r\n");
    IntMasterDisable();
    for( ;; );
}
/*-----------------------------------------------------------*/
#if 0
void *malloc( size_t xSize )
{
    /* There should not be a heap defined, so trap any attempts to call
    malloc. */
    IntMasterDisable();
    for( ;; );
}
#endif
/*-----------------------------------------------------------*/

void vApplicationTickHook( void )
{
    /* This function will be called by each tick interrupt if
        configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h.  User code can be
        added here, but the tick hook is called from an interrupt context, so
        code must not attempt to block, and only the interrupt safe FreeRTOS API
        functions can be used (those that end in FromISR()). */

    /* Only the full demo uses the tick hook so there is no code is
        executed here. */
}

uint32_t get_current_clock(void)
{
    return g_ui32SysClock;
}

