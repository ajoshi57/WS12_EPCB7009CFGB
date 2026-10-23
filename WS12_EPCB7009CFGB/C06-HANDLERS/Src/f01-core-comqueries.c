/****************************************************
  FILE NAME		: f01-core-comqueries.c
  ORIGINATOR	: A Joshi
  LAST EDITED   : 23 OCT 2026
  PROJECT       : WS12_EPCB7011
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/

#include "f01-core-comqueries.h"
//
#define DEBUG 0
#define DEBUGTWI0EP128 0
#define DEBUGTWI1LOGICRD 0
#define DEBUGTWI1LOGICWR 0

//
//cli cmds for serial oam can have identifier "com"
//
int handle_showcomversion(int argc, char **argv, uint8_t cmdkwlen, char *response_buf);
int handle_showcomsettings(int argc, char **argv, uint8_t cmdkwlen, char *response_buf);
int handle_showcomselftest(int argc, char **argv, uint8_t cmdkwlen, char *response_buf);
int handle_showtripcounters(int argc, char **argv, uint8_t cmdkwlen, char *response_buf);
int handle_showalarmrelay(int argc, char **argv, uint8_t cmdkwlen, char *response_buf);
//////////////////////////////////////////////////////////////////////
//
static int construct_showcomversion_response(char *response_buf);
static int construct_showcomsettings_response(char *response_buf);
static int construct_showcomselftest_response(char *response_buf);
static int construct_showtripcounters_response(char *response_buf);
static int construct_showalarmrelay_response(char *response_buf);

//
//////////////////////////////////////////////////////////////////////
//
void init_difcommand_list_core_comqueries(void);
//
//
CMDMSG_T showcomversion =
{
 		"#showversion#", //cmdstr
 		 0,
 		"Show Version Details\r\n", //help_str
 		(CLIGROUP_CORECOMQUERY + 1), //command_id
 		AUTH_VIEWUSER,         //auth_level
 		&handle_showcomversion      //handler fn
};

CMDMSG_T showcomsettings =
{
  		"#showsettings#", //cmdstr
  		1,
  		"Show Configuration Settings\r\n", //help_str
  		(CLIGROUP_CORECOMQUERY + 2), //command_id
  		AUTH_VIEWUSER,         //auth_level
  		&handle_showcomsettings      //handler fn
};


CMDMSG_T showcomselftest =
{
 		"#showselftest#", //cmdstr
 		1,
 		"Show Selftest Test Report of Most recent BOOT-UP\r\n", //help_str
 		(CLIGROUP_CORECOMQUERY + 3), //command_id
 		AUTH_VIEWUSER,           //auth_level
 		&handle_showcomselftest  //handler fn
};

CMDMSG_T showtripcounters =
{
        "#showtripcounters#", //cmdstr
        1,
        "Show TRIP COUNTERS\r\n", //help_str
        (CLIGROUP_CORECOMQUERY + 4), //command_id
        AUTH_VIEWUSER,           //auth_level
        &handle_showtripcounters  //handler fn
};

CMDMSG_T showalarmrelay =
{
        "#showalarmrelay#", //cmdstr
        1,
        "Show ALARM RELAY STATUS\r\n", //help_str
        (CLIGROUP_CORECOMQUERY + 5), //command_id
        AUTH_VIEWUSER,           //auth_level
        &handle_showalarmrelay  //handler fn
};


int handle_showcomversion(int argc, char **argv, uint8_t cmdkwlen, char *response_buf)
{
	//possibilities
	//showcomversion
	//showcomversion help
	//showcomversion ?
	int c1 = 0;
	int c1x = 0;
	int selhelp = 0;
	int ret = RET_FAIL;
	uint16_t superlen,msglen ;
	char buf[100];
	int j;
	int flg;
	//
    uint8_t titlelen;
    uint8_t title[MAX_TITLE_SIZE+1];
     uint8_t addr,data;
     //
     //vars used for testing Logic TWI1
     uint8_t addr1,addr2;
     uint8_t wdata1,wdata2;
     uint8_t rdata1,rdata2;
     uint8_t addrlogic, rdatalogic, exdatalogic;

	//================================================================================
	uint8_t *oammsg = (uint8_t *) pvPortMalloc(sizeof(uint8_t) * MAX_RESPSTRING_SIZE);
	if (oammsg == NULL)
	{
		debug_msg("\r\nhandle_showversioncc: FAIL Alloc Mem - 'oammsg'\r\n");
		while (1)
			;	//stuck here
	}
	///////////////////////////////////////////////////////////////////////////
	VALIDRESP_T *oamresppkt = (VALIDRESP_T *) pvPortMalloc(sizeof(VALIDRESP_T));
	if(oamresppkt == NULL)
	{
		debug_msg("\r\nhandle_showversioncc: FAIL Alloc Mem - 'VALIDRESP_T'\r\n");
		while (1)
			;	//stuck here
	}
	//================================================================================
	///////////////////////////////
	///////////////////////////////
	//
	if (argc != 1 && argc != 2)
	{
		memset(oammsg,0,MAX_RESPSTRING_SIZE);
    	sprintf((char *)oammsg, "Incorrect Argument Count !\r\n");
        strcat((char *)oammsg, "SYNTAX ERROR\r\n@FAIL@\r\n");
		construct_oam_responsepkt(oammsg, oamresppkt);
#if DEBUG
        debug_msg((char *)oamresppkt->resppkt);
#endif
        memcpy(response_buf, oamresppkt->resppkt, oamresppkt->totallen );
	    goto endx;
	}
	//check...this could be 'help'
	selhelp = 0;
	if (argc == 1)
	{
		selhelp = 0;
	}
	else
	{
		if ((argc == 2) && (argv[1] == NULL))
		{
			memset(oammsg,0,MAX_RESPSTRING_SIZE);
			sprintf((char *)oammsg, "Execution Error !\r\n@FAIL@\r\n");
			construct_oam_responsepkt(oammsg, oamresppkt);
#if DEBUG
	       debug_msg((char *)oamresppkt->resppkt);
#endif
	       memcpy(response_buf, oamresppkt->resppkt, oamresppkt->totallen );
		   goto endx;
		}
		else
		{
			c1 = strncmp(argv[1], "help",strlen("help"));
			c1x = strncmp(argv[1], "?",1);
			if (c1 == 0 || c1x == 0)
			{
				selhelp = 1;
			}
			else
			{
				memset(oammsg,0,MAX_RESPSTRING_SIZE);
				sprintf((char *)oammsg, "Execution Error !\r\n@FAIL@\r\n");
				construct_oam_responsepkt(oammsg, oamresppkt);
#if DEBUG
		       debug_msg((char *)oamresppkt->resppkt);
#endif
		       memcpy(response_buf, oamresppkt->resppkt, oamresppkt->totallen );
			   goto endx;
			}
		}
	}
	//////////////////////////////////////////////////////////////////////
    if(selhelp != 0)
    {
		 construct_oam_large_template_responsepkt((uint8_t *)response_buf, RESPONSE_BUF_SIZE);
  	    strncat(response_buf, showcomversion.help_str, strlen(showcomversion.help_str));
    	strcat(response_buf, "\r\n");
        //
 		memset(buf,0,sizeof(buf));
        sprintf(buf, "Cmd String <%s> has no Arguments\r\n",showcomversion.command_name);
        strcat(response_buf, buf);
        //
 		memset(buf,0,sizeof(buf));
        sprintf(buf, "E.G. %s <CR> \r\n", showcomversion.command_name);
        strcat(response_buf, buf);
        //
		strcat((char *)response_buf, "@OK@\r\n##");
        superlen = strlen(response_buf);
        msglen = superlen - 10;
        //
        /////////////////////
        sprintf(buf,"$$[%04d]",msglen);
        for(j=0;j<8;j++)
        {
        	response_buf[j] = buf[j];
        }
#if DEBUG
       debug_msg((char *)response_buf);
#endif
        /////////////////////
        //stop_watchdog();

        ret = RET_SUCCESS;
        goto endx;
        //
    }
    //////////////////////////////////////////////////////////////////////
    //NORMAL EXECUTION
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//Construct VALID RESPONSE
	flg = construct_showcomversion_response(response_buf);
	if(flg != 0)
	{
		goto endx;
	}
	ret = RET_SUCCESS;
	//////////////////////////////////////////////////////////////////////
#if DEBUGTWI0EP128
	memset(title,0,sizeof(title));
    for(j = 0;j < MAX_TITLE_SIZE;j++)
    {
        addr = j;
        ep24aa128_readcycle(0,0, addr, &data);
        title[j] = data;
    }
    debug_msg("TITLE EP24AA128 = %s\r\n", title);
#endif
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#if DEBUGTWI1LOGICRD
    addrlogic = 0xFB;
    exdatalogic = 0x0B;
    //flg = logic_read_cycle(page,addr,&rdata);
    flg = twi1logic_readcycle(0, addrlogic, &rdatalogic);
    if(flg != 0)
    {
        debug_msg("ERROR, 'logic_read_cycle' !\r\n");
        goto skiptwi1a;
    }
    else
    {
        debug_msg("PASS, 'logic_read_cycle' !\r\n");
    }
    vTaskDelay(100);
    if(exdatalogic != rdatalogic)
    {
        debug_msg("ERROR, Data Compare Reg #%2X !\r\n",addrlogic);
        debug_msg("Logic Read Addr %02X, Data %02X, exdata %02X\r\n", addrlogic, rdatalogic, exdatalogic);
    }
    else
    {
        debug_msg("PASS Logic Read Addr %02X, Data %02X\r\n", addrlogic, rdatalogic);
    }
    skiptwi1a:
    //////////////////////////////////////////////////
    addrlogic = 0xFC;
    exdatalogic = 0x70;
    //flg = logic_read_cycle(page,addr,&rdata);
    flg = twi1logic_readcycle(0, addrlogic, &rdatalogic);
    if(flg != 0)
    {
        debug_msg("ERROR, 'logic_read_cycle' !\r\n");
        goto skiptwi1b;
    }
    else
    {
        debug_msg("PASS, 'logic_read_cycle' !\r\n");
    }
    vTaskDelay(100);
    if(exdatalogic != rdatalogic)
    {
        debug_msg("ERROR, Data Compare Reg #%2X !\r\n",addrlogic);
        debug_msg("Logic Read Addr %02X, Data %02X, exdata %02X\r\n", addrlogic, rdatalogic, exdatalogic);
    }
    else
    {
        debug_msg("PASS Logic Read Addr %02X, Data %02X\r\n", addrlogic, rdatalogic);
    }
    skiptwi1b:
    //////////////////////////////////////////////////
    addrlogic = 0xFD;
    exdatalogic = 0x11;
    //flg = logic_read_cycle(page,addr,&rdata);
    flg = twi1logic_readcycle(0, addrlogic, &rdatalogic);
    if(flg != 0)
    {
        debug_msg("ERROR, 'logic_read_cycle' !\r\n");
        goto skiptwi1c;
    }
    else
    {
        debug_msg("PASS, 'logic_read_cycle' !\r\n");
    }
    vTaskDelay(100);
    if(exdatalogic != rdatalogic)
    {
        debug_msg("ERROR, Data Compare Reg #%2X !\r\n",addrlogic);
        debug_msg("Logic Read Addr %02X, Data %02X, exdata %02X\r\n", addrlogic, rdatalogic, exdatalogic);
    }
    else
    {
        debug_msg("PASS Logic Read Addr %02X, Data %02X\r\n", addrlogic, rdatalogic);
    }
    skiptwi1c:
    //////////////////////////////////////////////////
    addrlogic = 0xFE;
    exdatalogic = 0x69;
    //flg = logic_read_cycle(page,addr,&rdata);
    flg = twi1logic_readcycle(0, addrlogic, &rdatalogic);
    if(flg != 0)
    {
        debug_msg("ERROR, 'logic_read_cycle' !\r\n");
        goto skiptwi1d;
    }
    else
    {
        debug_msg("PASS, 'logic_read_cycle' !\r\n");
    }
    vTaskDelay(100);
    if(exdatalogic != rdatalogic)
    {
        debug_msg("ERROR, Data Compare Reg #%2X !\r\n",addrlogic);
        debug_msg("Logic Read Addr %02X, Data %02X, exdata %02X\r\n", addrlogic, rdatalogic, exdatalogic);
    }
    else
    {
        debug_msg("PASS Logic Read Addr %02X, Data %02X\r\n", addrlogic, rdatalogic);
    }
    skiptwi1d:
    //////////////////////////////////////////////////
#endif
    //////////////////////////////////////////////////
#if DEBUGTWI1LOGICWR
    //
    //write and read cycle REG #02,#03
    addr1 = 0x02;
    addr2 = 0x03;
    wdata1 = 0xA5;
    wdata2 = 0x5A;
    ///////////////
    //flg = logic_write_cycle(page,addr1,wdata1);
    flg = twi1logic_writecycle(0, addr1, wdata1);
    if(flg != 0)
    {
        debug_msg("ERROR, 'logic_write_cycle' !\r\n");
        goto skiptwi1wr;
    }
    vTaskDelay(100);
    ////////////////
    flg = twi1logic_writecycle(0, addr2, wdata2);
    if(flg != 0)
    {
        debug_msg("ERROR, 'logic_write_cycle' !\r\n");
        goto skiptwi1wr;
    }
    vTaskDelay( 100);
    ////////////////
    //flg = logic_read_cycle(page,addr1,&rdata1);
    flg = twi1logic_readcycle(0, addr1, &rdata1);
    if(flg != 0)
    {
        debug_msg("ERROR, 'logic_read_cycle' !\r\n");
        goto skiptwi1wr;
    }
    vTaskDelay(100);
    /////////////////
    //flg = logic_read_cycle(page,addr2,&rdata2);
    flg = twi1logic_readcycle(0, addr2, &rdata2);
    if(flg != 0)
    {
        debug_msg("ERROR, 'logic_read_cycle' !\r\n");
        goto skiptwi1wr;
    }
    vTaskDelay(100);
    /////////////////
    //read compare REG addr1
    if(wdata1 != rdata1)
    {
        debug_msg("ERROR, Data Compare Reg #%2X !\r\n", addr1);
        debug_msg("Logic Read Addr %02X, Data %02X, exdata %02X\r\n", addr1, rdata1, wdata1);
    }
    else
    {
        debug_msg("PASS Logic Read Addr %02X, Data %02X\r\n", addr1, rdata1);
    }
    /////////////////
    //read compare REG addr2
    if(wdata2 != rdata2)
    {
        debug_msg("ERROR, Data Compare Reg #%2X !\r\n", addr2);
        debug_msg("Logic Read Addr %02X, Data %02X, exdata %02X\r\n", addr2, rdata2, wdata2);
    }
    else
    {
    debug_msg("PASS Logic Read Addr %02X, Data %02X\r\n", addr2, rdata2);
    }
    skiptwi1wr:
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#endif

	////////////////
	endx:
	vPortFree(oammsg);
	vPortFree(oamresppkt);
	return ret;
	////////////////
}
//


int handle_showcomsettings(int argc, char **argv, uint8_t cmdkwlen, char *response_buf)
{
	//possibilities
	//showcomsettings
	//showcomsettings help
	//showcomsettings ?
	int c1 = 0;
	int c1x = 0;
	int selhelp = 0;
	int ret = RET_FAIL;
	uint16_t superlen,msglen ;
	char buf[20];
	int j;
	int flg;
	//
	//================================================================================
	uint8_t *oammsg = (uint8_t *) pvPortMalloc(sizeof(uint8_t) * MAX_RESPSTRING_SIZE);
	if (oammsg == NULL)
	{
		debug_msg("\r\nhandle_showccsettings: FAIL Alloc Mem - 'oammsg'\r\n");
		while (1)
			;	//stuck here
	}
	///////////////////////////////////////////////////////////////////////////
	VALIDRESP_T *oamresppkt = (VALIDRESP_T *) pvPortMalloc(sizeof(VALIDRESP_T));
	if(oamresppkt == NULL)
	{
		debug_msg("\r\nhandle_showccsettings: FAIL Alloc Mem - 'VALIDRESP_T'\r\n");
		while (1)
			;	//stuck here
	}
	//================================================================================
	///////////////////////////////
	///////////////////////////////
	//
	if (argc != 1 && argc != 2)
	{
		memset(oammsg,0,MAX_RESPSTRING_SIZE);
    	sprintf((char *)oammsg, "Incorrect Argument Count !\r\n");
        strcat((char *)oammsg, "SYNTAX ERROR\r\n@FAIL@\r\n");
		construct_oam_responsepkt(oammsg, oamresppkt);
#if DEBUG
        debug_msg((char *)oamresppkt->resppkt);
#endif
        memcpy(response_buf, oamresppkt->resppkt, oamresppkt->totallen );
	    goto endx;
	}
	//check...this could be 'help'
	selhelp = 0;
	if (argc == 1)
	{
		selhelp = 0;
	}
	else
	{
		if ((argc == 2) && (argv[1] == NULL))
		{
			memset(oammsg,0,MAX_RESPSTRING_SIZE);
			sprintf((char *)oammsg, "Execution Error !\r\n@FAIL@\r\n");
			construct_oam_responsepkt(oammsg, oamresppkt);
#if DEBUG
	       debug_msg((char *)oamresppkt->resppkt);
#endif
	       memcpy(response_buf, oamresppkt->resppkt, oamresppkt->totallen );
		   goto endx;
		}
		else
		{
			c1 = strncmp(argv[1], "help",strlen("help"));
			c1x = strncmp(argv[1], "?",1);
			if (c1 == 0 || c1x == 0)
			{
				selhelp = 1;
			}
			else
			{
				memset(oammsg,0,MAX_RESPSTRING_SIZE);
				sprintf((char *)oammsg, "Execution Error !\r\n@FAIL@\r\n");
				construct_oam_responsepkt(oammsg, oamresppkt);
#if DEBUG
		       debug_msg((char *)oamresppkt->resppkt);
#endif
		       memcpy(response_buf, oamresppkt->resppkt, oamresppkt->totallen );
			   goto endx;

			}
		}
	}
	//////////////////////////////////////////////////////////////////////
    if(selhelp != 0)
    {
		 construct_oam_large_template_responsepkt((uint8_t *)response_buf, RESPONSE_BUF_SIZE);
  	    strncat(response_buf, showcomsettings.help_str, strlen(showcomsettings.help_str));
    	strcat(response_buf, "\r\n");
        //
 		memset(buf,0,sizeof(buf));
        sprintf(buf, "Cmd String <%s> has no Arguments\r\n", showcomsettings.command_name);
        strcat(response_buf, buf);
        //
 		memset(buf,0,sizeof(buf));
        sprintf(buf, "E.G. %s <CR> \r\n", showcomsettings.command_name);
        strcat(response_buf, buf);
        //
		 strcat((char *)response_buf, "@OK@\r\n##");
        superlen = strlen(response_buf);
        msglen = superlen - 10;
        //
        /////////////////////
        sprintf(buf,"$$[%04d]",msglen);
        for(j=0;j<8;j++)
        {
        	response_buf[j] = buf[j];
        }
#if DEBUG
       debug_msg((char *)response_buf);
#endif
        /////////////////////
        ret = RET_SUCCESS;
        goto endx;
        //
    }
    //////////////////////////////////////////////////////////////////////
    //NORMAL EXECUTION
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//Construct VALID RESPONSE
	flg = construct_showcomsettings_response(response_buf);
	if(flg != 0)
	{
		goto endx;
	}
	ret = RET_SUCCESS;
	//////////////////////////////////////////////////////////////////////
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	////////////////
	endx:
	vPortFree(oammsg);
	vPortFree(oamresppkt);
	return ret;
	////////////////
}

extern LOGICCFG_T g_LOGIC_RDCFG;
int handle_showcomselftest(int argc, char **argv, uint8_t cmdkwlen, char *response_buf)
{
	//possibilities
	//showcomselftest
	//showcomselftest help
	//showcomselftest ?
	int c1 = 0;
	int c1x = 0;
	int selhelp = 0;
	int ret = RET_FAIL;
	uint16_t superlen,msglen ;
	char buf[20];
	int j;
	int flg;
	//
	//
	//================================================================================
	uint8_t *oammsg = (uint8_t *) pvPortMalloc(sizeof(uint8_t) * MAX_RESPSTRING_SIZE);
	if (oammsg == NULL)
	{
		debug_msg("\r\nhandle_showselftestcc: FAIL Alloc Mem - 'oammsg'\r\n");
		while (1)
			;	//stuck here
	}
	///////////////////////////////////////////////////////////////////////////
	VALIDRESP_T *oamresppkt = (VALIDRESP_T *) pvPortMalloc(sizeof(VALIDRESP_T));
	if(oamresppkt == NULL)
	{
		debug_msg("\r\nhandle_showselftestcc: FAIL Alloc Mem - 'VALIDRESP_T'\r\n");
		while (1)
			;	//stuck here
	}
	//================================================================================
	///////////////////////////////
	///////////////////////////////
	//
	if (argc != 1 && argc != 2)
	{
		memset(oammsg,0,MAX_RESPSTRING_SIZE);
    	sprintf((char *)oammsg, "Incorrect Argument Count !\r\n");
        strcat((char *)oammsg, "SYNTAX ERROR\r\n@FAIL@\r\n");
		construct_oam_responsepkt(oammsg, oamresppkt);
#if DEBUG
        debug_msg((char *)oamresppkt->resppkt);
#endif
        memcpy(response_buf, oamresppkt->resppkt, oamresppkt->totallen );
	    goto endx;
	}
	//check...this could be 'help'
	selhelp = 0;
	if (argc == 1)
	{
		selhelp = 0;
	}
	else
	{
		if ((argc == 2) && (argv[1] == NULL))
		{
			memset(oammsg,0,MAX_RESPSTRING_SIZE);
			sprintf((char *)oammsg, "Execution Error !\r\n@FAIL@\r\n");
			construct_oam_responsepkt(oammsg, oamresppkt);
#if DEBUG
	       debug_msg((char *)oamresppkt->resppkt);
#endif
	       memcpy(response_buf, oamresppkt->resppkt, oamresppkt->totallen );
		   goto endx;
		}
		else
		{
			c1 = strncmp(argv[1], "help",strlen("help"));
			c1x = strncmp(argv[1], "?",1);
			if (c1 == 0 || c1x == 0)
			{
				selhelp = 1;
			}
			else
			{
				memset(oammsg,0,MAX_RESPSTRING_SIZE);
				sprintf((char *)oammsg, "Execution Error !\r\n@FAIL@\r\n");
				construct_oam_responsepkt(oammsg, oamresppkt);
	#if DEBUG
		       debug_msg((char *)oamresppkt->resppkt);
	#endif
		       memcpy(response_buf, oamresppkt->resppkt, oamresppkt->totallen );
			   goto endx;
			}
		}
	}
	//////////////////////////////////////////////////////////////////////
    if(selhelp != 0)
    {
		construct_oam_large_template_responsepkt((uint8_t *)response_buf, RESPONSE_BUF_SIZE);
  	    strncat(response_buf, showcomselftest.help_str, strlen(showcomselftest.help_str));
    	strcat(response_buf, "\r\n");
        //
 		memset(buf,0,sizeof(buf));
        sprintf(buf, "Cmd String <%s> has no Arguments\r\n", showcomselftest.command_name);
        strcat(response_buf, buf);
        //
 		memset(buf,0,sizeof(buf));
        sprintf(buf, "E.G. %s  <CR> \r\n",showcomselftest.command_name);
        strcat(response_buf, buf);
        //
		strcat((char *)response_buf, "@OK@\r\n##");
        superlen = strlen(response_buf);
        msglen = superlen - 10;
        //
        /////////////////////
        sprintf(buf,"$$[%04d]",msglen);
        for(j=0;j<8;j++)
        {
        	response_buf[j] = buf[j];
        }
#if DEBUG
       debug_msg((char *)response_buf);
#endif
        /////////////////////
        ret = RET_SUCCESS;
        goto endx;
        //
    }
    //////////////////////////////////////////////////////////////////////
    //NORMAL EXECUTION
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//Construct VALID RESPONSE
	flg = construct_showcomselftest_response(response_buf);
	if(flg != 0)
	{
		goto endx;
	}
	ret = RET_SUCCESS;
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	////////////////
	endx:
	vPortFree(oammsg);
	vPortFree(oamresppkt);
	return ret;
	////////////////
}

int handle_showtripcounters(int argc, char **argv, uint8_t cmdkwlen, char *response_buf)
{
    //possibilities
    //showcomsettings
    //showcomsettings help
    //showcomsettings ?
    int c1 = 0;
    int c1x = 0;
    int selhelp = 0;
    int ret = RET_FAIL;
    uint16_t superlen,msglen ;
    char buf[20];
    int j;
    int flg;
    //
    //================================================================================
    uint8_t *oammsg = (uint8_t *) pvPortMalloc(sizeof(uint8_t) * MAX_RESPSTRING_SIZE);
    if (oammsg == NULL)
    {
        debug_msg("\r\nhandle_showccsettings: FAIL Alloc Mem - 'oammsg'\r\n");
        while (1)
            ;   //stuck here
    }
    ///////////////////////////////////////////////////////////////////////////
    VALIDRESP_T *oamresppkt = (VALIDRESP_T *) pvPortMalloc(sizeof(VALIDRESP_T));
    if(oamresppkt == NULL)
    {
        debug_msg("\r\nhandle_showccsettings: FAIL Alloc Mem - 'VALIDRESP_T'\r\n");
        while (1)
            ;   //stuck here
    }
    //================================================================================
    ///////////////////////////////
    ///////////////////////////////
    //
    if (argc != 1 && argc != 2)
    {
        memset(oammsg,0,MAX_RESPSTRING_SIZE);
        sprintf((char *)oammsg, "Incorrect Argument Count !\r\n");
        strcat((char *)oammsg, "SYNTAX ERROR\r\n@FAIL@\r\n");
        construct_oam_responsepkt(oammsg, oamresppkt);
#if DEBUG
        debug_msg((char *)oamresppkt->resppkt);
#endif
        memcpy(response_buf, oamresppkt->resppkt, oamresppkt->totallen );
        goto endx;
    }
    //check...this could be 'help'
    selhelp = 0;
    if (argc == 1)
    {
        selhelp = 0;
    }
    else
    {
        if ((argc == 2) && (argv[1] == NULL))
        {
            memset(oammsg,0,MAX_RESPSTRING_SIZE);
            sprintf((char *)oammsg, "Execution Error !\r\n@FAIL@\r\n");
            construct_oam_responsepkt(oammsg, oamresppkt);
#if DEBUG
           debug_msg((char *)oamresppkt->resppkt);
#endif
           memcpy(response_buf, oamresppkt->resppkt, oamresppkt->totallen );
           goto endx;
        }
        else
        {
            c1 = strncmp(argv[1], "help",strlen("help"));
            c1x = strncmp(argv[1], "?",1);
            if (c1 == 0 || c1x == 0)
            {
                selhelp = 1;
            }
            else
            {
                memset(oammsg,0,MAX_RESPSTRING_SIZE);
                sprintf((char *)oammsg, "Execution Error !\r\n@FAIL@\r\n");
                construct_oam_responsepkt(oammsg, oamresppkt);
#if DEBUG
               debug_msg((char *)oamresppkt->resppkt);
#endif
               memcpy(response_buf, oamresppkt->resppkt, oamresppkt->totallen );
               goto endx;

            }
        }
    }
    //////////////////////////////////////////////////////////////////////
    if(selhelp != 0)
    {
         construct_oam_large_template_responsepkt((uint8_t *)response_buf, RESPONSE_BUF_SIZE);
        strncat(response_buf, showtripcounters.help_str, strlen(showtripcounters.help_str));
        strcat(response_buf, "\r\n");
        //
        memset(buf,0,sizeof(buf));
        sprintf(buf, "Cmd String <%s> has no Arguments\r\n", showtripcounters.command_name);
        strcat(response_buf, buf);
        //
        memset(buf,0,sizeof(buf));
        sprintf(buf, "E.G. %s <CR> \r\n", showtripcounters.command_name);
        strcat(response_buf, buf);
        //
         strcat((char *)response_buf, "@OK@\r\n##");
        superlen = strlen(response_buf);
        msglen = superlen - 10;
        //
        /////////////////////
        sprintf(buf,"$$[%04d]",msglen);
        for(j=0;j<8;j++)
        {
            response_buf[j] = buf[j];
        }
#if DEBUG
       debug_msg((char *)response_buf);
#endif
        /////////////////////
        ret = RET_SUCCESS;
        goto endx;
        //
    }
    //////////////////////////////////////////////////////////////////////
    //NORMAL EXECUTION
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //Construct VALID RESPONSE
    flg = construct_showtripcounters_response(response_buf);
    if(flg != 0)
    {
        goto endx;
    }
    ret = RET_SUCCESS;
    //////////////////////////////////////////////////////////////////////
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    ////////////////
    endx:
    vPortFree(oammsg);
    vPortFree(oamresppkt);
    return ret;
    ////////////////
}

int handle_showalarmrelay(int argc, char **argv, uint8_t cmdkwlen, char *response_buf)
{
    //possibilities
    //showalarmrelay
    //showalarmrelay help
    //showalarmrelay ?
    int c1 = 0;
    int c1x = 0;
    int selhelp = 0;
    int ret = RET_FAIL;
    uint16_t superlen,msglen ;
    char buf[20];
    int j;
    int flg;
    //
    //================================================================================
    uint8_t *oammsg = (uint8_t *) pvPortMalloc(sizeof(uint8_t) * MAX_RESPSTRING_SIZE);
    if (oammsg == NULL)
    {
        debug_msg("\r\nhandle_showccsettings: FAIL Alloc Mem - 'oammsg'\r\n");
        while (1)
            ;   //stuck here
    }
    ///////////////////////////////////////////////////////////////////////////
    VALIDRESP_T *oamresppkt = (VALIDRESP_T *) pvPortMalloc(sizeof(VALIDRESP_T));
    if(oamresppkt == NULL)
    {
        debug_msg("\r\nhandle_showccsettings: FAIL Alloc Mem - 'VALIDRESP_T'\r\n");
        while (1)
            ;   //stuck here
    }
    //================================================================================
    ///////////////////////////////
    ///////////////////////////////
    //
    if (argc != 1 && argc != 2)
    {
        memset(oammsg,0,MAX_RESPSTRING_SIZE);
        sprintf((char *)oammsg, "Incorrect Argument Count !\r\n");
        strcat((char *)oammsg, "SYNTAX ERROR\r\n@FAIL@\r\n");
        construct_oam_responsepkt(oammsg, oamresppkt);
#if DEBUG
        debug_msg((char *)oamresppkt->resppkt);
#endif
        memcpy(response_buf, oamresppkt->resppkt, oamresppkt->totallen );
        goto endx;
    }
    //check...this could be 'help'
    selhelp = 0;
    if (argc == 1)
    {
        selhelp = 0;
    }
    else
    {
        if ((argc == 2) && (argv[1] == NULL))
        {
            memset(oammsg,0,MAX_RESPSTRING_SIZE);
            sprintf((char *)oammsg, "Execution Error !\r\n@FAIL@\r\n");
            construct_oam_responsepkt(oammsg, oamresppkt);
#if DEBUG
           debug_msg((char *)oamresppkt->resppkt);
#endif
           memcpy(response_buf, oamresppkt->resppkt, oamresppkt->totallen );
           goto endx;
        }
        else
        {
            c1 = strncmp(argv[1], "help",strlen("help"));
            c1x = strncmp(argv[1], "?",1);
            if (c1 == 0 || c1x == 0)
            {
                selhelp = 1;
            }
            else
            {
                memset(oammsg,0,MAX_RESPSTRING_SIZE);
                sprintf((char *)oammsg, "Execution Error !\r\n@FAIL@\r\n");
                construct_oam_responsepkt(oammsg, oamresppkt);
#if DEBUG
               debug_msg((char *)oamresppkt->resppkt);
#endif
               memcpy(response_buf, oamresppkt->resppkt, oamresppkt->totallen );
               goto endx;

            }
        }
    }
    //////////////////////////////////////////////////////////////////////
    if(selhelp != 0)
    {
         construct_oam_large_template_responsepkt((uint8_t *)response_buf, RESPONSE_BUF_SIZE);
        strncat(response_buf, showalarmrelay.help_str, strlen(showalarmrelay.help_str));
        strcat(response_buf, "\r\n");
        //
        memset(buf,0,sizeof(buf));
        sprintf(buf, "Cmd String <%s> has no Arguments\r\n", showalarmrelay.command_name);
        strcat(response_buf, buf);
        //
        memset(buf,0,sizeof(buf));
        sprintf(buf, "E.G. %s <CR> \r\n", showalarmrelay.command_name);
        strcat(response_buf, buf);
        //
         strcat((char *)response_buf, "@OK@\r\n##");
        superlen = strlen(response_buf);
        msglen = superlen - 10;
        //
        /////////////////////
        sprintf(buf,"$$[%04d]",msglen);
        for(j=0;j<8;j++)
        {
            response_buf[j] = buf[j];
        }
#if DEBUG
       debug_msg((char *)response_buf);
#endif
        /////////////////////
        ret = RET_SUCCESS;
        goto endx;
        //
    }
    //////////////////////////////////////////////////////////////////////
    //NORMAL EXECUTION
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //Construct VALID RESPONSE
    flg = construct_showalarmrelay_response(response_buf);
    if(flg != 0)
    {
        goto endx;
    }
    ret = RET_SUCCESS;
    //////////////////////////////////////////////////////////////////////
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    ////////////////
    endx:
    vPortFree(oammsg);
    vPortFree(oamresppkt);
    return ret;
    ////////////////
}




static int construct_showcomversion_response(char *response_buf)
{
	int ret = RET_FAIL;
	/////////////////////////////////////////////////////////////////
	uint8_t title[MAX_TITLE_SIZE + 1];
	uint8_t cardserial[MAX_SERIAL_SIZE + 1];
	uint32_t validcardserial;
	uint8_t carddesc[MAX_SERIAL_SIZE + 1];
	uint32_t validcarddesc;
	uint32_t isvalid;
	/////////////////////////////////////////////////////////////////
	uint16_t superlen,msglen ;
	char buf[100];
	int j;
	//////////////////////////////////////////////////////////////////////
	//
	//////////////////////////////////////////////////////////////////////////
	BOOTSTATUS_T *bootstatus = (BOOTSTATUS_T *) pvPortMalloc(sizeof(BOOTSTATUS_T));
	if(bootstatus == NULL)
	{
		debug_msg("\r\nFAIL Alloc Mem - 'BOOTSTATUS_T'\r\n");
		while (1)
			;	//stuck here
	}
	//////////////////////////////////////////////////////////////////////
	copy_bootstatus_mutex(bootstatus);
	//////////////////////////////////////////////////////////////////////
	//
	//Construct VALID RESPONSE
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	construct_oam_large_template_responsepkt((uint8_t *)response_buf, RESPONSE_BUF_SIZE);
	///////////////////////////////////////////////////////////////
	copy_oamcfgdbase_title((uint8_t *)title);
	copy_serialdbase_cardserial((uint8_t *)&cardserial[0]);
	copy_serialdbase_validcardserial((uint8_t *)&validcardserial);
	copy_serialdbase_carddesc((uint8_t *)&carddesc[0]);
	copy_serialdbase_validcarddesc((uint8_t *)&validcarddesc);
	copy_serialdbase_isvalid((uint8_t *)&isvalid);
	//////////////////////////////////////////////////////////////////////////
	//DISPLAY
	strcat((char *)response_buf, "\r\n");
	sprintf((char *)response_buf, "Card Details...\r\n");
	//===================================================================
	///////////////////////////////////////////////////////////////
	strcat((char *)response_buf, "System        : ");
	strcat((char *)response_buf,(char *)CFG_SYSTYPE);
	strcat((char *)response_buf, "\r\n");
	///////////////////////////////////////////////////////////////
	strcat((char *)response_buf, "FW Version    : ");
	strcat((char *)response_buf,(char *)CFG_FWVER);
	strcat((char *)response_buf, "\r\n");
	///////////////////////////////////////////////////////////////
	strcat((char *)response_buf, "Firmware Desc : ");
	strcat((char *)response_buf,(char *)CFG_DESC01);
	strcat((char *)response_buf, "\r\n");
	//-------------------------------------------------------------
    strcat((char *)response_buf, "                ");
    strcat((char *)response_buf,(char *)CFG_DESC02);
    strcat((char *)response_buf, "\r\n");
    //-------------------------------------------------------------
    strcat((char *)response_buf, "                ");
    strcat((char *)response_buf,(char *)CFG_DESC03);
    strcat((char *)response_buf, "\r\n");
    //-------------------------------------------------------------
    strcat((char *)response_buf, "                ");
    strcat((char *)response_buf,(char *)CFG_DESC04);
    strcat((char *)response_buf, "\r\n");
    //-------------------------------------------------------------
    strcat((char *)response_buf, "                ");
    strcat((char *)response_buf,(char *)CFG_DESC05);
    strcat((char *)response_buf, "\r\n");
    //-------------------------------------------------------------


	///////////////////////////////////////////////////////////////
	strcat((char *)response_buf, "Card Title    : ");
	strcat((char *)response_buf,(char *)title);
	strcat((char *)response_buf, "\r\n");
	//////////////////////////////////////////////////////////////
	strcat((char *)response_buf, "Description   : ");
	if(validcarddesc == VALIDDESC)
	{
		strcat((char *)response_buf,(char *)carddesc);
	}
	else
	{
		strcat((char *)response_buf, "Not Valid !");
	}
	strcat((char *)response_buf, "\r\n");
	///////////////////////////////////////////////////////////////
	strcat((char *)response_buf, "Serial Number : ");
	if(validcardserial == VALIDSERNO)
	{
		strcat((char *)response_buf,(char *)cardserial);
	}
	else
	{
		strcat((char *)response_buf, "Not Valid !");
	}
	strcat((char *)response_buf, "\r\n");
	///////////////////////////////////////////////////////////////
	if(bootstatus->boot_done == 0)
	{
		strcat((char *)response_buf, "Boot-UP Pending !\r\n");
		//
		goto finished;
	}
	///////////////////////////////////////////////////////////////
	strcat((char *)response_buf, "LOGIC UID     : ");
	memset(buf,0,sizeof(buf));
	if(bootstatus->result[3] == BOOTRESULT_PASS)
	{
	    sprintf(buf," %02X %02X %02X %02X %02X\r\n",
			bootstatus->logicparams.uid[0], bootstatus->logicparams.uid[1],
			bootstatus->logicparams.uid[2], bootstatus->logicparams.uid[3],
			bootstatus->logicparams.uid[4]);
	    strcat((char *)response_buf,(char *)buf);
	    ///////////////////////////////////////////////////////////////
	    strcat((char *)response_buf, "LOGIC Release : ");
	    memset(buf,0,sizeof(buf));
	    sprintf(buf,"[DD-MM-YYYY] %02X-%02X-%02X%02X\r\n",
			bootstatus->logicparams.logicrelease[0], bootstatus->logicparams.logicrelease[1],
			bootstatus->logicparams.logicrelease[2], bootstatus->logicparams.logicrelease[3]);
	    strcat((char *)response_buf,(char *)buf);
	    ///////////////////////////////////////////////////////////////
	    strcat((char *)response_buf, "                ");
	    memset(buf,0,sizeof(buf));
	    sprintf(buf,"[HH-MM] %02X:%02X\r\n",
            bootstatus->logicparams.logictime[0], bootstatus->logicparams.logictime[1]);
	    strcat((char *)response_buf,(char *)buf);
    }
    else
    {
        sprintf(buf,"Not Available !\r\n");
        strcat((char *)response_buf,(char *)buf);
    }
	finished:
	//
	ret = RET_SUCCESS;
	/////////////////////////
	if(ret == RET_SUCCESS)
		strcat((char *)response_buf, "@OK@\r\n##");
	else
		strcat((char *)response_buf, "@FAIL@\r\n##");
    superlen = strlen(response_buf);
    msglen = superlen - 10;
    //
    /////////////////////
    memset(buf,0,sizeof(buf));
    sprintf(buf,"$$[%04d]",msglen);
    for(j=0;j<8;j++)
    {
    	response_buf[j] = buf[j];
    }
    /////////////////////
#if DEBUG
	debug_msg("Sent Response to OAM...\r\n");
	debug_msg((char *)response_buf);
	ajUART0crlf();
#endif
	//
	/////////////////////
	vPortFree(bootstatus);
	return ret;
	/////////////////////
}


static int construct_showcomsettings_response(char *response_buf)
{
	int ret = RET_SUCCESS;
	uint16_t superlen,msglen ;
	char buf[100];
	char source[MAX_TABLEENTRY_BUF];
	char dest[MAX_TABLEENTRY_BUF];
	int j;
	//==================================================================
	//CC Setting variables
	uint8_t title[MAX_TITLE_SIZE + 1];
	uint8_t cardserial[MAX_SERIAL_SIZE + 1];
	uint32_t validcardserial;
	uint8_t carddesc[MAX_SERIAL_SIZE + 1];
	uint32_t validcarddesc;
	uint32_t isvalidser;
	//==================================================================
	uint32_t isvalidcfg;
	uint8_t glitchfilter[MAX_INCHANS]; //4
	uint8_t outputpulse[MAX_OUTCHANS]; //16
	uint8_t outputsource[MAX_OUTCHANS]; //16
	///////////////////////////////////////////////////////////////
	copy_oamcfgdbase_title((uint8_t *)title);
	copy_serialdbase_carddesc((uint8_t *)&carddesc[0]);
	copy_serialdbase_validcarddesc((uint8_t *)&validcarddesc);
	copy_serialdbase_cardserial((uint8_t *)&cardserial[0]);
	copy_serialdbase_validcardserial((uint8_t *)&validcardserial);
	copy_serialdbase_isvalid((uint8_t *)&isvalidser);
	///////////////////////////////////////////////////////////////
	copy_oamcfgdbase_isvalid((uint8_t *)&isvalidcfg);
	copy_oamcfgdbase_glitchfilter((uint8_t *)&glitchfilter[0]);
	copy_oamcfgdbase_outputpulse((uint8_t *)&outputpulse[0]);
	///////////////////////////////////////////////////////////////
    copy_oamcfgdbase_outputsource((uint8_t *)&outputsource[0]);
    ///////////////////////////////////////////////////////////////
	//==================================================================
	//////////////////////////////////////////////////////////////////////
	//Construct VALID RESPONSE
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	construct_oam_large_template_responsepkt((uint8_t *)response_buf, RESPONSE_BUF_SIZE);
	///////////////////////////////////////////////////////////////
	strcat((char *)response_buf, "\r\n");
	sprintf((char *)response_buf, "Card Settings...\r\n");
    //===================================================================
    ///////////////////////////////////////////////////////////////
    strcat((char *)response_buf, "System        : ");
    strcat((char *)response_buf,(char *)CFG_SYSTYPE);
    strcat((char *)response_buf, "\r\n");
    ///////////////////////////////////////////////////////////////
    strcat((char *)response_buf, "FW Version    : ");
    strcat((char *)response_buf,(char *)CFG_FWVER);
    strcat((char *)response_buf, "\r\n");
    ///////////////////////////////////////////////////////////////
    strcat((char *)response_buf, "Firmware Desc : ");
    strcat((char *)response_buf,(char *)CFG_DESC01);
    strcat((char *)response_buf, "\r\n");
    //-------------------------------------------------------------
    strcat((char *)response_buf, "                ");
    strcat((char *)response_buf,(char *)CFG_DESC02);
    strcat((char *)response_buf, "\r\n");
    //-------------------------------------------------------------
    strcat((char *)response_buf, "                ");
    strcat((char *)response_buf,(char *)CFG_DESC03);
    strcat((char *)response_buf, "\r\n");
    //-------------------------------------------------------------
    strcat((char *)response_buf, "                ");
    strcat((char *)response_buf,(char *)CFG_DESC04);
    strcat((char *)response_buf, "\r\n");
    //-------------------------------------------------------------
    strcat((char *)response_buf, "                ");
    strcat((char *)response_buf,(char *)CFG_DESC05);
    strcat((char *)response_buf, "\r\n");
    //-------------------------------------------------------------
	//===================================================================
    if(isvalidcfg != VALIDDBASE)
    {
        strcat((char *)response_buf, "Config Not Valid !\r\n");
        ret = RET_FAIL;
        goto endx;
    }
	///////////////////////////////////////////////////////////////
	strcat((char *)response_buf, "Card Title    : ");
	strcat((char *)response_buf,(char *)title);
	strcat((char *)response_buf, "\r\n");
	//////////////////////////////////////////////////////////////
	strcat((char *)response_buf, "Description   : ");
	if(validcarddesc == VALIDDESC)
	{
		strcat((char *)response_buf,(char *)carddesc);
	}
	else
	{
		strcat((char *)response_buf, "Not Valid !");
	}
	strcat((char *)response_buf, "\r\n");
	///////////////////////////////////////////////////////////////
	strcat((char *)response_buf, "Serial Number : ");
	if(validcardserial == VALIDSERNO)
	{
		strcat((char *)response_buf,(char *)cardserial);
	}
	else
	{
		strcat((char *)response_buf, "Not Valid !");
	}
	strcat((char *)response_buf, "\r\n");
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//show glitch filter settings
	strcat((char *)response_buf, "\r\n");
	strcat((char *)response_buf, "GLITCH FILTER CONFIGURATION:\r\n");
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//CHAN NO 1-4
	memset(source,0,sizeof(source));
	memset(dest,0,sizeof(dest));
	sprintf((char *)source,"CHANNEL NO     ");
	strcat((char *)response_buf, source);
	for(j = 0;j < 4;j++)
	{
		//add chan no
		memset(source,0,sizeof(source));
		memset(dest,0,sizeof(dest));
		sprintf((char *)source,"%02d", j + 1);
		add_spaces_tostr(source,dest,6);
		strcat((char *)response_buf, dest);
	}
	strcat((char *)response_buf, "\r\n");
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//
	//GLITCHFILTER SETTING 1-4
	memset(source,0,sizeof(source));
	memset(dest,0,sizeof(dest));
	sprintf((char *)source,"GLITCH FILTER  ");
	strcat((char *)response_buf, source);
	for(j = 0;j < 4;j++)
	{
		//add setting
		memset(source,0,sizeof(source));
		memset(dest,0,sizeof(dest));
		///////////////////
		memset(buf,0,sizeof(buf));
		get_glitchfilterstr(glitchfilter[j], &buf[0], sizeof(buf));
		sprintf((char *)source,"%s",buf);
		///////////////////
		add_spaces_tostr(source,dest,6);
		strcat((char *)response_buf, dest);
	}
	strcat((char *)response_buf, "\r\n");
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//show outputpulse settings
	strcat((char *)response_buf, "\r\n");
	strcat((char *)response_buf, "OUTPUT PULSE WIDTH CONFIGURATION:\r\n");
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//CHAN NO 1-8
	memset(source,0,sizeof(source));
	memset(dest,0,sizeof(dest));
	sprintf((char *)source,"CHANNEL NO     ");
	strcat((char *)response_buf, source);
	for(j = 0;j < 8;j++)
	{
		//add chan no
		memset(source,0,sizeof(source));
		memset(dest,0,sizeof(dest));
		sprintf((char *)source,"%02d", j + 1);
		add_spaces_tostr(source,dest,7);
		strcat((char *)response_buf, dest);
	}
	strcat((char *)response_buf, "\r\n");
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//
	//OUTPUT PW SETTING 1-8
	memset(source,0,sizeof(source));
	memset(dest,0,sizeof(dest));
	sprintf((char *)source,"OUTPUT PULSE   ");
	strcat((char *)response_buf, source);
	for(j = 0;j < 8;j++)
	{
		//add setting
		memset(source,0,sizeof(source));
		memset(dest,0,sizeof(dest));
		///////////////////
		memset(buf,0,sizeof(buf));
		sprintf((char *)source,"%s", get_outputpulsestr(outputpulse[j]));
		///////////////////
		add_spaces_tostr(source,dest,7);
		strcat((char *)response_buf, dest);
	}
	strcat((char *)response_buf, "\r\n");
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//CHAN NO 9 - 16
	memset(source,0,sizeof(source));
	memset(dest,0,sizeof(dest));
	sprintf((char *)source,"CHANNEL NO     ");
	strcat((char *)response_buf, source);
	for(j = 8;j < 16;j++)
	{
		//add chan no
		memset(source,0,sizeof(source));
		memset(dest,0,sizeof(dest));
		sprintf((char *)source,"%02d", j + 1);
		add_spaces_tostr(source,dest,7);
		strcat((char *)response_buf, dest);
	}
	strcat((char *)response_buf, "\r\n");
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//
	//OUTPUT PW SETTING 9-16
	memset(source,0,sizeof(source));
	memset(dest,0,sizeof(dest));
	sprintf((char *)source,"OUTPUT PULSE   ");
	strcat((char *)response_buf, source);
	for(j = 8;j < 16;j++)
	{
		//add setting
		memset(source,0,sizeof(source));
		memset(dest,0,sizeof(dest));
		///////////////////
		memset(buf,0,sizeof(buf));
		sprintf((char *)source,"%s",get_outputpulsestr(outputpulse[j]) );
		///////////////////
		add_spaces_tostr(source,dest,7);
		strcat((char *)response_buf, dest);
	}
	strcat((char *)response_buf, "\r\n");
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//
    //
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    strcat((char *)response_buf, "\r\n");
    strcat((char *)response_buf, "OUTPUT SOURCE MAPPING CONFIGURATION:\r\n");
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //CHAN NO 1-8
    memset(source,0,sizeof(source));
    memset(dest,0,sizeof(dest));
    sprintf((char *)source,"CHANNEL NO     ");
    strcat((char *)response_buf, source);
    for(j = 0;j < 8;j++)
    {
        //add chan no
        memset(source,0,sizeof(source));
        memset(dest,0,sizeof(dest));
        sprintf((char *)source,"%02d", j + 1);
        add_spaces_tostr(source,dest,7);
        strcat((char *)response_buf, dest);
    }
    strcat((char *)response_buf, "\r\n");
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //
    //OUTPUT SOURCE SETTING 1-8
    memset(source,0,sizeof(source));
    memset(dest,0,sizeof(dest));
    sprintf((char *)source,"OUTPUT SOURCE  ");
    strcat((char *)response_buf, source);
    for(j = 0;j < 8;j++)
    {
        //add setting
        memset(source,0,sizeof(source));
        memset(dest,0,sizeof(dest));
        ///////////////////
        memset(buf,0,sizeof(buf));
        sprintf((char *)source,"%d", outputsource[j] );
        ///////////////////
        add_spaces_tostr(source,dest,7);
        strcat((char *)response_buf, dest);
    }
    strcat((char *)response_buf, "\r\n");
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //CHAN NO 9 - 16
    memset(source,0,sizeof(source));
    memset(dest,0,sizeof(dest));
    sprintf((char *)source,"CHANNEL NO     ");
    strcat((char *)response_buf, source);
    for(j = 8;j < 16;j++)
    {
        //add chan no
        memset(source,0,sizeof(source));
        memset(dest,0,sizeof(dest));
        sprintf((char *)source,"%02d", j + 1);
        add_spaces_tostr(source,dest,7);
        strcat((char *)response_buf, dest);
    }
    strcat((char *)response_buf, "\r\n");
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //
    //OUTPUT PW SETTING 9-16
    memset(source,0,sizeof(source));
    memset(dest,0,sizeof(dest));
    sprintf((char *)source,"OUTPUT SOURCE  ");
    strcat((char *)response_buf, source);
    for(j = 8;j < 16;j++)
    {
        //add setting
        memset(source,0,sizeof(source));
        memset(dest,0,sizeof(dest));
        ///////////////////
        memset(buf,0,sizeof(buf));
        sprintf((char *)source,"%d", outputsource[j] );
        ///////////////////
        add_spaces_tostr(source,dest,7);
        strcat((char *)response_buf, dest);
    }
    strcat((char *)response_buf, "\r\n");
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	///////////////////////////////////////////////////////////////
    endx:
	if(ret == RET_SUCCESS)
		strcat((char *)response_buf, "@OK@\r\n##");
	else
		strcat((char *)response_buf, "@FAIL@\r\n##");
    superlen = strlen(response_buf);
    msglen = superlen - 10;
    //
    /////////////////////
    memset(buf,0,sizeof(buf));
    sprintf(buf,"$$[%04d]",msglen);
    for(j=0;j<8;j++)
    {
    	response_buf[j] = buf[j];
    }
    /////////////////////
#if DEBUG
	debug_msg("Sent Response to OAM...\r\n");
#endif
	//
	/////////////////////
	return ret;
	/////////////////////
}



static int construct_showcomselftest_response(char *response_buf)
{
	int ret = RET_SUCCESS;
	uint8_t title[24];
	uint8_t cardserial[24];
	uint32_t validcardserial;
	uint8_t carddesc[24];
	uint32_t validcarddesc;
	uint16_t superlen,msglen ;
	char buf[100];
	char buffs[50];
	char buf1[50];
	char buf2[50];

	int j;
	//
	//////////////////////////////////////////////////////////////////////////
	BOOTSTATUS_T *bootstatus = (BOOTSTATUS_T *) pvPortMalloc(sizeof(BOOTSTATUS_T));
	if(bootstatus == NULL)
	{
		debug_msg("\r\nFAIL Alloc Mem - 'BOOTSTATUS_T'\r\n");
		while (1)
			;	//stuck here
	}
	//////////////////////////////////////////////////////////////////////
	copy_bootstatus_mutex(bootstatus);
	//////////////////////////////////////////////////////////////////////
	//Construct VALID RESPONSE
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	construct_oam_large_template_responsepkt((uint8_t *)response_buf, RESPONSE_BUF_SIZE);
	///////////////////////////////////////////////////////////////
	copy_oamcfgdbase_title((uint8_t *)title);
	copy_serialdbase_carddesc((uint8_t *)&carddesc[0]);
	copy_serialdbase_validcarddesc((uint8_t *)&validcarddesc);
	copy_serialdbase_cardserial((uint8_t *)&cardserial[0]);
	copy_serialdbase_validcardserial((uint8_t *)&validcardserial);
	///////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////
	strcat((char *)response_buf, "\r\n");
	strcat((char *)response_buf, "Card SELFTEST Result...\r\n");
   //===================================================================
    ///////////////////////////////////////////////////////////////
    strcat((char *)response_buf, "System        : ");
    strcat((char *)response_buf,(char *)CFG_SYSTYPE);
    strcat((char *)response_buf, "\r\n");
    ///////////////////////////////////////////////////////////////
    strcat((char *)response_buf, "FW Version    : ");
    strcat((char *)response_buf,(char *)CFG_FWVER);
    strcat((char *)response_buf, "\r\n");
    ///////////////////////////////////////////////////////////////
    strcat((char *)response_buf, "Firmware Desc : ");
    strcat((char *)response_buf,(char *)CFG_DESC01);
    strcat((char *)response_buf, "\r\n");
    //-------------------------------------------------------------
    strcat((char *)response_buf, "                ");
    strcat((char *)response_buf,(char *)CFG_DESC02);
    strcat((char *)response_buf, "\r\n");
    //-------------------------------------------------------------
    strcat((char *)response_buf, "                ");
    strcat((char *)response_buf,(char *)CFG_DESC03);
    strcat((char *)response_buf, "\r\n");
    //-------------------------------------------------------------
    strcat((char *)response_buf, "                ");
    strcat((char *)response_buf,(char *)CFG_DESC04);
    strcat((char *)response_buf, "\r\n");
    //-------------------------------------------------------------
    strcat((char *)response_buf, "                ");
    strcat((char *)response_buf,(char *)CFG_DESC05);
    strcat((char *)response_buf, "\r\n");
    //-------------------------------------------------------------
   //===================================================================
	///////////////////////////////////////////////////////////////
	strcat((char *)response_buf, "Card Title    : ");
	strcat((char *)response_buf,(char *)title);
	strcat((char *)response_buf, "\r\n");
	///////////////////////////////////////////////////////////////
	//debug_msg("validcarddesc [%04X], validcardserial [%04X]\r\n", validcarddesc, validcardserial);
	strcat((char *)response_buf, "Description   : ");
	if(validcarddesc == VALIDDESC)
	{
		strcat((char *)response_buf,(char *)carddesc);
	}
	else
	{
		strcat((char *)response_buf, "Not Valid !");
	}
	strcat((char *)response_buf, "\r\n");
	//////////////////////////////////////////////////////////////
	strcat((char *)response_buf, "Serial Number : ");
	if(validcardserial == VALIDSERNO)
	{
		strcat((char *)response_buf,(char *)cardserial);
	}
	else
	{
		strcat((char *)response_buf, "Not Valid !");
	}
	strcat((char *)response_buf, "\r\n");
	//
	///////////////////////////////////////////////////////////////
	if(bootstatus->boot_done == 0)
	{
		strcat((char *)response_buf, "Boot-UP Pending !\r\n");
		//
		goto finished;
	}
	//////////////////////////////////////////////////////////////////////
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	for(j = 0;j < BOOTSTATUS_STEPS;j++)
	{
		strcat((char *)response_buf, "BOOT STEP ");
		//
		memset(buf,0,sizeof(buf));
		memset(buf1,0,sizeof(buf1));
		memset(buf2,0,sizeof(buf2));
		//debug_msg(" step %d result %d\r\n", (j+1), bootstatus->result[j]);
		sprintf(buf, "-%d %s %s",(j+1), getbootstepstr(j + 1), getbootresultstr(bootstatus->result[j]));
		strcat((char *)response_buf, buf);//main msg
		if(bootstatus->result[j] == BOOTRESULT_FAIL ||
				bootstatus->result[j] == BOOTRESULT_CRITICAL	)
		{
			sprintf(buffs, "  FAIL Step-%d\r\n",bootstatus->fail_step[j]);
			strcat((char *)response_buf, buffs);
		}
		else
		{
			if(j == 4) //Step-5, logic
			{
				construct_logicparams(&bootstatus->logicparams, buf1, buf2);
				debug_msg("%s\r\n%s\r\n", buf1,buf2 );
				strcat((char *)response_buf, buf1);
				if(buf2[0] != 0)
				{
					strcat((char *)response_buf, buf2);
				}
			}
			else
			{
				sprintf(buf1, "\r\n");
				strcat((char *)response_buf, buf1);
			}
		}
		/////////////////////////////////////////////////////////
		//
		if(j == 4) //Step-5, logic
		{
			strcat((char *)response_buf, "\r\n\r\n");
		}
		//
	}
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//////////////////////////////////////////////////////////////
	strcat((char *)response_buf, "OVERALL Result :");
	//
	memset(buf,0,sizeof(buf));
	sprintf(buf, "%s\r\n",getbootresultstr(bootstatus->overallresult));
	strcat((char *)response_buf, buf);
	//////////////////////////////////////////////////////////////
	//
	finished:
	//
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	strcat((char *)response_buf, "\r\n");
	///////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////
	if(ret == RET_SUCCESS)
		strcat((char *)response_buf, "@OK@\r\n##");
	else
		strcat((char *)response_buf, "@FAIL@\r\n##");
    superlen = strlen(response_buf);
    msglen = superlen - 10;
    //
    /////////////////////
    memset(buf,0,sizeof(buf));
    sprintf(buf,"$$[%04d]",msglen);
    for(j=0;j<8;j++)
    {
    	response_buf[j] = buf[j];
    }
    /////////////////////
#if DEBUG
	debug_msg("Sent Response to OAM...\r\n");
	debug_msg((char *)response_buf);
	ajUART0crlf();
#endif
	//
	/////////////////////
	vPortFree(bootstatus);
	return ret;
	/////////////////////
}

static int construct_showtripcounters_response(char *response_buf)
{
    int ret = RET_SUCCESS;
    uint16_t superlen,msglen ;
    char buf[100];
    char source[MAX_TABLEENTRY_BUF];
    char dest[MAX_TABLEENTRY_BUF];
    int j;
    //==================================================================
    //==================================================================
    uint32_t isvalidcfg;
    uint8_t outputsource[MAX_OUTCHANS]; //16
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
    copy_tripcntrs_mutex(tripcntrs);
    //////////////////////////////////////////////////////////////////////
    //Construct VALID RESPONSE
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    construct_oam_large_template_responsepkt((uint8_t *)response_buf, RESPONSE_BUF_SIZE);
    ///////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////
    copy_oamcfgdbase_isvalid((uint8_t *)&isvalidcfg);
    ///////////////////////////////////////////////////////////////
    copy_oamcfgdbase_outputsource((uint8_t *)&outputsource[0]);
    ///////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////
    strcat((char *)response_buf, "\r\n");
    strcat((char *)response_buf, "TRIP COUNTERS...\r\n");
    //===================================================================
    if(isvalidcfg != VALIDDBASE)
    {
        strcat((char *)response_buf, "Config Not Valid !\r\n");
        ret = RET_FAIL;
        goto endx;
    }
    //===================================================================
    //INPUT CHAN NO 1-4
    memset(source,0,sizeof(source));
    memset(dest,0,sizeof(dest));
    sprintf((char *)source,"INPUT NO       ");
    strcat((char *)response_buf, source);
    for(j = 0;j < MAX_INCHANS;j++)
    {
        //add chan no
        memset(source,0,sizeof(source));
        memset(dest,0,sizeof(dest));
        sprintf((char *)source,"%02d", j + 1);
        add_spaces_tostr(source,dest,7);
        strcat((char *)response_buf, dest);
    }
    strcat((char *)response_buf, "\r\n");
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //TRIP COUNTERS
    memset(source,0,sizeof(source));
    memset(dest,0,sizeof(dest));
    sprintf((char *)source,"INPUT TRIPS    ");
    strcat((char *)response_buf, source);
    for(j = 0;j < MAX_INCHANS;j++)
    {
        //add setting
        memset(source,0,sizeof(source));
        memset(dest,0,sizeof(dest));
        ///////////////////
        memset(buf,0,sizeof(buf));
        sprintf((char *)source,"%d", tripcntrs->inputcnt[j] );
        ///////////////////
        add_spaces_tostr(source,dest,7);
        strcat((char *)response_buf, dest);
    }
    strcat((char *)response_buf, "\r\n");
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++



    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    strcat((char *)response_buf, "\r\n");
    strcat((char *)response_buf, "OUTPUT SOURCE & TRIP COUNTS:\r\n");
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //CHAN NO 1-8
    memset(source,0,sizeof(source));
    memset(dest,0,sizeof(dest));
    sprintf((char *)source,"OUTPUT NO      ");
    strcat((char *)response_buf, source);
    for(j = 0;j < 8;j++)
    {
        //add chan no
        memset(source,0,sizeof(source));
        memset(dest,0,sizeof(dest));
        sprintf((char *)source,"%02d", j + 1);
        add_spaces_tostr(source,dest,7);
        strcat((char *)response_buf, dest);
    }
    strcat((char *)response_buf, "\r\n");
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //OUTPUT SOURCE SETTING 1-8
    memset(source,0,sizeof(source));
    memset(dest,0,sizeof(dest));
    sprintf((char *)source,"OUTPUT SOURCE  ");
    strcat((char *)response_buf, source);
    for(j = 0;j < 8;j++)
    {
        //add setting
        memset(source,0,sizeof(source));
        memset(dest,0,sizeof(dest));
        ///////////////////
        memset(buf,0,sizeof(buf));
        sprintf((char *)source,"%d", outputsource[j] );
        ///////////////////
        add_spaces_tostr(source,dest,7);
        strcat((char *)response_buf, dest);
    }
    strcat((char *)response_buf, "\r\n");
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //OUTPUT TRIP COUNTS 1-8
    memset(source,0,sizeof(source));
    memset(dest,0,sizeof(dest));
    sprintf((char *)source,"OUTPUT TRIPS   ");
    strcat((char *)response_buf, source);
    for(j = 0;j < 8;j++)
    {
        //add setting
        memset(source,0,sizeof(source));
        memset(dest,0,sizeof(dest));
        ///////////////////
        memset(buf,0,sizeof(buf));
        sprintf((char *)source,"%d", tripcntrs->outputcnt[j] );
        ///////////////////
        add_spaces_tostr(source,dest,7);
        strcat((char *)response_buf, dest);
    }
    strcat((char *)response_buf, "\r\n");
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //CHAN NO 9 - 16
    memset(source,0,sizeof(source));
    memset(dest,0,sizeof(dest));
    sprintf((char *)source,"OUTPUT NO      ");
    strcat((char *)response_buf, source);
    for(j = 8;j < 16;j++)
    {
        //add chan no
        memset(source,0,sizeof(source));
        memset(dest,0,sizeof(dest));
        sprintf((char *)source,"%02d", j + 1);
        add_spaces_tostr(source,dest,7);
        strcat((char *)response_buf, dest);
    }
    strcat((char *)response_buf, "\r\n");
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //OUTPUT SOURCE 9-16
    memset(source,0,sizeof(source));
    memset(dest,0,sizeof(dest));
    sprintf((char *)source,"OUTPUT SOURCE  ");
    strcat((char *)response_buf, source);
    for(j = 8;j < 16;j++)
    {
        //add setting
        memset(source,0,sizeof(source));
        memset(dest,0,sizeof(dest));
        ///////////////////
        memset(buf,0,sizeof(buf));
        sprintf((char *)source,"%d", outputsource[j] );
        ///////////////////
        add_spaces_tostr(source,dest,7);
        strcat((char *)response_buf, dest);
    }
    strcat((char *)response_buf, "\r\n");
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //OUTPUT TRIP COUNTS 9-16
    memset(source,0,sizeof(source));
    memset(dest,0,sizeof(dest));
    sprintf((char *)source,"OUTPUT TRIPS   ");
    strcat((char *)response_buf, source);
    for(j = 8;j < 16;j++)
    {
        //add setting
        memset(source,0,sizeof(source));
        memset(dest,0,sizeof(dest));
        ///////////////////
        memset(buf,0,sizeof(buf));
        sprintf((char *)source,"%d", tripcntrs->outputcnt[j] );
        ///////////////////
        add_spaces_tostr(source,dest,7);
        strcat((char *)response_buf, dest);
    }
    strcat((char *)response_buf, "\r\n");
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    ///////////////////////////////////////////////////////////////
    endx:
    if(ret == RET_SUCCESS)
        strcat((char *)response_buf, "@OK@\r\n##");
    else
        strcat((char *)response_buf, "@FAIL@\r\n##");
    superlen = strlen(response_buf);
    msglen = superlen - 10;
    //
    /////////////////////
    memset(buf,0,sizeof(buf));
    sprintf(buf,"$$[%04d]",msglen);
    for(j=0;j<8;j++)
    {
        response_buf[j] = buf[j];
    }
    /////////////////////
#if DEBUG
    debug_msg("Sent Response to OAM...\r\n");
#endif
    //
    /////////////////////
    return ret;
    /////////////////////
    ///////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////
}

static int construct_showalarmrelay_response(char *response_buf)
{
    int ret = RET_SUCCESS;
    uint16_t superlen,msglen ;
    char buf[100];
    int j;
    uint8_t status;
    //==================================================================
    //
    //////////////////////////////////////////////////////////////////////
    //Construct VALID RESPONSE
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    construct_oam_large_template_responsepkt((uint8_t *)response_buf, RESPONSE_BUF_SIZE);
    ///////////////////////////////////////////////////////////////
    get_alarmrelay_status(&status);
    ///////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////
    strcat((char *)response_buf, "\r\n");
    if(status != 0)
    {
    strcat((char *)response_buf, "ALARM STATUS: **ACTIVE\r\n");
    strcat((char *)response_buf, "              (RELAY IS OFF, FAIL-SAFE OPERATION)\r\n");
    }
    else
    {
    strcat((char *)response_buf, "ALARM STATUS: INACTIVE\r\n");
    strcat((char *)response_buf, "              (RELAY IS ON, FAIL-SAFE OPERATION)\r\n");
    }
    //===================================================================
    //
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    ///////////////////////////////////////////////////////////////
    if(ret == RET_SUCCESS)
        strcat((char *)response_buf, "@OK@\r\n##");
    else
        strcat((char *)response_buf, "@FAIL@\r\n##");
    superlen = strlen(response_buf);
    msglen = superlen - 10;
    //
    /////////////////////
    memset(buf,0,sizeof(buf));
    sprintf(buf,"$$[%04d]",msglen);
    for(j=0;j<8;j++)
    {
        response_buf[j] = buf[j];
    }
    /////////////////////
#if DEBUG
    debug_msg("Sent Response to OAM...\r\n");
#endif
    //
    /////////////////////
    return ret;
    /////////////////////
    ///////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////
}



void init_difcommand_list_core_comqueries(void)
{
  	//cli group CLIGROUP_COMCOREQUERY
  	//1- 5(5nos)
  	add_command_to_list(&showcomversion);
  	add_command_to_list(&showcomselftest);
  	add_command_to_list(&showcomsettings);
  	add_command_to_list(&showtripcounters);
  	add_command_to_list(&showalarmrelay);
  	//

}



