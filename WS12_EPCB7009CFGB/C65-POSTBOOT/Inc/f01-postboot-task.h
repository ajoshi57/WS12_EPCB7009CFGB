/****************************************************
  FILE NAME     : f01-postboot-task.h
  ORIGINATOR    : A Joshi
  LAST EDITED   : 18 OCT 2025
  PROJECT       : WS12_EPCB7011
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/

#ifndef C65_POSTBOOT_INC_F01_POSTBOOT_TASK_H_
#define C65_POSTBOOT_INC_F01_POSTBOOT_TASK_H_


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
#include "f01-gpio.h"
#include "f04-sem-bootup.h"

//fns defined here...
void StartTaskPostBoot(void *argument);


#endif /* C65_POSTBOOT_INC_F01_POSTBOOT_TASK_H_ */
