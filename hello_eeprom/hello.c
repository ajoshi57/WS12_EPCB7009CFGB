//*****************************************************************************
//
// hello.c - Simple hello world example.
//
// Copyright (c) 2013-2020 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 2.2.0.295 of the EK-TM4C1294XL Firmware Package.
//
//*****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "drivers/pinout.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "driverlib/eeprom.h"


//*****************************************************************************
//
//! \addtogroup example_list
//! <h1>Hello World (hello)</h1>
//!
//! A very simple ``hello world'' example.  It simply displays ``Hello World!''
//! on the UART and is a starting point for more complicated applications.
//!
//! Open a terminal with 115,200 8-N-1 to see the output for this demo.
//
//*****************************************************************************

//*****************************************************************************
//
// System clock rate in Hz.
//
//*****************************************************************************
uint32_t g_ui32SysClock;

//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif

//*****************************************************************************
//
// Configure the UART and its pins.  This must be called before UARTprintf().
//
//*****************************************************************************
void
ConfigureUART(void)
{
    //
    // Enable the GPIO Peripheral used by the UART.
    //
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //
    // Enable UART0
    //
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    //
    // Configure GPIO Pins for UART mode.
    //
    MAP_GPIOPinConfigure(GPIO_PA0_U0RX);
    MAP_GPIOPinConfigure(GPIO_PA1_U0TX);
    MAP_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Initialize the UART for console I/O.
    //
    UARTStdioConfig(0, 115200, g_ui32SysClock);
}

#define E2PROM_TEST_ADRES 0x0000

struct E2PROM
{
    uint8_t value1;
    uint8_t value2;
    uint16_t value3;
    uint8_t value4[12];
};



struct E2PROM e2prom_write_value = {5,7,9826, "Hello World"}; /* Write struct */
struct E2PROM e2prom_read_value =  {0,0,0,""}; /* Read struct */



//*****************************************************************************
//
// Print "Hello World!" to the UART on the Intelligent UART Module.
//
//*****************************************************************************
int
main(void)
{
    uint32_t e2size,e2block,returnCode;
   //
    // Run from the PLL at 120 MHz.
    // Note: SYSCTL_CFG_VCO_240 is a new setting provided in TivaWare 2.2.x and
    // later to better reflect the actual VCO speed due to SYSCTL#22.
    //
    g_ui32SysClock = MAP_SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                                             SYSCTL_OSC_MAIN |
                                             SYSCTL_USE_PLL |
                                             SYSCTL_CFG_VCO_240), 120000000);

    //
    // Configure the device pins.
    //
    PinoutSet(false, false);

    //
    // Enable the GPIO pins for the LED D1 (PN1).
    //
    MAP_GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_1);

    //
    // Initialize the UART.
    //
    ConfigureUART();

    //
    // Hello!
    //
    UARTprintf("Hello, world!\n");
    //
    /*******************************/

    /* EEPROM SETTINGS */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_EEPROM0); // EEPROM activate

    do
    {
        returnCode=EEPROMInit(); // EEPROM start
    }
    while(returnCode!=EEPROM_INIT_OK);


    /*******************************/

    UARTprintf("EEPROM Test Program\r\n");


    e2size = EEPROMSizeGet(); // Get EEPROM Size
    UARTprintf("EEPROM Size %d bytes\n", e2size);

    e2block = EEPROMBlockCountGet(); // Get EEPROM Block Count
    UARTprintf("EEPROM Blok Count: %d\n", e2block);
    //
    //UARTprintf("Write Try > Address %u: Struct : {%u,%u,%u,%s}\n", E2PROM_TEST_ADRES, e2prom_write_value.value1, e2prom_write_value.value2, e2prom_write_value.value3, e2prom_write_value.value4);
     //EEPROMProgram((uint32_t *)&e2prom_write_value, E2PROM_TEST_ADRES, sizeof(e2prom_write_value)); //Write struct to EEPROM start from 0x0000
     EEPROMRead((uint32_t *)&e2prom_read_value, E2PROM_TEST_ADRES, sizeof(e2prom_read_value)); //Read from struct at EEPROM start from 0x0000
     UARTprintf("Read Try > Address %u: Struct : {%u,%u,%u,%s}\n", E2PROM_TEST_ADRES, e2prom_read_value.value1, e2prom_read_value.value2, e2prom_read_value.value3, e2prom_read_value.value4);



    //
    // We are finished.  Hang around flashing D1.
    //
    while(1)
    {
        //
        // Turn on D1.
        //
        LEDWrite(CLP_D1, 1);

        //
        // Delay for a bit.
        //
        SysCtlDelay(g_ui32SysClock / 10 / 3);

        //
        // Turn off D1.
        //
        LEDWrite(CLP_D1, 0);

        //
        // Delay for a bit.
        //
        SysCtlDelay(g_ui32SysClock / 10 / 3);
        //
        EEPROMRead((uint32_t *)&e2prom_read_value, E2PROM_TEST_ADRES, sizeof(e2prom_read_value)); //Read from struct at EEPROM start from 0x0000
        UARTprintf("Read Try > Address %u: Struct : {%u,%u,%u,%s}\n", E2PROM_TEST_ADRES, e2prom_read_value.value1, e2prom_read_value.value2, e2prom_read_value.value3, e2prom_read_value.value4);
        SysCtlDelay(g_ui32SysClock / 100 / 3);
    }
}
