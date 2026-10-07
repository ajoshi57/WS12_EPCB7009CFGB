/****************************************************
  FILE NAME     : f04-sem-bootup.h
  ORIGINATOR    : A Joshi
  LAST EDITED   : 18 OCT 2025
  PROJECT       : WS12_EPCB7011
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/

#ifndef C60_BOOT_INC_F04_SEM_BOOTUP_H_
#define C60_BOOT_INC_F04_SEM_BOOTUP_H_

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
//
//fn protos defined here...
//-------------------------------------------------------------
int create_sem_boot_to_postboot(void);
int wait_postboot_for_bootup(void);
int give_sem_bootup_to_postboot(void);
//-------------------------------------------------------------
int create_sem_postboot_to_TaskCLI(void);
int wait_TaskCLI_for_postboot(void);
int give_sem_postboot_to_TaskCLI(void);
//-------------------------------------------------------------
int create_sem_postboot_to_TaskCntr(void);
int wait_TaskCntr_for_postboot(void);
int give_sem_postboot_to_TaskCntr(void);
//-------------------------------------------------------------
int create_sem_postboot_to_TaskFetchAlarm(void);
int wait_TaskFetchAlarm_for_postboot(void);
int give_sem_postboot_to_TaskFetchAlarm(void);
//-------------------------------------------------------------




#endif /* C60_BOOT_INC_F04_SEM_BOOTUP_H_ */
