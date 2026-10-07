/****************************************************
  FILE NAME		: f02-bootprocedure.c
  ORIGINATOR	: A Joshi
  LAST EDITED   : 18 OCT 2025
  PROJECT       : WS12_EPCB7011
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/

#include "f02-bootprocedure.h"

#define DEBUG 1
//
extern OAMCFGDBASE_T g_CFG_DBASE;
extern SERIALDBASE_T g_SERIAL_DBASE;

//
//fns protos defined here...
//
void boot_procedure(BOOTSTATUS_T * status, LOGICPARAMS_T *logicparams);
//

//fns defined here...
//
void boot_procedure(BOOTSTATUS_T * status, LOGICPARAMS_T *logicparams)
{
	//Vars used for returning boot result for each step
	int testresult = 0;
	int testfailstep = 0;
	int failstage = 0;
	int sdbaseresult = 0;
	int oamcfgdbaseresult = 0;
	int flg;
	uint8_t tstep = 0;
	uint8_t skip = 0;
	//
	uint8_t overallresult = BOOTRESULT_PASS;
	//
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//BOOTSTEP-1, TEST DATABASE
	init_test_ref();
	///////////////////////////////////////////////////////////////////
	iwdg_strobpulse();
	tstep++;
	testresult = 0;
	testfailstep = 0;
	flg = boot_testdbase_eeprom(&testresult,&testfailstep);
	if(flg!= 0)
	{
		status->result[tstep - 1] = BOOTRESULT_CRITICAL;
		status->fail_step[tstep - 1] = 1;
		debug_msg("BOOT Step-%d (TEST DBASE)..**CRITICAL FAILURE !\r\n",tstep);
		skip = 1;
		overallresult = BOOTRESULT_FAIL;
	}
	else
	{
		if(testresult != 0)
		{
			status->result[tstep - 1] = BOOTRESULT_FAIL;
			status->fail_step[tstep - 1] = 2;
			debug_msg("BOOT Step-%d (TEST DBASE)..**FAIL, Step %d !\r\n",tstep, testfailstep);
			overallresult = BOOTRESULT_FAIL;
		}
		else
		{
			status->result[tstep - 1] = BOOTRESULT_PASS;
			debug_msg("BOOT Step-%d (TEST DBASE)..PASS !\r\n",tstep);
		}
	}
	//
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//BOOTSTEP-2, DEVICE FACTORY CONFIG DATABASE (SERIALDBASE)
	///////////////////////////////////////////////////////////////////
	iwdg_strobpulse();
	//
	tstep++;
	testresult = 0;
	testfailstep = 0;
	//
	if(skip == 1)
	{
		status->fail_step[tstep - 1] = 0;
		status->result[tstep - 1] = BOOTRESULT_SKIP;
		debug_msg("BOOT Step-%d (FACTORY CONFIG)..**SKIPPED !\r\n" ,tstep);
	}
	else
	{
	    iwdg_strobpulse();
	    testresult = 0;
	    testfailstep = 0;
	    flg = boot_serialdbase_eeprom(&testresult,&testfailstep);
	    if(flg!= 0)
	    {
	        status->result[tstep - 1] = BOOTRESULT_CRITICAL;
	        status->fail_step[tstep - 1] = 1;
	        debug_msg("BOOT Step-%d (FACTORY CONFIG)..**CRITICAL FAILURE !\r\n",tstep);
	        skip = 1;
	        overallresult = BOOTRESULT_FAIL;
	    }
	    else
	    {
	        if(testresult != 0)
	        {
	            status->result[tstep - 1] = BOOTRESULT_FAIL;
	            status->fail_step[tstep - 1] = 2;
	            debug_msg("BOOT Step-%d (FACTORY CONFIG)..**FAIL, Step %d !\r\n",tstep, testfailstep);
	            overallresult = BOOTRESULT_FAIL;
	        }
	        else
	        {
	            status->result[tstep - 1] = BOOTRESULT_PASS;
	            debug_msg("BOOT Step-%d (FACTORY CONFIG)..PASS !\r\n",tstep);
	        }
	    }
	}
	//
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//BOOTSTEP-3, DEVICE OAM CONFIG DATABASE (OAMCFGDBASE_T)
	///////////////////////////////////////////////////////////////////
	iwdg_strobpulse();
	//
	tstep++;
	testresult = 0;
	testfailstep = 0;
    if(skip == 1)
    {
        status->fail_step[tstep - 1] = 0;
        status->result[tstep - 1] = BOOTRESULT_SKIP;
        debug_msg("BOOT Step-%d (FACTORY CONFIG)..**SKIPPED !\r\n" ,tstep);
    }
    else
    {
        iwdg_strobpulse();
        testresult = 0;
        testfailstep = 0;
        flg = boot_oamcfgdbase_eeprom(&testresult,&testfailstep);
        if(flg!= 0)
        {
            status->result[tstep - 1] = BOOTRESULT_CRITICAL;
            status->fail_step[tstep - 1] = 1;
            debug_msg("BOOT Step-%d (USER CONFIG)..**CRITICAL FAILURE !\r\n",tstep);
            skip = 1;
            overallresult = BOOTRESULT_FAIL;
        }
        else
        {
            if(testresult != 0)
            {
                status->result[tstep - 1] = BOOTRESULT_FAIL;
                status->fail_step[tstep - 1] = 2;
                debug_msg("BOOT Step-%d (USER CONFIG)..**FAIL, Step %d !\r\n",tstep, testfailstep);
                overallresult = BOOTRESULT_FAIL;
            }
            else
            {
                status->result[tstep - 1] = BOOTRESULT_PASS;
                debug_msg("BOOT Step-%d (USER CONFIG)..PASS !\r\n",tstep);
            }
        }
    }
    //
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    vTaskDelay(2000);
    logic_reset_pulse();
    vTaskDelay(10);
    logic_reset_pulse();
    vTaskDelay(2000);
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//BOOTSTEP-4, LOGIC
	///////////////////////////////////////////////////////////////////
	iwdg_strobpulse();
	//
	tstep++;
	testresult = 0;
	testfailstep = 0;
	//
	if(skip == 1)
	{
		status->result[tstep - 1] = BOOTRESULT_SKIP;
		debug_msg("BOOT Step-%d (FPGA DEVICE)..**SKIPPED !\r\n" ,tstep);
		logicparams->valid = 0;
	}
	else
	{
		///////////////////////////////////////////////////////////////////
	    //flg = 0;
		flg = boot_logic(logicparams, &testfailstep );
		if(flg!= 0)
		{
			status->result[tstep - 1] = BOOTRESULT_FAIL;
			status->fail_step[tstep - 1] = 2;
			debug_msg("BOOT Step-%d (FPGA DEVICE)..**FAIL, Step %d !\r\n",tstep, testfailstep);
			skip = 1;
			overallresult = BOOTRESULT_FAIL;
		}
		else
		{
			status->result[tstep - 1] = BOOTRESULT_PASS;
			debug_msg("BOOT Step-%d (FPGA DEVICE)..PASS !\r\n",tstep);
		}
		///////////////////////////////////////////////////////////////////
	}
	memcpy(&status->logicparams,logicparams, sizeof(LOGICPARAMS_T));
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//BOOTSTEP-5, LOGIC Unique ID (FPGA  UID)
	///////////////////////////////////////////////////////////////////
	iwdg_strobpulse();
	//
	tstep++;
	testresult = 0;
	testfailstep = 0;
	//
	if(skip == 1)
	{
		status->result[tstep - 1] = BOOTRESULT_SKIP;
		debug_msg("BOOT Step-%d (LOGIC UID)..**SKIPPED !\r\n" ,tstep);
	}
	else
	{
		/////////////////////////////////////////////////////////////////////////////////
		flg = boot_logicuid(logicparams, &testfailstep);
		if(flg!= 0)
		{
			status->result[tstep - 1] = BOOTRESULT_FAIL;
			status->fail_step[tstep - 1] = 2;
			debug_msg("BOOT Step-%d (LOGIC UID)..**FAILURE !, FailStep %d\r\n",tstep, testfailstep);
			skip = 1;
			overallresult = BOOTRESULT_FAIL;
		}
		else
		{
			status->result[tstep - 1] = BOOTRESULT_PASS;
			debug_msg("BOOT Step-%d (LOGIC UID)..PASS !\r\n",tstep);
		}
		///////////////////////////////////////////////////////
		//
	}
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //BOOTSTEP-6, LOGIC Config
    ///////////////////////////////////////////////////////////////////
    iwdg_strobpulse();
    //
    tstep++;
    testresult = 0;
    testfailstep = 0;
    //
    if(skip == 1)
    {
        status->result[tstep - 1] = BOOTRESULT_SKIP;
        debug_msg("BOOT Step-%d (LOGIC Config Regs)..**SKIPPED !\r\n" ,tstep);
    }
    else
    {
        /////////////////////////////////////////////////////////////////////////////////
        flg = boot_logicconfig(&failstage, &testfailstep);
        if(flg!= 0)
        {
            status->result[tstep - 1] = BOOTRESULT_FAIL;
            status->fail_step[tstep - 1] = 2;
            debug_msg("BOOT Step-%d (LOGIC Config Regs)..**FAILURE !, Stage %d, FailStep %d\r\n",tstep, failstage, testfailstep);
            skip = 1;
            overallresult = BOOTRESULT_FAIL;
        }
        else
        {
            status->result[tstep - 1] = BOOTRESULT_PASS;
            debug_msg("BOOT Step-%d (LOGIC Config Regs)..PASS !\r\n",tstep);
        }
        ///////////////////////////////////////////////////////
        //
    }
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++




	////////////////////////////////////
	status->boot_done = 1;
	status->overallresult = overallresult;
	////////////////////////////////////
#if DEBUG
	debug_msg("serialdbase:\r\n");
	DumpHexDebug((const void *)&g_SERIAL_DBASE, sizeof(SERIALDBASE_T));
	debug_msg("configdbase:\r\n");
	DumpHexDebug((const void *)&g_CFG_DBASE, sizeof(OAMCFGDBASE_T));
#endif

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//
}



