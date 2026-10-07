/****************************************************
  FILE NAME     : f01-postboot-task.c
  ORIGINATOR    : A Joshi
  LAST EDITED   : 18 OCT 2025
  PROJECT       : WS12_EPCB7011
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/



#include "f01-postboot-task.h"

extern TaskHandle_t g_postbootup_task;


#define DEBUG 0


//fns defined here...
void StartTaskPostBoot(void *argument)
{
    //////////////////////////////
    iwdg_strobpulse();
    //////////////////////////////
    //
    //////////////////////////////////////////////////////////////////////
     vTaskDelay(5000); //20 secs
     debug_msg("\r\n 'TaskPostBoot' Waiting for BOOTUP !\r\n");

     if(wait_postboot_for_bootup() != 0)
     {
         stop_watchdog();
         while(1)
         {

         }
     }
     vTaskDelay(2000); //2 secs
     debug_msg("\r\n 'TaskPostBoot' BOOTUP wait over !\r\n");
     ////////////////////////////////////////////////////////////////////////
     vTaskDelay(2000); //2 secs
     debug_msg("Starting POST-BOOT UP !\r\n");
     vTaskDelay(1000); //1 secs
     ////////////////////////////////////////////////////////////////////////
     init_intr_watchdog();
     init_intr_timer0();
     iwdg_strobpulse();
     drive_cardready(1);
     debug_msg("WatchDog Enabled !\r\n");
     //
     give_sem_postboot_to_TaskCLI();
     give_sem_postboot_to_TaskCntr();
     give_sem_postboot_to_TaskFetchAlarm();
     //
     vTaskDelay(1000); //1 secs
     debug_msg("\r\n TaskCLI, TaskCntr, TaskFetchAlarm  Permitted..\r\n");
     vTaskDelay(1000); //1 secs
     drive_cardready(1);
     debug_msg("CARD Ready !\r\n");
     //////////////////////////////////////////////////////////////
     debug_msg("\r\n'postbootup_task' deleting itself...\r\n");
     vTaskDelete(g_postbootup_task);
     //////////////////////////////////////////////////////////////
     //
     ////////////////////////////////////////////////
     //
     while(1)
     {

     }
     //







     ////////////////////////////////////////////////////////////////////////


}
