/****************************************************
  FILE NAME		: f04-sem-bootup.c
  ORIGINATOR	: A Joshi
  LAST EDITED   : 29 SEPT 2026
  PROJECT       : WS12_EPCB7009
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/


#include "f04-sem-bootup.h"

#define DEBUG 0


///////////////////////////////////////////////////////////
//semaphores
static SemaphoreHandle_t g_semaphore_boot_to_postboot;
static SemaphoreHandle_t g_semaphore_postboot_to_TaskCLI;
static SemaphoreHandle_t g_semaphore_postboot_to_TaskCntr;
static SemaphoreHandle_t g_semaphore_postboot_to_TaskFetchAlarm;
///////////////////////////////////////////////////////////


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

//
//fns defined here...
int create_sem_boot_to_postboot(void)
{
    g_semaphore_boot_to_postboot = xSemaphoreCreateBinary();
    if( g_semaphore_boot_to_postboot == NULL )
    {
	    debug_msg("FAIL Create 'g_semaphore_boot_to_postboot' !\r\n");
	    return -1;
    }
    else
    {
       //The semaphore can now be used. Its handle is stored.
	   debug_msg("SUCCESS Create 'g_semaphore_boot_to_postboot' !\r\n");
	   return 0;
    }
}

int wait_postboot_for_bootup(void)
{
	if( xSemaphoreTake( g_semaphore_boot_to_postboot, ( TickType_t ) 30000 ) == pdTRUE )
	{
		//We were able to obtain the semaphore so 'proceed'
		return 0;
	}
	else
	{
		//We could not obtain the semaphore - timeout
	    debug_msg("EXCEPTION ! - g_semaphore_boot_to_postboot TIMEOUT\r\n");
	    return -1;
	}
}


int give_sem_bootup_to_postboot(void)
{
	if( xSemaphoreGive( g_semaphore_boot_to_postboot ) != pdTRUE )
 	{
 	    // We would NOT expect this call to fail because we can give an empty binary semaphore
 		debug_msg("EXCEPTION ! - xSemaphoreGive 'g_semaphore_boot_to_postboot'\r\n");
 		return -1;
  	}
	return 0;
	//
}

int create_sem_postboot_to_TaskCLI(void)
{
    g_semaphore_postboot_to_TaskCLI = xSemaphoreCreateBinary();
    if( g_semaphore_postboot_to_TaskCLI == NULL )
    {
        debug_msg("FAIL Create 'g_semaphore_postboot_to_TaskCLI' !\r\n");
        return -1;
    }
    else
    {
       //The semaphore can now be used. Its handle is stored.
       debug_msg("SUCCESS Create 'g_semaphore_postboot_to_TaskCLI' !\r\n");
       return 0;
    }
}

int wait_TaskCLI_for_postboot(void)
{
    if( xSemaphoreTake( g_semaphore_postboot_to_TaskCLI, ( TickType_t ) 40000 ) == pdTRUE )
    {
        //We were able to obtain the semaphore so 'proceed'
        return 0;
    }
    else
    {
        //We could not obtain the semaphore - timeout
        debug_msg("EXCEPTION ! - g_semaphore_postboot_to_TaskCLI TIMEOUT\r\n");
        return -1;
    }
}


int give_sem_postboot_to_TaskCLI(void)
{
    if( xSemaphoreGive( g_semaphore_postboot_to_TaskCLI ) != pdTRUE )
    {
        // We would NOT expect this call to fail because we can give an empty binary semaphore
        debug_msg("EXCEPTION ! - xSemaphoreGive 'g_semaphore_postboot_to_TaskCLI'\r\n");
        return -1;
    }
    return 0;
    //
}

int create_sem_postboot_to_TaskCntr(void)
{
    g_semaphore_postboot_to_TaskCntr = xSemaphoreCreateBinary();
    if( g_semaphore_postboot_to_TaskCntr == NULL )
    {
        debug_msg("FAIL Create 'g_semaphore_postboot_to_TaskCntr' !\r\n");
        return -1;
    }
    else
    {
       //The semaphore can now be used. Its handle is stored.
       debug_msg("SUCCESS Create 'g_semaphore_postboot_to_TaskCntr' !\r\n");
       return 0;
    }
}

int wait_TaskCntr_for_postboot(void)
{
    if( xSemaphoreTake( g_semaphore_postboot_to_TaskCntr, ( TickType_t ) 40000 ) == pdTRUE )
    {
        //We were able to obtain the semaphore so 'proceed'
        return 0;
    }
    else
    {
        //We could not obtain the semaphore - timeout
        debug_msg("EXCEPTION ! - g_semaphore_postboot_to_TaskCntr TIMEOUT\r\n");
        return -1;
    }
}


int give_sem_postboot_to_TaskCntr(void)
{
    if( xSemaphoreGive( g_semaphore_postboot_to_TaskCntr ) != pdTRUE )
    {
        // We would NOT expect this call to fail because we can give an empty binary semaphore
        debug_msg("EXCEPTION ! - xSemaphoreGive 'g_semaphore_postboot_to_TaskCntr'\r\n");
        return -1;
    }
    return 0;
    //
}

int create_sem_postboot_to_TaskFetchAlarm(void)
{
    g_semaphore_postboot_to_TaskFetchAlarm = xSemaphoreCreateBinary();
    if( g_semaphore_postboot_to_TaskFetchAlarm == NULL )
    {
        debug_msg("FAIL Create 'g_semaphore_postboot_to_TaskFetchAlarm' !\r\n");
        return -1;
    }
    else
    {
       //The semaphore can now be used. Its handle is stored.
       debug_msg("SUCCESS Create 'g_semaphore_postboot_to_TaskFetchAlarm' !\r\n");
       return 0;
    }
}

int wait_TaskFetchAlarm_for_postboot(void)
{
    if( xSemaphoreTake( g_semaphore_postboot_to_TaskFetchAlarm, ( TickType_t ) 40000 ) == pdTRUE )
    {
        //We were able to obtain the semaphore so 'proceed'
        return 0;
    }
    else
    {
        //We could not obtain the semaphore - timeout
        debug_msg("EXCEPTION ! - g_semaphore_postboot_to_TaskFetchAlarm TIMEOUT\r\n");
        return -1;
    }
}


int give_sem_postboot_to_TaskFetchAlarm(void)
{
    if( xSemaphoreGive( g_semaphore_postboot_to_TaskFetchAlarm ) != pdTRUE )
    {
        // We would NOT expect this call to fail because we can give an empty binary semaphore
        debug_msg("EXCEPTION ! - xSemaphoreGive 'g_semaphore_postboot_to_TaskFetchAlarm'\r\n");
        return -1;
    }
    return 0;
    //
}

