/****************************************************
  FILE NAME     : f01-default-task.h
  ORIGINATOR    : A Joshi
  LAST EDITED   : 04 MAY 2026
  PROJECT       : WS12_EPCB7011
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/

#ifndef C09_DEFAULT_TASK_INC_F01_DEFAULT_TASK_H_
#define C09_DEFAULT_TASK_INC_F01_DEFAULT_TASK_H_

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
#include "f01-pinouts.h"
#include "f01-console.h"
#include "f01-defines.h"
//
BaseType_t create_default_task(void);
void default_task(void *arg);




#endif /* C09_DEFAULT_TASK_INC_F01_DEFAULT_TASK_H_ */
