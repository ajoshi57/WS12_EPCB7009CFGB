/****************************************************
  FILE NAME		: f01-boot-task.h
  ORIGINATOR	: A Joshi
  LAST EDITED   : 18 OCT 2025
  PROJECT       : WS12_EPCB7011
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/

#ifndef INC_F01_BOOT_TASK_H_
#define INC_F01_BOOT_TASK_H_

/* Includes ------------------------------------------------------------------*/
/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"



#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#include "f01-defines.h"
#include "f01-console.h"
#include "f99-struct-bootstatus.h"
#include "f99-struct-configdbase.h"
#include "f04-configdbase.h"
#include "f01-timer0.h"
#include "f01-watchdog.h"
#include "f01-gpio.h"
#include "f02-bootprocedure.h"
#include "f04-sem-bootup.h"
//

//fns defined here...
void StartTaskBoot(void *argument);
int create_mutex_access_bootstatus(void);
int copy_bootstatus_mutex(BOOTSTATUS_T * status);
int copy_bootpass_mutex(uint8_t *pass);
//
char *getbootresultstr(uint8_t result);
char *getbootstepstr(uint8_t step);
//

//
#endif /* INC_F01_BOOT_TASK_H_ */
