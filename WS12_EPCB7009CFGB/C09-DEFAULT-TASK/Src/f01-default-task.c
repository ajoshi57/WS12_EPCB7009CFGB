/****************************************************
  FILE NAME     : f01-default-task.c
  ORIGINATOR    : A Joshi
  LAST EDITED   : 04 MAY 2026
  PROJECT       : WS12_EPCB7011
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/

#include "f01-default-task.h"
/////////////////////////////////////////////////////////
//tasks
xTaskHandle g_default_task;
/////////////////////////////////////////////////////////
BaseType_t create_default_task(void)
{
    BaseType_t ret;

    ret = xTaskCreate(default_task, (const portCHAR *)"default_task",
            (256), NULL,
            tskIDLE_PRIORITY, &g_default_task);
    return ret;
}

void default_task(void *arg)
{
    int32_t freebytes;
    uint8_t loop = 0;
    //uint8_t nsetdef;

    while(1)
    {
        if(loop == 8)
        {
            freebytes = xPortGetFreeHeapSize();
            debug_msg("FH %d !\r\n",freebytes);
            loop = 0;
        }
        vTaskDelay(500);
        //StrobePulse();
        loop++;
        //
        ///////////////////////////////////////////////////////////////////////
        //// any other tasks maybe done here....periodically
        //
    }
}


