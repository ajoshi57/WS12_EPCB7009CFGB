/****************************************************
  FILE NAME		: f01-pinouts.c
  ORIGINATOR	: A Joshi
  LAST EDITED   : 18 OCT 2025
  PROJECT       : WS12_EPCB7011
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/
//
//CUSTOM PINOUT SETTINGS
//
#include "f01-pinouts.h"
//
/* The system clock frequency. */
extern uint32_t g_ui32SysClock;


//Fn protos defined here...
void PinoutSet_spi3_flash(void);
void PinoutSet_i2c1_logic(void);
void PinoutSet_UART0Console( void );
void PinoutSet_epcb7011_gpios(void);
//
void PinoutSet_i2c0_twi0(void);
void PinoutSet_i2c1_twi1(void);


//fns defined here...
void PinoutSet_spi3_flash(void)
{
    //
    // Enable Peripheral Clocks
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    //
    // Configure the GPIO Pin Mux for PF1
	// for SSI3XDAT0,SSI3TX (MOSI3-FLASH)
	MAP_GPIOPinConfigure(GPIO_PF1_SSI3XDAT0);
	MAP_GPIOPinTypeSSI(GPIO_PORTF_BASE, GPIO_PIN_1);
    //
    // Configure the GPIO Pin Mux for PF0
	// for SSI3XDAT1,SSI3RX, (MISO3-FLASH)
	MAP_GPIOPinConfigure(GPIO_PF0_SSI3XDAT1);
	MAP_GPIOPinTypeSSI(GPIO_PORTF_BASE, GPIO_PIN_0);

    //
    // Configure the GPIO Pin Mux for PF3
	// for SSI3CLK (****SCLK3-FLASH)
	MAP_GPIOPinConfigure(GPIO_PF3_SSI3CLK);
	MAP_GPIOPinTypeSSI(GPIO_PORTF_BASE, GPIO_PIN_3);
    //
    //NSS3
    // Configure the GPIO Pin Mux for PF2
	// for GPIO          (****NCSFLASH)
	MAP_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);
    //
    //NCSFALSH
    //
}

void PinoutSet_i2c1_logic(void)
{
    //
    // Enable Peripheral Clocks
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
    //
    // Configure the GPIO Pin Mux for PG0
    // for I2C1SCL
    MAP_GPIOPinConfigure(GPIO_PG0_I2C1SCL);
    MAP_GPIOPinTypeI2CSCL(GPIO_PORTG_BASE, GPIO_PIN_0);

    //
    // Configure the GPIO Pin Mux for PG1
    // for I2C1SDA
    MAP_GPIOPinConfigure(GPIO_PG1_I2C1SDA);
    MAP_GPIOPinTypeI2C(GPIO_PORTG_BASE, GPIO_PIN_1);
    //
}

void PinoutSet_UART0Console( void )
{
    /* Configure UART0 to send messages to terminal. */
    //
    /* Enable GPIO port A which is used for UART0 pins. */
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

   /* Enable UART0 so that we can configure the clock. */
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    //SysCtlPeripheralReset(SYSCTL_PERIPH_UART0);

    /* Configure the pin muxing for UART0 functions on port A0 and A1.
     * This step is not necessary if your part does not support pin muxing. */
    MAP_GPIOPinConfigure(GPIO_PA0_U0RX);
    MAP_GPIOPinConfigure(GPIO_PA1_U0TX);
    MAP_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    /* config */
    UARTStdioConfig(0, 115200, g_ui32SysClock);
    //
    //IntMasterEnable();
    //
    // Enable the UART interrupt.
    //
    IntEnable(INT_UART0);
    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
    //
}

void PinoutSet_epcb7011_gpios(void)
{
    //
    // Enable Peripheral Clocks..FOR used banks
    //
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //ON PORT-A: CARD-READY, CARD-ALARM
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    //
	MAP_GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_4);
	MAP_GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, GPIO_PIN_4); //default HI to disable
    //
	MAP_GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_5);
	MAP_GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5, GPIO_PIN_5); //default HI to disable
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //ON PORT-B: NOAM-COMEN, I2CDEV2
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    //
	MAP_GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_0);
	MAP_GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0, GPIO_PIN_0); //default HI
    //
	MAP_GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_5);
	MAP_GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5, GPIO_PIN_5); //default LO, FPGA DEV2
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //PN PORT-E: SELFTEST-LED, SPARE, NRESETLOGIC, I2CDEV0, I2CDEV1
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    //**AJ SELFTEST-LED
	MAP_GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_1);
	MAP_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, 0); //default LO, selftest LED
    //
	MAP_GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_2);
	MAP_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, GPIO_PIN_2); //default HI, SPare
    //
	MAP_GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_3);
	MAP_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, GPIO_PIN_3); //default HI, reset logic
    //
	MAP_GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_4);
	MAP_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0); //default LO, FPGA-DEV0
    //
	MAP_GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_5);
	MAP_GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5, 0); //default LO, FPGA-DEV1
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //PN PORT-F: NRESETFLASH
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    //
	MAP_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_4);
	MAP_GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_PIN_4); //default HI
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);
	MAP_GPIOPinTypeGPIOInput(GPIO_PORTK_BASE, GPIO_PIN_2);
	MAP_GPIOPinTypeGPIOInput(GPIO_PORTK_BASE, GPIO_PIN_3);//temp input till ready
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //
}

void PinoutSet_i2c0_twi0(void)
{
    //TWI0_sda_releasehi
    GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_3,GPIO_PIN_3);
    MAP_GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_3);
    //
    //TWI0_scl_releasehi
    GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_2,GPIO_PIN_2);
    MAP_GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_2);
    //
}

void PinoutSet_i2c1_twi1(void)
{
    // Enable Peripheral Clocks
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
    //TWI1_sda_releasehi
    GPIOPinWrite(GPIO_PORTG_BASE,GPIO_PIN_1,GPIO_PIN_1);
    MAP_GPIOPinTypeGPIOInput(GPIO_PORTG_BASE, GPIO_PIN_1);
    //
    //TWI1_scl_releasehi
    GPIOPinWrite(GPIO_PORTG_BASE,GPIO_PIN_0,GPIO_PIN_0);
    MAP_GPIOPinTypeGPIOInput(GPIO_PORTG_BASE, GPIO_PIN_0);
    //

}

