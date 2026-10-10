/****************************************************
  FILE NAME     : f03-fetchalarm-task.c
  ORIGINATOR    : A Joshi
  LAST EDITED   : 28 SEPT 2026
  PROJECT       : WS12_EPCB7009CFGB
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/

#include "f03-fetchalarm-task.h"

//
#define DEBUGFETCH 1
//
/////////////////////////////////////////////////////////
//tasks
xTaskHandle g_fetchalarmtask;
QueueHandle_t QueueFetchAlarmHandle;
//
//////////////////////////////////////////////////////////////////////
//Current Alarms mutex
static SemaphoreHandle_t g_mutex_access_fetchalarms;
static FETCHALARMS_T g_FETCH_ALARMS;
//////////////////////////////////////////////////////////////////////

//fx protos defined here...
int create_mutex_access_fetchalarms(void);
int flush_fetchalarms_mutex(void);
int fetchalarms_available_check_mutex(uint8_t *avail);
int store_fetchalarms_mutex(ALARMENTRY_T *fetchalarms);
int extract_fetchalarms_mutex(ALARMENTRY_T *fetchalarms);
//
BaseType_t post_FetchAlarmsQueue(uint8_t allow, ALARMENTRY_T *entry);
char *get_alarmeventstr(ALARMENTRY_T *entry, char *msg, int msg_len);
//
BaseType_t create_fetchalarmtask(void);
void StartTaskFetchAlarm(void *arg);
int create_fetchalarm_queue(void);
//

//fxs defined here...
int create_mutex_access_fetchalarms(void)
{
    int ret = 0;
    ///////////////////////////////////////////////////////////////////////
    g_mutex_access_fetchalarms = xSemaphoreCreateMutex();
    if( g_mutex_access_fetchalarms == NULL )
    {
        debug_msg("ERROR create, 'g_mutex_access_fetchalarms' !\r\n");
        ret = -1;
    }
    else
    {
        //The semaphore can now be used.
        debug_msg("SUCCESS create, 'g_mutex_access_fetchalarms' !\r\n");
    }
    ///////////////////////////////////////////////////////////////////////
    ////////////////////
    return ret;
    ////////////////////
    //
}

int flush_fetchalarms_mutex(void)
{
    int ret = 0;
    //
    if( xSemaphoreTake( g_mutex_access_fetchalarms, ( TickType_t ) 100 ) == pdTRUE )
    {
        //FLUSH Alarms from globals
        memset(&g_FETCH_ALARMS,0,sizeof(FETCHALARMS_T));
        xSemaphoreGive( g_mutex_access_fetchalarms);
    }
    else
    {
        debug_msg("ERROR !, 'g_mutex_access_fetchalarms', mutex not obtained !\r\n");
        ret = -1;
    }
    return ret;
}

/* checks if the new data is available in the incoming buffer
 */
int fetchalarms_available_check_mutex(uint8_t *avail)
{
    int ret = 0;
    uint8_t tavail;
    if( xSemaphoreTake( g_mutex_access_fetchalarms, ( TickType_t ) 100 ) == pdTRUE )
    {
        //FLUSH Alarms from globals
        tavail = (uint8_t)(MAX_PENDING_ALARMS + g_FETCH_ALARMS.headidx - g_FETCH_ALARMS.tailidx) % MAX_PENDING_ALARMS;
        *avail = tavail;
        xSemaphoreGive( g_mutex_access_fetchalarms);
    }
    else
    {
        debug_msg("ERROR !, 'g_mutex_access_fetchalarms', mutex not obtained !\r\n");
        ret = -1;
    }
    return ret;
}

int store_fetchalarms_mutex(ALARMENTRY_T *fetchalarms)
{
    int ret = 0;
    uint8_t newheadidx;
    //
    if( xSemaphoreTake( g_mutex_access_fetchalarms, ( TickType_t ) 100 ) == pdTRUE )
    {
        //Store Alarm to global circular buffer
        //as long as headidx would not advance to tailidx (overflow)
        newheadidx = (uint8_t)(g_FETCH_ALARMS.headidx + 1) % MAX_PENDING_ALARMS;
        if(newheadidx != g_FETCH_ALARMS.tailidx)
        {
            memcpy(&g_FETCH_ALARMS.entry[g_FETCH_ALARMS.headidx],fetchalarms,sizeof(ALARMENTRY_T));
            g_FETCH_ALARMS.headidx = newheadidx;
        }
        else
        {
            debug_msg("ERROR FULL ! Overflow, store_fetchalarms !\r\n");
        }
        xSemaphoreGive( g_mutex_access_fetchalarms);
    }
    else
    {
        debug_msg("ERROR ! 'g_mutex_access_fetchalarms', mutex not obtained !\r\n");
        ret = -1;
    }
    return ret;

}



int extract_fetchalarms_mutex(ALARMENTRY_T *fetchalarms)
{
    int ret = 0;
    //
    if( xSemaphoreTake( g_mutex_access_fetchalarms, ( TickType_t ) 100 ) == pdTRUE )
    {
        //Extract Alarm from global circular buffer
        // if the head isn't ahead of the tail, we don't have any characters
        if(g_FETCH_ALARMS.headidx == g_FETCH_ALARMS.tailidx)
        {
            debug_msg("ERROR EMPTY ! underflow, extract_fetchalarms_mutex !\r\n");
            ret = -1;
        }
        else
        {
            memcpy(fetchalarms, &g_FETCH_ALARMS.entry[g_FETCH_ALARMS.tailidx], sizeof(ALARMENTRY_T));
            //new tail idx
            g_FETCH_ALARMS.tailidx = (uint8_t)(g_FETCH_ALARMS.tailidx + 1) % MAX_PENDING_ALARMS;
        }
        xSemaphoreGive( g_mutex_access_fetchalarms);
    }
    else
    {
        debug_msg("ERROR set, 'g_mutex_access_fetchalarms', mutex not obtained !\r\n");
        ret = -1;
    }
    return ret;
}


