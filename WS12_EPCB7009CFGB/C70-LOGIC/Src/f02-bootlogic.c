/****************************************************
  FILE NAME		: f02-bootlogic.c
  ORIGINATOR	: A Joshi
  LAST EDITED   : 18 OCT 2025
  PROJECT       : WS12_EPCB7011
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/
//
#define DEBUG 1
//

#include "f02-bootlogic.h"

int boot_logic(LOGICPARAMS_T *logicparams, int *testfailstep);
void construct_logicparams(LOGICPARAMS_T *logicparams, char *uidstr, char *reldatestr);


/*****************************************************************************
** Function name:		boot_logic
**
** Descriptions:		Bootup of DXT1-LIF
**
** Parameters:			NONE
**
** Returned value:		0 success, -1 fail
**
*****************************************************************************/
int boot_logic(LOGICPARAMS_T *logicparams, int *testfailstep)
{
	int ret = -1;
	int flg,j;
	uint32_t fstep;
	//uint8_t page = 0;
	uint8_t addr, rdata, exdata;
	uint8_t addr1,addr2, wdata1, wdata2, rdata1, rdata2;
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	fstep = 0; //starting
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	vTaskDelay(2000);
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//read cycle REG #04
	fstep++;
	addr = 0x04;
	exdata = 0x55;
	//flg = logic_read_cycle(page,addr,&rdata);
	flg = twi1logic_readcycle(0, addr, &rdata);
	if(flg != 0)
	{
		debug_msg("ERROR, 'logic_read_cycle' !\r\n");
		goto endx;
	}
	vTaskDelay(100);
	//read compare REG #04
	fstep++;
	if(exdata != rdata)
	{
		debug_msg("ERROR, Data Compare Reg #%2X !\r\n",addr);
		debug_msg("Logic Read Addr %02X, Data %02X, exdata %02X\r\n", addr, rdata, exdata);
		//goto endx;
	}
#if DEBUG
	debug_msg("PASS Logic Read Addr %02X, Data %02X\r\n", addr, rdata);
#endif
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//read cycle REG #05
	fstep++;
	addr = 0x05;
	exdata = 0xAA;
	//flg = logic_read_cycle(page,addr,&rdata);
	flg = twi1logic_readcycle(0, addr, &rdata);
	if(flg != 0)
	{
		debug_msg("ERROR, 'logic_read_cycle' !\r\n");
		goto endx;
	}
	/////////////////
	//read compare REG #05
	fstep++;
	if(exdata != rdata)
	{
		debug_msg("ERROR, Data Compare Reg #%2X !\r\n", addr);
		debug_msg("Logic Read Addr %02X, Data %02X, exdata %02X\r\n", addr, rdata, exdata);
		//goto endx;
	}
	vTaskDelay(100);
#if DEBUG
	debug_msg("PASS Logic Read Addr %02X, Data %02X\r\n", addr, rdata);
#endif
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//read cycle REG #00
	fstep++;
	addr = 0x00;
	exdata = 0x99;
	//flg = logic_read_cycle(page,addr,&rdata);
	flg = twi1logic_readcycle(0, addr, &rdata);
	if(flg != 0)
	{
		debug_msg("ERROR, 'logic_read_cycle' !\r\n");
		goto endx;
	}
	vTaskDelay(100);
	/////////////////
	//read compare REG #00
	fstep++;
	if(exdata != rdata)
	{
		debug_msg("ERROR, Data Compare Reg #%2X !\r\n", addr);
		debug_msg("Logic Read Addr %02X, Data %02X, exdata %02X\r\n", addr, rdata, exdata);
		//goto endx;
	}
#if DEBUG
	debug_msg("PASS Logic Read Addr %02X, Data %02X\r\n", addr, rdata);
#endif
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//read cycle REG #01
	fstep++;
	addr = 0x01;
	exdata = 0x22;
	//flg = logic_read_cycle(page,addr,&data);
	flg = twi1logic_readcycle(0, addr, &rdata);
	if(flg != 0)
	{
		debug_msg("ERROR, 'logic_read_cycle' !\r\n");
		goto endx;
	}
	vTaskDelay(100);
	/////////////////
	//read compare REG #00
	fstep++;
	if(exdata != rdata)
	{
		debug_msg("ERROR, Data Compare Reg #%2X !\r\n", addr);
		debug_msg("Logic Read Addr %02X, Data %02X, exdata %02X\r\n", addr, rdata, exdata);
		//goto endx;
	}
#if DEBUG
	debug_msg("PASS Logic Read Addr %02X, Data %02X\r\n", addr, rdata);
#endif
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//
	//
	//write and read cycle REG #02,#03
	addr1 = 0x02;
	addr2 = 0x03;
	wdata1 = 0xA5;
	wdata2 = 0x5A;
	///////////////
	fstep++;
	//flg = logic_write_cycle(page,addr1,wdata1);
	flg = twi1logic_writecycle(0,addr1,wdata1);
	if(flg != 0)
	{
		debug_msg("ERROR, 'logic_read_cycle' !\r\n");
		goto endx;
	}
	vTaskDelay(100);
	////////////////
	fstep++;
	//flg = logic_write_cycle(page,addr2,wdata2);
	flg = twi1logic_writecycle(0,addr2,wdata2);
	if(flg != 0)
	{
		debug_msg("ERROR, 'logic_read_cycle' !\r\n");
		goto endx;
	}
	vTaskDelay( 100);
	////////////////
	fstep++;
	//flg = logic_read_cycle(page,addr1,&rdata1);
	flg = twi1logic_readcycle(0, addr1, &rdata1);
	if(flg != 0)
	{
		debug_msg("ERROR, 'logic_read_cycle' !\r\n");
		goto endx;
	}
	vTaskDelay(100);
	/////////////////
	fstep++;
	//flg = logic_read_cycle(page,addr2,&rdata2);
	flg = twi1logic_readcycle(0, addr2, &rdata2);
	if(flg != 0)
	{
		debug_msg("ERROR, 'logic_read_cycle' !\r\n");
		goto endx;
	}
	vTaskDelay(100);
	/////////////////
	//read compare REG addr1
	fstep++;
	if(wdata1 != rdata1)
	{
		debug_msg("ERROR, Data Compare Reg #%2X !\r\n", addr1);
		debug_msg("Logic Read Addr %02X, Data %02X, exdata %02X\r\n", addr1, rdata1, wdata1);
		goto endx;
	}
#if DEBUG
	debug_msg("PASS Logic Read Addr %02X, Data %02X\r\n", addr1, rdata1);
#endif
	/////////////////
	//read compare REG addr2
	fstep++;
	if(wdata2 != rdata2)
	{
		debug_msg("ERROR, Data Compare Reg #%2X !\r\n", addr2);
		debug_msg("Logic Read Addr %02X, Data %02X, exdata %02X\r\n", addr2, rdata2, wdata2);
		goto endx;
	}
#if DEBUG
	debug_msg("PASS Logic Read Addr %02X, Data %02X\r\n", addr2, rdata2);
#endif
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//fetch UID
	for(j=0;j<5;j++)
	{
		fstep++;
		//flg = logic_read_cycle(page, (0xfb + j),&rdata);
		flg = twi1logic_readcycle(0, (0xfb + j), &rdata);
		if(flg != 0)
		{
			debug_msg("ERROR, 'logic_read_cycle' !\r\n");
			goto endx;
		}
		logicparams->uid[j] = rdata;
		vTaskDelay(100);
	}
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//fetch REL DATA
	for(j=0;j<4;j++)
	{
		fstep++;
		//flg = logic_read_cycle(page, (0xf0 + j),&rdata);
		flg = twi1logic_readcycle(0, (0xf0 + j), &rdata);
		if(flg != 0)
		{
			debug_msg("ERROR, 'logic_read_cycle' !\r\n");
			goto endx;
		}
		logicparams->logicrelease[j] = rdata;
		vTaskDelay(100);
	}
    for(j=0;j<2;j++)
    {
        fstep++;
        //flg = logic_read_cycle(page, (0xf0 + j),&rdata);
        flg = twi1logic_readcycle(0, (0xf4 + j), &rdata);
        if(flg != 0)
        {
            debug_msg("ERROR, 'logic_read_cycle' !\r\n");
            goto endx;
        }
        logicparams->logictime[j] = rdata;
        vTaskDelay(100);
    }


	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	logicparams->valid = 1;
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	/////////
	fstep = 0;
	ret = 0;
	//
	////////////////////////
	endx:

	construct_logicparams(logicparams, NULL, NULL);
	////////////////////////
	*testfailstep = fstep;
	return ret;
	////////////////////////
}


