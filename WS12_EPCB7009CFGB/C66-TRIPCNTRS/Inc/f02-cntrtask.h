/****************************************************
  FILE NAME     : f02-cntrtask.h
  ORIGINATOR    : A Joshi
  LAST EDITED   : 26 SEPT 2026
  PROJECT       : WS12_EPCB7009CFGB
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/

#ifndef C66_TRIPCNTRS_INC_F02_CNTRTASK_H_
#define C66_TRIPCNTRS_INC_F02_CNTRTASK_H_

/* Standard includes. */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/* Hardware includes. */
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
#include "drivers/rtos_hw_drivers.h"
#include "utils/uartstdio.h"
//

/*-----------------------------------------------------------*/
#include "f01-pinouts.h"
#include "f01-console.h"
#include "f01-defines.h"
#include "f01-tripcntrs.h"
#include "f03-fetchalarm-task.h"
//
BaseType_t create_cntrtask(void);
void StartTaskCntr(void *arg);
//
int create_tripcntr_queue(void);



#endif /* C66_TRIPCNTRS_INC_F02_CNTRTASK_H_ */
