/****************************************************
  FILE NAME		: f01-i2cmast.h
  ORIGINATOR	: A Joshi
  LAST EDITED   : 18 OCT 2025
  PROJECT       : WS12_EPCB7011
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/

#ifndef INC_F01_I2CMAST_H_
#define INC_F01_I2CMAST_H_
//
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
//
#include "inc/hw_i2c.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
//
#include "f01-defines.h"
#include "f01-console.h"
//
#define SLAVE_ADDRESS_EXT 0x50

//fns protos defined here....
void InitI2C1(void);
void I2C1Send(uint8_t num_of_args, ...);
void I2C1SendArray(uint8_t array[]);
uint32_t I2C1Receive(uint8_t reg);


#endif /* INC_F01_I2CMAST_H_ */
