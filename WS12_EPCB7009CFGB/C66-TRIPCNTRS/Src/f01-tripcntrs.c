/*
 * f01-tripcntrs.c
 *
 *  Created on: 25-Sep-2026
 *      Author: Arvind Joshi
 */

#define DEBUGLOG 0

#include "f01-tripcntrs.h"



//
//////////////////////////////////////////////////////////////////////
SemaphoreHandle_t g_mutex_access_tripcntrs;
//////////////////////////////////////////////////////////////////////
//GLOBAL log summary
static TRIPCNTRS_T g_TRIPCNTRS;
//
extern OAMCFGDBASE_T g_CFG_DBASE;


//fns protos defined here...
int create_mutex_access_tripcntrs(void);
int init_tripcntrs_mutex(void);
int incr_tripcntrs_mutex(uint8_t inputno);
int copy_tripcntrs_mutex(TRIPCNTRS_T *tripcntrs);
//

//fns defined here...
int create_mutex_access_tripcntrs(void)
{
    g_mutex_access_tripcntrs = xSemaphoreCreateMutex();
    if( g_mutex_access_tripcntrs == NULL )
    {
        debug_msg("ERROR create, 'g_mutex_access_tripcntrs' !\r\n");
        return -1;
    }
    else
    {
        //The semaphore can now be used.
        debug_msg("SUCCESS create, 'g_mutex_access_tripcntrs' !\r\n");
    }
    return 0;
}



int init_tripcntrs_mutex(void)
{
    int ret = 0;
    int j;
    if( xSemaphoreTake( g_mutex_access_tripcntrs, ( TickType_t ) 10 ) == pdTRUE )
    {
        //////////////////////////////////////////////
        for(j=0;j<MAX_INCHANS;j++)
        {
            g_TRIPCNTRS.inputcnt[j] = 0;
        }
        for(j=0;j<MAX_OUTCHANS;j++)
        {
            g_TRIPCNTRS.outputcnt[j] = 0;
        }

        //////////////////////////////////////////////
        xSemaphoreGive( g_mutex_access_tripcntrs);
    }
    else
    {
        debug_msg("ERROR init, 'g_mutex_access_tripcntrs', mutex not obtained !\r\n");
        ret = -1;
    }
    return ret;
}

int incr_tripcntrs_mutex(uint8_t inputno)
{
    int ret = 0;
    int j;

    if( xSemaphoreTake( g_mutex_access_tripcntrs, ( TickType_t ) 10 ) == pdTRUE )
    {
        //////////////////////////////////////////////
        if(inputno < 1 || inputno > MAX_INCHANS)
        {
            debug_msg("ERROR incr, Illegal 'inputno' !\r\n");
            ret = -1;
        }
        else
        {

            void copy_oamcfgdbase_global(uint8_t *data);

            //==========================================================
            //Input Pulses
            g_TRIPCNTRS.inputcnt[inputno - 1]++;
            if(g_TRIPCNTRS.inputcnt[inputno - 1] == 100)
            {
                g_TRIPCNTRS.inputcnt[inputno - 1] = 0;
            }
            //==========================================================
            //Output Pulses
            for(j=0;j<MAX_OUTCHANS;j++)
            {
                if(g_CFG_DBASE.outputsource[j] == inputno)
                {
                    g_TRIPCNTRS.outputcnt[j]++;
                    if(g_TRIPCNTRS.outputcnt[j] == 100)
                    {
                        g_TRIPCNTRS.outputcnt[j] = 0;
                    }
                }
            }
            //==========================================================
        }
        //////////////////////////////////////////////
        xSemaphoreGive( g_mutex_access_tripcntrs);
    }
    else
    {
        debug_msg("ERROR incr, 'g_mutex_access_tripcntrs', mutex not obtained !\r\n");
        ret = -1;
    }
    return ret;
}

int copy_tripcntrs_mutex(TRIPCNTRS_T *tripcntrs)
{
    int ret = 0;
    if( xSemaphoreTake( g_mutex_access_tripcntrs, ( TickType_t ) 10 ) == pdTRUE )
    {
        //////////////////////////////////////////////
        memcpy(tripcntrs,&g_TRIPCNTRS, sizeof(TRIPCNTRS_T));
        //////////////////////////////////////////////
        xSemaphoreGive( g_mutex_access_tripcntrs);
    }
    else
    {
        debug_msg("ERROR incr, 'g_mutex_access_tripcntrs', mutex not obtained !\r\n");
        ret = -1;
    }
    return ret;
}


//




