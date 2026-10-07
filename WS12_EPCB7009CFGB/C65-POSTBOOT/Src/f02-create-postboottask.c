/****************************************************
  FILE NAME     : f02-create-postboottask.c
  ORIGINATOR    : A Joshi
  LAST EDITED   : 18 OCT 2025
  PROJECT       : WS12_EPCB7011
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/



#include "f02-create-postboottask.h"


TaskHandle_t g_postbootup_task;

//
//fn protos defined here...
BaseType_t ajCreatePostBootEPCB7011Task( void );
//

//fns defined here...
BaseType_t ajCreatePostBootEPCB7011Task( void )
{
    BaseType_t ret;
   //////////////////////////////////////////////////////////////////////////////////////////
   /* creation of TaskBoot */
   ret = xTaskCreate(      StartTaskPostBoot,
                     "PostTaskBoot",
                     1024,
                     "TM4C129x PostBootTask ",
                     tskIDLE_PRIORITY + 2,
                     &g_postbootup_task );
   //////////////////////////////////////////////////////////////////////////////////////////
   return ret;
   //////////////////////////////////////////////////////////////////////////////////////////
}