void construct_logicparams(LOGICPARAMS_T *logicparams, char *uidstr, char *reldatestr)
{
	int j;
	char str[100];
	char buf[5];
	////////////////////////////////////////////////////
	//UID Bytes msg
	memset(str,0,sizeof(str));
	if(logicparams->valid == 0)
	{
		strcpy(str,"UID Not Valid !\r\n");
	}
	else
	{
		sprintf(str,"UID Bytes : ");
		for(j=0;j<5;j++)
		{
			memset(buf,0,sizeof(buf));
			sprintf(buf,"%02X ", logicparams->uid[j]);
			strcat(str,buf);
		}
		strcat(str,"\r\n\r\n");
	}
	if(uidstr != NULL)
	{
		strcpy(uidstr,str);
	}
	debug_msg(str);
	////////////////////////////////////////////////////
	//rel date msg
	memset(str,0,sizeof(str));
	if(logicparams->valid == 0)
	{
		strcpy(str, (const char *)"Release Date Not Valid !\r\n");
	}
	else
	{
		sprintf(str, "Release DATE [DD-MM-YYYY] %02X-%02X-%02X%02X\r\n\r\n",
				logicparams->logicrelease[0], logicparams->logicrelease[1],
				logicparams->logicrelease[2], logicparams->logicrelease[3]);
	}
	if(reldatestr != NULL)
	{
		strcpy(reldatestr, (const char *) str);
	}
	debug_msg(str);
	////////////////////////////////////////////////////
	////
}

