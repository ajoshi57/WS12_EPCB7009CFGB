/****************************************************
  FILE NAME		: f02-u0inthandler.c
  ORIGINATOR	: A Joshi
  LAST EDITED   : 18 OCT 2025
  PROJECT       : WS12_EPCB7011
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/

#ifndef INC_F02_U0INTHANDLER_H_
#define INC_F02_U0INTHANDLER_H_

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include <string.h>
//

//#include "u0ringbuffer.h"
#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "f01-u0msgtask.h"

extern QueueHandle_t QueueU0CharsHandle;

void Post_Uart0_char(int c);
void UART0IntHandler(void);

#endif /* INC_F02_U0INTHANDLER_H_ */
