/****************************************************
  FILE NAME		: f01-i2cmast.c
  ORIGINATOR	: A Joshi
  LAST EDITED   : 18 OCT 2025
  PROJECT       : WS12_EPCB7011
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/

#include "f01-i2cmast.h"
//
#define DEBUG 1
//
/* The system clock frequency. */
extern uint32_t g_ui32SysClock;

//
//fns protos defined here....
void InitI2C1(void);
void I2C1Send(uint8_t num_of_args, ...);
void I2C1SendArray(uint8_t array[]);
uint32_t I2C1Receive(uint8_t reg);
//
//
//fns defined here....
//initialize I2C module 1
//Slightly modified version of TI's example code
void InitI2C1(void)
{   
    //enable GPIO peripheral that contains I2C1
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
 
    // Configure the pin muxing for I2C1 functions on port G0 and G1.
    GPIOPinConfigure(GPIO_PG0_I2C1SCL);
    GPIOPinConfigure(GPIO_PG1_I2C1SDA);
     
    // Select the I2C function for these pins.
    GPIOPinTypeI2CSCL(GPIO_PORTG_BASE, GPIO_PIN_0);
    GPIOPinTypeI2C(GPIO_PORTG_BASE, GPIO_PIN_1);
    //
    // Stop the Clock, Reset and Enable I2C Module
    // in Master Function    //disable I2C module 1
    SysCtlPeripheralDisable(SYSCTL_PERIPH_I2C1); 
    //reset module
    SysCtlPeripheralReset(SYSCTL_PERIPH_I2C1);
    //enable I2C module 1
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C1);
    //
    // Wait for the Peripheral to be ready for programming
    //
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_I2C1));
    //
    // Enable and initialize the I2C1 master module.  Use the system clock for
    // the I2C1 module.  The last parameter sets the I2C data transfer rate.
    // If false the data rate is set to 100kbps and if true the data rate will
    // be set to 400kbps.
    I2CMasterInitExpClk(I2C1_BASE, g_ui32SysClock, false);
     
    //clear I2C FIFOs
    HWREG(I2C1_BASE + I2C_O_FIFOCTL) = 80008000;
}


//sends an I2C1 command to the specified slave
void I2C1Send(uint8_t num_of_args, ...)
{
    uint8_t num;
    uint8_t i;
    // Tell the master module what address it will place on the bus when
    // communicating with the slave.
    I2CMasterSlaveAddrSet(I2C1_BASE, SLAVE_ADDRESS_EXT, false);
     
    //stores list of variable number of arguments
    va_list vargs;
     
    //specifies the va_list to "open" and the last fixed argument
    //so vargs knows where to start looking
    va_start(vargs, num_of_args);
     
    //put data to be sent into FIFO
    I2CMasterDataPut(I2C1_BASE, va_arg(vargs, uint32_t));
     
    //if there is only one argument, we only need to use the
    //single send I2C function
    num = num_of_args;
    if(num_of_args == 1)
    {
        //Initiate send of data from the MCU
        I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_SINGLE_SEND);
         
        // Wait until MCU is done transferring.
        while(I2CMasterBusy(I2C1_BASE));
         
        //"close" variable argument list
        va_end(vargs);
    }
     
    //otherwise, we start transmission of multiple bytes on the
    //I2C bus
    else
    {
        //Initiate send of data from the MCU
        I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_START);
         
        // Wait until MCU is done transferring.
        while(I2CMasterBusy(I2C1_BASE));
         
        //send num_of_args-2 pieces of data, using the
        //BURST_SEND_CONT command of the I2C module
        for(i = 1; i < (num - 1); i++)
        {
            //put next piece of data into I2C FIFO
            I2CMasterDataPut(I2C1_BASE, va_arg(vargs, uint32_t));
            //send next data that was just placed into FIFO
            I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_CONT);
     
            // Wait until MCU is done transferring.
            while(I2CMasterBusy(I2C1_BASE));
        }
     
        //put last piece of data into I2C FIFO
        I2CMasterDataPut(I2C1_BASE, va_arg(vargs, uint32_t));
        //send next data that was just placed into FIFO
        I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
        // Wait until MCU is done transferring.
        while(I2CMasterBusy(I2C1_BASE));
         
        //"close" variable args list
        va_end(vargs);
    }
}


//sends an array of data via I2C1 to the specified slave
void I2C1SendArray(uint8_t array[])
{
    // Tell the master module what address it will place on the bus when
    // communicating with the slave.
    I2CMasterSlaveAddrSet(I2C1_BASE, SLAVE_ADDRESS_EXT, false);
     
    //put data to be sent into FIFO
    I2CMasterDataPut(I2C1_BASE, array[0]);
     
    //if there is only one argument, we only need to use the
    //single send I2C function
    if(array[1] == '\0')
    {
        //Initiate send of data from the MCU
        I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_SINGLE_SEND);
         
        // Wait until MCU is done transferring.
        while(I2CMasterBusy(I2C1_BASE));
    }
     
    //otherwise, we start transmission of multiple bytes on the
    //I2C bus
    else
    {
        //Initiate send of data from the MCU
        I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_START);
         
        // Wait until MCU is done transferring.
        while(I2CMasterBusy(I2C1_BASE));
         
        //initialize index into array
        uint8_t i = 1;
 
        //send num_of_args-2 pieces of data, using the
        //BURST_SEND_CONT command of the I2C module
        while(array[i + 1] != '\0')
        {
            //put next piece of data into I2C FIFO
            I2CMasterDataPut(I2C1_BASE, array[i++]);
 
            //send next data that was just placed into FIFO
            I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_CONT);
     
            // Wait until MCU is done transferring.
            while(I2CMasterBusy(I2C1_BASE));
        }
     
        //put last piece of data into I2C1 FIFO
        I2CMasterDataPut(I2C1_BASE, array[i]);
 
        //send next data that was just placed into FIFO
        I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
 
        // Wait until MCU is done transferring.
        while(I2CMasterBusy(I2C1_BASE));
    }
}

//read specified register on slave device
uint32_t I2C1Receive(uint8_t reg)
{
    //specify that we are writing (a register address) to the
    //slave device
    I2CMasterSlaveAddrSet(I2C1_BASE, SLAVE_ADDRESS_EXT, false);
 
    //specify register to be read
    I2CMasterDataPut(I2C1_BASE, reg);
 
    //send control byte and register address byte to slave device
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_START);
     
    //wait for MCU to finish transaction
    while(I2CMasterBusy(I2C1_BASE));
     
    //specify that we are going to read from slave device
    I2CMasterSlaveAddrSet(I2C1_BASE, SLAVE_ADDRESS_EXT, true);
     
    //send control byte and read from the register we
    //specified
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);
     
    //wait for MCU to finish transaction
    while(I2CMasterBusy(I2C1_BASE));
     
    //return data pulled from the specified register
    return I2CMasterDataGet(I2C1_BASE);
}


