/****************************************************
  FILE NAME		: f01-pinouts.h
  ORIGINATOR	: A Joshi
  LAST EDITED   : 18 OCT 2025
  PROJECT       : WS12_EPCB7011
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/

#ifndef INC_F01_PINOUTS_H_
#define INC_F01_PINOUTS_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Hardware includes. */
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_ints.h"
//
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "drivers/rtos_hw_drivers.h"
#include "drivers/pinout.h"
#include "driverlib/ssi.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"

/*-----------------------------------------------------------*/


//Fn protos defined here...
void PinoutSet_spi3_flash(void);
void PinoutSet_i2c1_logic(void);
void PinoutSet_UART0Console( void );
void PinoutSet_epcb7011_gpios(void);
void PinoutSet_i2c0_twi0(void);

#endif /* INC_F01_WATCHDOG_H_ */
