/****************************************************
  FILE NAME		: f01-console.h
  ORIGINATOR	: A Joshi
  LAST EDITED   : 18 OCT 2025
  PROJECT       : WS12_EPCB7011
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/

#ifndef INC_F01_CONSOLE_H_
#define INC_F01_CONSOLE_H_
//
/* Standard includes. */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* Hardware includes. */
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "drivers/rtos_hw_drivers.h"
#include "utils/uartstdio.h"
/*-----------------------------------------------------------*/



///////////////////////////////////////////////////////////////
//
//fns protos defined here...
int ajCreate_mutex_access_u0(void);
void ajConfigureUART0Console( void );
void ajUART0printf( const char *pcString, ... );
void print(int dest, const char *pcString, ...);
void ajUART0Write( const char *pcString );
void ajClearTerm(int dest);
void ajUART0crlf(void);
////////
//////////////////////////////////////////////////////////////


#endif /* INC_F01_CONSOLE_H_ */