BaseType_t post_FetchAlarmsQueue(uint8_t allow, ALARMENTRY_T *entry)
{
    BaseType_t status = pdTRUE;
    if(allow)
    {
        status = xQueueSend(QueueFetchAlarmHandle, entry, portMAX_DELAY  );
        if(status != pdTRUE)
        {
            debug_msg("FAIL: 'post_FetchAlarmsQueue' !\r\n");
        }
    }
    return status;
}


char *get_alarmeventstr(ALARMENTRY_T *entry, char *msg, int msg_len)
{
    uint8_t errflg = 0;
    char str1[100];
    char str2[50];
    int j;
    int first = 0;
    //
    memset(msg,0,msg_len);
    //
#if DEBUGFETCH
    debug_msg("inputno %d tripcnt %d\r\n",
              entry->curinputno, entry->tripcnt);
#endif
    /////////////////////////////////////////////////////////////////////////////////
    //determine mapped outputs
    memset(str1,0,sizeof(str1));
    memset(str2,0,sizeof(str2));
    sprintf(str1,"Mapped Outputs: [");
    for(j=0;j<MAX_OUTCHANS;j++)
    {
        if(entry->isoutputmapped[j])
        {
            if(first == 0)
            {
                sprintf(str2,"%d", j + 1);
                first = 1;
            }
            else
            {
                sprintf(str2," ,%d", j + 1);
            }
            strcat(str1,str2);
        }
    }
    memset(str2,0,sizeof(str2));
    if(first == 0)
    {
        sprintf(str2,"None]\r\n");
    }
    else
    {
        sprintf(str2,"]\r\n");
    }
    strcat(str1,str2);
    /////////////////////////////////////////////////////////////////////////////////
    snprintf(msg,msg_len,"TRIP Input:%d, Count:%d, %s", entry->curinputno, entry->tripcnt, str1);
    ///////////////////
    return msg;
    ///////////////////
}


/////////////////////////////////////////////////////////
BaseType_t create_fetchalarmtask(void)
{
    BaseType_t ret;

    ret = xTaskCreate(StartTaskFetchAlarm, (const portCHAR *)"fetchalarmtask",
            (256), NULL,
            tskIDLE_PRIORITY, &g_fetchalarmtask);
    return ret;
}

void StartTaskFetchAlarm(void *argument)
{
    int flg;
    int exerr = 0;
    BaseType_t status;
    static uint32_t loopcnt = 0;
    //
    static uint32_t postcnt = 0;
    //================================================================================
    ALARMENTRY_T *alarmentry = (ALARMENTRY_T *) pvPortMalloc(sizeof(ALARMENTRY_T));
    if(alarmentry == NULL)
    {
        debug_msg("\r\nFAIL Alloc Mem - 'ALARMENTRY_T'\r\n");
        while (1)
            ;   //stuck here
    }
    //================================================================================
    //================================================================================
    //
    //////////////////////////////////////////////////////////////////////
    vTaskDelay(40000); //40 secs
    debug_msg("\r\n TaskFetchAlarm Waiting for BOOTUP !\r\n");
    if(wait_TaskFetchAlarm_for_postboot() != 0)
    {
        stop_watchdog();
        while(1)
        {

        }
    }
    debug_msg("\r\n TaskFetchAlarm BOOTUP wait over !\r\n");
    ////////////////////////////////////////////////////////////////////////
    debug_msg("\r\nStarting TaskFetchAlarm !\r\n");
    flush_fetchalarms_mutex();
    loopcnt = 0;
    ////////////////////////////////////////////////////////////////////////////////
    //
    while(1)
    {
        //processing loop - check if any log is pending..
        iwdg_strobpulse();
        //loop++;
        //debug_msg((char *)"LOOP cnt %d\r\n", loop);
        ////////////////////////////////////////////////////////////////////////////////
        exerr = 0; //find out if executed successfully
        status = xQueueReceive(QueueFetchAlarmHandle, (void* )alarmentry, portMAX_DELAY);
        if(status == pdTRUE)
        {
            //do a log entry
            postcnt++;
#if DEBUG
            debug_msg("RX ALARM Entry [%d] \r\n", postcnt);
#endif
            ///////////////////////////////////////////////////////////////////////////
            store_fetchalarms_mutex(alarmentry);
            ///////////////////////////////////////////////////////////////////////////
            loopcnt++;
            drive_cardalarm(1);
            vTaskDelay(100);
            drive_cardalarm(0);
            debug_msg("INTR AlarmFetch [%d]\r\n", loopcnt);
            ///////////////////////////////////////////////////////////////////////////
            //
        }
        else
        {
            exerr = 1;
        }
        ///////////////////////////////////////////////////////////
        if(exerr == 0)
        {
#if DEBUG
            debug_msg("SUCCESS, Executed... 'StartTaskFetchAlarm'\r\n");
#endif
        }
        else
        {
            debug_msg("FAIL, Executed... 'StartTaskFetchAlarm'\r\n");
        }
        ///////////////////////////////////////////////////////////
        //
    }
}

int create_fetchalarm_queue(void)
{
    int ret = 0;
    //////////////////////////////////////////////////////////////////////////////////////////
    /* creation of QueueFetchAlarm */
    // Create a queue that holds ALARMENTRY_T items
    QueueFetchAlarmHandle = xQueueCreate(64, sizeof(ALARMENTRY_T));
    //////////////////////////////////////////////////////////////////////////////////////////
    if(QueueFetchAlarmHandle == NULL)
    {
        ret = -1;
    }
    return ret;
    //
}

