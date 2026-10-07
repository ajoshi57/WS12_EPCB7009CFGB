/*
 * f02-uart7.h
 *
 *  Created on: 16-May-2026
 *      Author: Arvind Joshi
 */

#ifndef C99_CONSOLE_INC_F02_UART7_H_
#define C99_CONSOLE_INC_F02_UART7_H_

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
//
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_uart.h"
#include "driverlib/debug.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"

#include "f01-defines.h"
#include "f01-console.h"



//Functions Defined for UART7 Operation
//*******************************************
void set_pinout_uart7(void);
void init_uart7(void);
void UART7Send(char *pui8Buffer, uint32_t ui32Count);
void debug_msg(const char *pcString, ...);
//*******************************************




#endif /* C99_CONSOLE_INC_F02_UART7_H_ */
