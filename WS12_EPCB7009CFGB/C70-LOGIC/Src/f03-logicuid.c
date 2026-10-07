/****************************************************
  FILE NAME		: f03-logicuid.c
  ORIGINATOR	: A Joshi
  LAST EDITED   : 13 JUN 2026
  PROJECT       : WS12_EPCB7011
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/
//
#define DEBUG 1
//

#include "f03-logicuid.h"
//
//fn proto defined here...
int boot_logicuid(LOGICPARAMS_T *logicparams, int *testfailstep);

//fn defined here...
int boot_logicuid(LOGICPARAMS_T *logicparams, int *testfailstep)
{
	int ret = -1;
	uint32_t fstep;
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	fstep = 0; //starting
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//compare UID1
	fstep++;
	if(logicparams->uid[0] != OAMSERLOGICUID1)
	{
		debug_msg("ERROR, UID1 Compare expected #%02X, read #%02X !\r\n",OAMSERLOGICUID1, logicparams->uid[0] );
		goto endx;
	}
#if DEBUG
	debug_msg("PASS UID1 %02X\r\n", OAMSERLOGICUID1);
#endif
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//compare UID2
	fstep++;
	if(logicparams->uid[1] != OAMSERLOGICUID2)
	{
		debug_msg("ERROR, UID2 Compare expected #%02X, read #%02X !\r\n", OAMSERLOGICUID2, logicparams->uid[1] );
		goto endx;
	}
#if DEBUG
	debug_msg("PASS UID2 %02X\r\n", OAMSERLOGICUID2);
#endif
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//compare UID3
	fstep++;
	if(logicparams->uid[2] != OAMSERLOGICUID3)
	{
		debug_msg("ERROR, UID3 Compare expected #%02X, read #%02X !\r\n",OAMSERLOGICUID3, logicparams->uid[2] );
		goto endx;
	}
#if DEBUG
	debug_msg("PASS UID3 %02X\r\n", OAMSERLOGICUID3);
#endif
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//compare UID4
	fstep++;
	if(logicparams->uid[3] != OAMSERLOGICUID4)
	{
		debug_msg("ERROR, UID4 Compare expected #%02X, read #%02X !\r\n",OAMSERLOGICUID4, logicparams->uid[3] );
		goto endx;
	}
#if DEBUG
	debug_msg("PASS UID4 %02X\r\n", OAMSERLOGICUID4);
#endif
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//compare UID5
	fstep++;
	if(logicparams->uid[4] != OAMSERLOGICUID5)
	{
		debug_msg("ERROR, UID5 Compare expected #%02X, read #%02X !\r\n",OAMSERLOGICUID5, logicparams->uid[4] );
		goto endx;
	}
#if DEBUG
	debug_msg("PASS UID5 %02X\r\n", OAMSERLOGICUID5);
#endif
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	/////////
	fstep = 0;
	ret = 0;
	//
	////////////////////////
	endx:
	////////////////////////
	*testfailstep = fstep;
	return ret;
	////////////////////////
	//
}
