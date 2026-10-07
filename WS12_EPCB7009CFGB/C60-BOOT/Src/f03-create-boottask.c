/****************************************************
  FILE NAME     : f03-create-boottask.c
  ORIGINATOR    : A Joshi
  LAST EDITED   : 18 OCT 2025
  PROJECT       : WS12_EPCB7011
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/


#include "f03-create-boottasks.h"
//
TaskHandle_t g_bootup_task;

//
//fn protos defined here...
BaseType_t ajCreateBootEPCB7011Task( void );
//




//fns defined here...
BaseType_t ajCreateBootEPCB7011Task( void )
{
    BaseType_t ret;
   //////////////////////////////////////////////////////////////////////////////////////////
   /* creation of TaskBoot */
   ret = xTaskCreate(      StartTaskBoot,
                     "TaskBoot",
                     1024,
                     "TM4C129x BootTask ",
                     tskIDLE_PRIORITY + 2,
                     &g_bootup_task );
   //////////////////////////////////////////////////////////////////////////////////////////
   return ret;
   //////////////////////////////////////////////////////////////////////////////////////////
}

