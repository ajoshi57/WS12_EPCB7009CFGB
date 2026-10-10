/****************************************************
  FILE NAME     : f02-cntrtask.c
  ORIGINATOR    : A Joshi
  LAST EDITED   : 26 SEPT 2026
  PROJECT       : WS12_EPCB7009CFGB
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/

#include "f02-cntrtask.h"
//
#define DEBUG 1
//
/////////////////////////////////////////////////////////
//tasks
xTaskHandle g_cntrtask;
QueueHandle_t QueueEXTIntrHandle;
//
BaseType_t create_cntrtask(void);
void StartTaskCntr(void *arg);
//
int create_tripcntr_queue(void);



/////////////////////////////////////////////////////////
BaseType_t create_cntrtask(void)
{
    BaseType_t ret;

    ret = xTaskCreate(StartTaskCntr, (const portCHAR *)"cntrtask",
            (256), NULL,
            tskIDLE_PRIORITY, &g_cntrtask);
    return ret;
}


void StartTaskCntr(void *arg)
{
    int flg;
    int exerr = 0;
    BaseType_t status;
    //
    static uint32_t postcnt = 0;
    uint8_t inputno;
    //
    //================================================================================
    TRIPCNTRS_T *tripcntrs = (TRIPCNTRS_T *) pvPortMalloc(sizeof(TRIPCNTRS_T));
    if(tripcntrs == NULL)
    {
        debug_msg("\r\nFAIL Alloc Mem - 'TRIPCNTRS_T'\r\n");
        while (1)
            ;   //stuck here
    }
    //================================================================================
    ALARMENTRY_T *alarmentry = (ALARMENTRY_T *) pvPortMalloc(sizeof(ALARMENTRY_T));
    if(alarmentry == NULL)
    {
        debug_msg("\r\nFAIL Alloc Mem - 'ALARMENTRY_T'\r\n");
        while (1)
            ;   //stuck here
    }
    //================================================================================
    init_tripcntrs_mutex();
    //
    //================================================================================
    while(1)
    {
        //processing loop - wait for trip post
        //loop++;
        //debug_msg((char *)"LOOP cnt %d\r\n", loop);
        ////////////////////////////////////////////////////////////////////////////////
        exerr = 0; //find out if executed successfully
        status = xQueueReceive(QueueEXTIntrHandle, (void* )&inputno, portMAX_DELAY);
        if(status == pdTRUE)
        {
            //incr trip cntr
             postcnt++;
 #if DEBUG
             debug_msg("RX TRIP Post [%d] Input [%d]\r\n", postcnt, inputno);
 #endif
             ////////////////////////////////////////////////////////////////////////////////////////
             memset(alarmentry,0,sizeof(ALARMENTRY_T));
             flg = incr_tripcntrs_mutex( inputno, alarmentry);
             if(flg == 0)
             {
                 post_FetchAlarmsQueue(1, alarmentry);
             }
             ////////////////////////////////////////////////////////////////////////////////////////
             copy_tripcntrs_mutex(tripcntrs);
#if DEBUG
             debug_msg("TRIP COUNT INPUTS[1-4]   : %02d %02d %02d %02d  \r\n",
                       tripcntrs->inputcnt[0], tripcntrs->inputcnt[1],
                       tripcntrs->inputcnt[2], tripcntrs->inputcnt[3]);
             debug_msg("TRIP COUNT OUTPUTS[1-4]  : %02d %02d %02d %02d  \r\n",
                       tripcntrs->outputcnt[0], tripcntrs->outputcnt[1],
                       tripcntrs->outputcnt[2], tripcntrs->outputcnt[3]);
             debug_msg("TRIP COUNT OUTPUTS[5-8]  : %02d %02d %02d %02d  \r\n",
                       tripcntrs->outputcnt[4], tripcntrs->outputcnt[5],
                       tripcntrs->outputcnt[6], tripcntrs->outputcnt[7]);
             debug_msg("TRIP COUNT OUTPUTS[9-12] : %02d %02d %02d %02d  \r\n",
                       tripcntrs->outputcnt[8], tripcntrs->outputcnt[9],
                       tripcntrs->outputcnt[10], tripcntrs->outputcnt[11]);
             debug_msg("TRIP COUNT OUTPUTS[13-16]: %02d %02d %02d %02d  \r\n",
                       tripcntrs->outputcnt[12], tripcntrs->outputcnt[13],
                       tripcntrs->outputcnt[14], tripcntrs->outputcnt[15]);

#endif
        }
        //
        ///////////////////////////////////////////////////////////////////////
        //// any other tasks maybe done here....periodically
        //
    }
}

int create_tripcntr_queue(void)
{
    int ret = 0;
    //////////////////////////////////////////////////////////////////////////////////////////
    /* creation of QueueEXTIntr */
    // Create a queue that holds uint8_t items
    QueueEXTIntrHandle = xQueueCreate(64, sizeof(uint8_t));
    //////////////////////////////////////////////////////////////////////////////////////////
    if(QueueEXTIntrHandle == NULL)
    {
        ret = -1;
    }
    return ret;
    //
}
