/****************************************************
  FILE NAME		: f01-boot-task.c
  ORIGINATOR	: A Joshi
  LAST EDITED   : 18 OCT 2025
  PROJECT       : WS12_EPCB7011
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/



#include "f01-boot-task.h"

#define DEBUG 0

static BOOTSTATUS_T BOOT_STATUS;
static SemaphoreHandle_t g_mutex_access_bootstatus;
//
extern TaskHandle_t g_bootup_task;

//fns defined here...
void StartTaskBoot(void *argument);
int create_mutex_access_bootstatus(void);
static int init_bootstatus_mutex(void);
static int set_bootstatus_mutex(BOOTSTATUS_T * status);
int copy_bootstatus_mutex(BOOTSTATUS_T * status);
int copy_bootpass_mutex(uint8_t *pass);
//

char *getbootresultstr(uint8_t result);
char *getbootstepstr(uint8_t step);
//

void StartTaskBoot(void *argument)
{
    //////////////////////////////
    //
	//init_oamcfgdbase_global();
	init_bootstatus_mutex();
	//
	iwdg_strobpulse();
	//
	//
	debug_msg("Starting BOOT UP !\r\n");
	uint8_t *ui8_response_buf = (uint8_t *)pvPortMalloc(MAX_RESPONSE_PAGE);
	if (ui8_response_buf == NULL)
	{
		printf("StartTaskBoot: FAIL Alloc Mem - 'ui8_response_buf'\r\n");
		while(1);//stuck here
	}
	//////////////////////////////////////////////////////////////////////
	BOOTSTATUS_T *bootstatus = (BOOTSTATUS_T *) pvPortMalloc(sizeof(BOOTSTATUS_T));
	if (bootstatus == NULL)
	{
		printf("FAIL Alloc Mem - 'BOOTSTATUS_T'\r\n");
		while(1);//stuck here
	}
	//////////////////////////////////////////////////////////////////////
	LOGICPARAMS_T *logicparams = (LOGICPARAMS_T *) pvPortMalloc(sizeof(LOGICPARAMS_T));
	if (logicparams == NULL)
	{
		printf("FAIL Alloc Mem - 'LOGICPARAMS_T'\r\n");
		while(1);//stuck here
	}
	//////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////
	boot_procedure((BOOTSTATUS_T *)bootstatus, (LOGICPARAMS_T *)logicparams);
	set_bootstatus_mutex((BOOTSTATUS_T *)bootstatus);
	/////////////////////////////////////////////////////////////////////
	if(bootstatus->overallresult == BOOTRESULT_PASS)
	{
		drive_ledselftest(1);
		debug_msg("SelfTest LED ON\r\n");
	}
	else
	{
		drive_ledselftest(0);
		debug_msg("SelfTest LED OFF\r\n");
	}
	//////////////////////////////////////////////////////////////
	//
	vPortFree(ui8_response_buf);
	vPortFree(bootstatus);
	vPortFree(logicparams);
	//
	///////////////////////////////////////////////////////
	vTaskDelay(1000);
    ///////////////////////////////////////////////////////
    if(give_sem_bootup_to_postboot() != 0)
    {
        stop_watchdog();
        while(1)
        {

        }
    }
    debug_msg("\r\n TaskPostBoot Permitted..\r\n");
    //
	//////////////////////////////////////////////////////////////
	debug_msg("\r\n'bootup_task' deleting itself...\r\n");
	vTaskDelete(g_bootup_task);
	//////////////////////////////////////////////////////////////
	//
	////////////////////////////////////////////////
	//
	while(1)
	{

	}
	//


}


int create_mutex_access_bootstatus(void)
{
	g_mutex_access_bootstatus = xSemaphoreCreateMutex();
	if( g_mutex_access_bootstatus == NULL )
	{
		debug_msg("ERROR create, 'g_mutex_access_bootstatus' !\r\n");
		return -1;
	}
	else
	{
		//The semaphore can now be used.
		debug_msg("SUCCESS create, 'g_mutex_access_bootstatus' !\r\n");
	}
    return 0;
}

static int init_bootstatus_mutex(void)
{
	int ret = 0;
	int j;
	if( xSemaphoreTake( g_mutex_access_bootstatus, ( TickType_t ) 10 ) == pdTRUE )
    {
		BOOT_STATUS.boot_done = 0;
		BOOT_STATUS.overallresult = BOOTRESULT_UNKNOWN;
		//
		for(j=0;j<BOOTSTATUS_STEPS;j++)
		{
			BOOT_STATUS.result[j] = BOOTRESULT_UNKNOWN;
			BOOT_STATUS.fail_step[j] = 0;
		}
        xSemaphoreGive( g_mutex_access_bootstatus);
    }
	else
	{
    	debug_msg("ERROR init, 'g_mutex_access_bootstatus', mutex not obtained !\r\n");
    	ret = -1;
	}
	return ret;
}

static int set_bootstatus_mutex(BOOTSTATUS_T * status)
{
	int ret = 0;
	if( xSemaphoreTake( g_mutex_access_bootstatus, ( TickType_t ) 10 ) == pdTRUE )
    {
		memcpy(&BOOT_STATUS,status,sizeof(BOOTSTATUS_T));
        xSemaphoreGive( g_mutex_access_bootstatus);
    }
	else
	{
    	debug_msg("ERROR set, 'g_mutex_access_bootstatus', mutex not obtained !\r\n");
    	ret = -1;
	}
	return ret;
}

int copy_bootstatus_mutex(BOOTSTATUS_T * status)
{
	int ret = 0;
	if( xSemaphoreTake(g_mutex_access_bootstatus, ( TickType_t ) 10 ) == pdTRUE )
    {
		memcpy(status,&BOOT_STATUS,sizeof(BOOTSTATUS_T));
        xSemaphoreGive( g_mutex_access_bootstatus);
    }
	else
	{
    	debug_msg("ERROR copy, 'g_mutex_access_bootstatus', mutex not obtained !\r\n");
    	ret = -1;
	}
	return ret;
}

int copy_bootpass_mutex(uint8_t *pass)
{
	int ret = 0;
	if( xSemaphoreTake(g_mutex_access_bootstatus, ( TickType_t ) 10 ) == pdTRUE )
    {
		memcpy(pass,&BOOT_STATUS.overallresult,sizeof(uint8_t));
        xSemaphoreGive( g_mutex_access_bootstatus);
    }
	else
	{
    	debug_msg("ERROR copy, 'g_mutex_access_bootstatus', mutex not obtained !\r\n");
    	ret = -1;
	}
	return ret;
}



char *getbootresultstr(uint8_t result)
{
	switch(result)
	{
		case BOOTRESULT_UNKNOWN:
		return "** Unknown";
		//
		case BOOTRESULT_PASS:
		return "PASS !";
		//
		case BOOTRESULT_FAIL:
		return "** FAIL";
		//
		case BOOTRESULT_CRITICAL:
		return "** CRITICAL FAILURE";
		//
		case BOOTRESULT_SKIP:
		return "** Skipped";
		//
		case BOOTRESULT_ERROR:
		return "** ERROR";
		//
		default:
		return "??";
		//
	}
}

char *getbootstepstr(uint8_t step)
{
	switch(step)
	{
		case 1:
		return "(TEST FLASH)";
		//
		case 2:
		return "(FACTORY DataBase)";
		//
		case 3:
		return "(USER Config)";
		//
		case 4:
		return "(FPGA Device)";
		//
		case 5:
		return "(LOGIC UID)";
        //
        case 6:
        return "(LOGIC Config REGS)";
		//
		default:
		return "(?)";
		//
	}
}

