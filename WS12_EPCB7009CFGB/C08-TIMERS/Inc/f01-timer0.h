/****************************************************
  FILE NAME     : f01-timer0.c
  ORIGINATOR    : A Joshi
  LAST EDITED   : 18 OCT 2025
  PROJECT       : WS12_EPCB7011
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/

#ifndef C08_TIMERS_INC_F01_TIMER0_H_
#define C08_TIMERS_INC_F01_TIMER0_H_


//*****************************************************************************
#include <stdint.h>
#include <stdbool.h>

//*****************************************************************************
//
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/watchdog.h"
#include "drivers/buttons.h"
#include "utils/uartstdio.h"
#include "driverlib/timer.h"
#include "f01-defines.h"
#include "f01-console.h"
#include "f02-uart7.h"

void init_intr_timer0(void);
void iwdg_strobpulse(void);
void stop_watchdog(void);




#endif /* C08_TIMERS_INC_F01_TIMER0_H_ */
