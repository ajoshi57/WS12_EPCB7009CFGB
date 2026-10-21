/****************************************************
  FILE NAME		: f01-core-comsettings.c
  ORIGINATOR	: A Joshi
  LAST EDITED   : 10 MAY 2026
  PROJECT       : WS12_EPCB7011CFGB
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/

#include "f01-core-comsettings.h"
//
#define DEBUG 1
#define INCLUDE_COMFLASH 1
//
int handle_setcomdefaults(int argc, char **argv, uint8_t cmdkwlen, char *response_buf);
int handle_setcomtitle(int argc, char **argv, uint8_t cmdkwlen, char *response_buf);
int handle_setglitchfilter(int argc, char **argv, uint8_t cmdkwlen, char *response_buf);
int handle_setoutputpulse(int argc, char **argv, uint8_t cmdkwlen, char *response_buf);
int handle_setoutputsource(int argc, char **argv, uint8_t cmdkwlen, char *response_buf);
int handle_resetcard(int argc, char **argv, uint8_t cmdkwlen, char *response_buf);
int handle_cleartripcounters(int argc, char **argv, uint8_t cmdkwlen, char *response_buf);
//
void init_difcommand_list_core_comsettings(void);


CMDMSG_T setcomdefaults =
{
		"#setdefaults#", //cmdstr
		1, //"exe"
		//(char *[]){"setccdefaults"},
		"Set Card to FACTORY Defaults (ALL Settings)\r\n", //help_str
		(CLIGROUP_CORECOMSET + 1), //command_id
		AUTH_SUPERUSER,         //auth_level
		&handle_setcomdefaults     //handler fn
};

CMDMSG_T setcomtitle =
 {
 		"#settitle#", //cmdstr
 		1, //<Title String>
 		//(char *[]){"settitle"}, //argtype str[Title String]
 		"Set User Configured CARD TITLE String (2-20 Characters)\r\n", //help_str
 		(CLIGROUP_CORECOMSET + 2), //command_id
 		AUTH_ADMINUSER,         //auth_level
 		&handle_setcomtitle     //handler fn
 };


CMDMSG_T setglitchfilter =
{
	"#setglitchfilter#", //cmdstr
	2,
	"Set GLITCH FILTER to 0.5ms - 5.0ms\r\n", //help_str
	(CLIGROUP_CORECOMSET + 3), //command_id
	AUTH_ADMINUSER,         //auth_level
	&handle_setglitchfilter      //handler fn
};

CMDMSG_T setoutputpulse =
{
 	"#setoutputpulse#", //cmdstr
 	2, 
  	"Set OUTPUT PULSE WIDTH TO various Options (in ms)...see help\r\n", //help_str
 	(CLIGROUP_CORECOMSET + 4), //command_id
 	AUTH_ADMINUSER,            //auth_level
 	&handle_setoutputpulse     //handler fn
};

CMDMSG_T setoutputsource =
{
    "#setoutputsource#", //cmdstr
    2,
    "Set OUTPUT SOURCE Mapping Selection...see help\r\n", //help_str
    (CLIGROUP_CORECOMSET + 5), //command_id
    AUTH_ADMINUSER,            //auth_level
    &handle_setoutputsource     //handler fn
};

CMDMSG_T resetcard =
{
        "#resetcard#", //cmdstr
        1, //"exe"
        "RESET and Re-Boot (ALL Settings)\r\n", //help_str
        (CLIGROUP_CORECOMSET + 6), //command_id
        AUTH_SUPERUSER,         //auth_level
        &handle_resetcard     //handler fn
};

CMDMSG_T cleartripcounters =
{
        "#cleartripcounters#", //cmdstr
        1, //"exe"
        "CLEAR/RESET Trip Counters and FETCH Alarm Queue\r\n", //help_str
        (CLIGROUP_CORECOMSET + 7), //command_id
        AUTH_SUPERUSER,         //auth_level
        &handle_cleartripcounters     //handler fn
};


int handle_setcomdefaults(int argc, char **argv, uint8_t cmdkwlen, char *response_buf)
{
	//possibilities
	//setcomdefaults
	//setcomdefaults help
	//setcomdefaults ?
	//setcomdefaults exe
	int c1 = 0;
	int c1x = 0;
	int c2 = 0;
	int selhelp = 0;
	//
	int ret = RET_FAIL;
	uint16_t superlen,msglen ;
	char buf[100];
	int j;
	//
	int flg;
	//================================================================================
	//
	//////////////////////////////////////////////////////////////////////////
	uint8_t *oammsg = (uint8_t *) pvPortMalloc(sizeof(uint8_t) * MAX_RESPSTRING_SIZE);
	if (oammsg == NULL)
	{
		debug_msg("\r\nFAIL Alloc Mem - 'oammsg'\r\n");
		while (1)
			;	//stuck here
	}
	///////////////////////////////////////////////////////////////////////////
	VALIDRESP_T *oamresppkt = (VALIDRESP_T *) pvPortMalloc(sizeof(VALIDRESP_T));
	if(oamresppkt == NULL)
	{
		debug_msg("\r\nFAIL Alloc Mem - 'VALIDRESP_T'\r\n");
		while (1)
			;	//stuck here
	}
	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////
    //arg check and help
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
        selhelp = 1;
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
				c2 = strncmp(argv[1], "exe",strlen("exe"));
				if(c2 != 0)
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
    }
    if(selhelp != 0)
    {
    	construct_oam_large_template_responsepkt((uint8_t *)response_buf, RESPONSE_BUF_SIZE);
		 strcat(response_buf, setcomdefaults.help_str);
         strcat(response_buf, "\r\n");
        //
		memset(buf,0,sizeof(buf));
        sprintf(buf, "Cmd String <%s> has 1 Arg :\r\n", setcomdefaults.command_name);
        strcat(response_buf, buf);
 		//
        //ARG-1, <Group>
        strcat(response_buf, "Arg-1 a fixed string for execution 'exe'\r\n");
		memset(buf,0,sizeof(buf));
        sprintf(buf, "E.G. %s exe\r\n", setcomdefaults.command_name);
        strcat(response_buf, buf);
         //
         strcat(response_buf, "Device shall Re-START Card after default configuration is applied\r\n");
         //
 		strcat((char *)response_buf, "@OK@\r\n##");
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
 #if DEBUG
        debug_msg((char *)response_buf);
 #endif
         /////////////////////
         ret = RET_SUCCESS;
         goto endx;
    }
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    //////////////////////////////////////////////////////////////////////
	//Normal Execution
    flg = EEPROM_OAMConfig_default();
    if(flg != 0)
    {
        ret = RET_FAIL;
        memset(oammsg,0,MAX_RESPSTRING_SIZE);
        sprintf((char *)oammsg, "Error Saving Config in EEPROM !\r\nSYSTEM ERROR !\r\n@FAIL@\r\n");
        construct_oam_responsepkt(oammsg, oamresppkt);
#if DEBUG
       debug_msg((char *)oamresppkt->resppkt);
#endif
       memcpy(response_buf, oamresppkt->resppkt, oamresppkt->totallen );
       goto endx;
    }
	/////////////////////////////
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //////////////////////////////////////////////////////////////////////
    //VALID RESPONSE
    ret = RET_SUCCESS;
    memset(oammsg,0,MAX_RESPSTRING_SIZE);
    sprintf((char *)oammsg,"SUCCESS\r\n@OK@\r\n");
    construct_oam_responsepkt(oammsg, oamresppkt);
    memcpy(response_buf, oamresppkt->resppkt, oamresppkt->totallen );
    //
    //////////////////////////////////////////////////////////////////////
	//Normal Execution, first send response then...just BOOTUP
	//VALID RESPONSE
	memset(oammsg,0,MAX_RESPSTRING_SIZE);
	sprintf((char *)oammsg,"Card Boot up under progress...\r\nPlease Wait...\r\n@OK@\r\n");
    construct_oam_responsepkt(oammsg, oamresppkt);
    memcpy(response_buf, oamresppkt->resppkt, oamresppkt->totallen );
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    ajUART0Write(response_buf);
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	vPortFree(oammsg);
	vPortFree(oamresppkt);
	vTaskDelay(500);
	//drive_cccard_ready(0);
    vTaskDelay( 500 );
    //
    HWREG(NVIC_APINT) = NVIC_APINT_VECTKEY | NVIC_APINT_SYSRESETREQ;
	//
	////////////////
	endx:
	vPortFree(oammsg);
	vPortFree(oamresppkt);
	return ret;
	////////////////
}


int handle_setcomtitle(int argc, char **argv, uint8_t cmdkwlen, char *response_buf)
{
	//possibilities
	//setcomtitle
	//setcomtitle help
	//setcomtitle ?
	//setcomtitle <TitleString> <CR>
	int c1 = 0;
	int c1x = 0;
	int selhelp = 0;
	//
	int ret = RET_FAIL;
	uint16_t superlen,msglen ;
	char buf[20];
	int j;
	int flg;
	uint8_t titlelen;
	uint8_t title[24];
	uint8_t ttitle[24];
	uint8_t addr,data;
	//
	//////////////////////////////////////////////////////////////////////////
	uint8_t *oammsg = (uint8_t *) pvPortMalloc(sizeof(uint8_t) * MAX_RESPSTRING_SIZE);
	if (oammsg == NULL)
	{
		debug_msg("\r\nhandle_difsettitle: FAIL Alloc Mem - 'oammsg'\r\n");
		while (1)
			;	//stuck here
	}
	VALIDRESP_T *oamresp = (VALIDRESP_T *) pvPortMalloc(sizeof(VALIDRESP_T));
	if(oamresp == NULL)
	{
		debug_msg("\r\nhandle_difsettitle: FAIL Alloc Mem - 'VALIDRESP_T'\r\n");
		while (1)
			;	//stuck here
	}
	//////////////////////////////////////////////////////////////////////////
	///////////////////////////////
    //arg check and help
    if (argc != 1 && argc != 2)
    {
		memset(oammsg,0,MAX_RESPSTRING_SIZE);
    	sprintf((char *)oammsg, "Incorrect Argument Count !\r\n");
        strcat((char *)oammsg, "SYNTAX ERROR\r\n@FAIL@\r\n");
		construct_oam_responsepkt(oammsg, oamresp);
#if DEBUG
        debug_msg((char *)oamresp->resppkt);
#endif
        memcpy(response_buf, oamresp->resppkt, oamresp->totallen );
	    goto endx;
    }
    //check...this could be 'help'
    selhelp = 0;
    if (argc == 1)
    {
        selhelp = 1;
    }
    else
    {
        if ((argc == 2) && (argv[1] == NULL))
        {
			memset(oammsg,0,MAX_RESPSTRING_SIZE);
			sprintf((char *)oammsg, "Execution Error !\r\n@FAIL@\r\n");
			construct_oam_responsepkt(oammsg, oamresp);
#if DEBUG
	       debug_msg((char *)oamresp->resppkt);
#endif
	       memcpy(response_buf, oamresp->resppkt, oamresp->totallen );
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
        }
    }
    if(selhelp != 0)
    {
		 construct_oam_large_template_responsepkt((uint8_t *)response_buf, RESPONSE_BUF_SIZE);
    	 strcat(response_buf, setcomtitle.help_str);
         //
         strcat(response_buf, "Cmd String <#settitle#> has 1 Arg :\r\n");
         strcat(response_buf, "Arg-1 is a String Argument representing Card Title\r\n");
         strcat(response_buf, "String of upto 2-20 chars without spaces\r\n");
         strcat(response_buf, "Use Underscore chars where spaces are required\r\n");
         strcat(response_buf, "E.G. #setcomtitle# <Title String><CR>\r\n");
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
    /////////////////////////////////////////////
    //check argv[1]
    /////////////////////////////////////////////
    memset(ttitle,0,sizeof(ttitle));
    memset(title,0,sizeof(title));
    strcpy((char *)ttitle, argv[1]);
    /////////////////////////////////////////////
	titlelen = strlen((char *)ttitle);
	debug_msg("title len %d\r\n", titlelen);
	/////////////////////////////////////////////
    if(titlelen < 2 || titlelen > MAX_TITLE_SIZE)
    {
		memset(oammsg,0,MAX_RESPSTRING_SIZE);
		sprintf((char *)oammsg, "ERROR Title string!\r\nExpected 2-20 chars\r\nExecution Error !\r\n@FAIL@\r\n");
		construct_oam_responsepkt(oammsg, oamresp);
#if DEBUG
       debug_msg((char *)oamresp->resppkt);
#endif
       memcpy(response_buf, oamresp->resppkt, oamresp->totallen );
	   goto endx;

    }
    //////////////////////////////////////////////////////////////////////
    //copy and convert to CAPS
	//////////////////////////////////////////////////////////////////////
	//cfgdbase...Modify Title in cfg dbase
	//**AJ
	for(j = 0;j < titlelen;j++)
	{
		if(ttitle[j] >=0x61 && ttitle[j] <= 0x7a)
		{
			//a-z...make cAPS
			title[j] = ttitle[j] - 0x20;
		}
		else
		{
			title[j] = ttitle[j];
		}
	}
    //////////////////////////////////////////////////////////////////////
    for(j = titlelen;j < 24;j++)
    {
        title[j] = 0x0;
    }
    //////////////////////////////////////////////////////////////////////

	debug_msg("new title %s len %d\r\n", title, titlelen);
	//
	flg = EEPROM_OAMConfig_title(title, 24);
    if(flg != 0)
    {
        memset(oammsg,0,MAX_RESPSTRING_SIZE);
        sprintf((char *)oammsg, "Error Saving Config in EEPROM !\r\nSYSTEM ERROR !\r\n@FAIL@\r\n");
        construct_oam_responsepkt(oammsg, oamresp);
#if DEBUG
       debug_msg((char *)oamresp->resppkt);
#endif
       memcpy(response_buf, oamresp->resppkt, oamresp->totallen );
       goto endx;
    }
	//////////////////////////////////////////////////////////////////////
    for(j = 0;j < titlelen;j++)
    {
        addr = j;
        data = title[j];
        ep24aa128_writecycle(0,0, addr, data);
    }
    //////////////////////////////////////////////////////////////////////
    for(j = titlelen;j < (24);j++)
    {
        addr = j;
        data = 0x0;
        ep24aa128_writecycle(0,0, addr, data);
    }
	//////////////////////////////////////////////////////////////////////
	//VALID RESPONSE
	ret = RET_SUCCESS;
	memset(oammsg,0,MAX_RESPSTRING_SIZE);
	sprintf((char *)oammsg,"SUCCESS\r\n@OK@\r\n");
	construct_oam_responsepkt(oammsg, oamresp);
	memcpy(response_buf, oamresp->resppkt, oamresp->totallen );
	//
	//////////////////////////////////////////////////////////////////////
	///////////////////////////////
	endx:
	vPortFree(oammsg);
	vPortFree(oamresp);
	return ret;
	///////////////////////////////
}


int handle_setglitchfilter(int argc, char **argv, uint8_t cmdkwlen, char *response_buf)
{
	//possibilities
	//setglitchfilter
	//setglitchfilter help
	//setglitchfilter ?
	//setglitchfilter <ChanNo> <Factor> <CR>
	int c1 = 0;
	int c1x = 0;
	int selhelp = 0;
	uint16_t u16channo;
	uint8_t lenstr1; //len of string argument 1
	uint8_t lenstr2; //len of string argument 2
	//
	int ret = RET_FAIL;
	uint16_t superlen,msglen ;
	char buf[100];
	int j;
	int flg;
	//
	uint8_t u8channo;
	uint8_t u8gfvalue;
	uint8_t glitchfilter[MAX_INCHANS];
	//
    uint8_t num;
    uint8_t den;
	//////////////////////////////////////////////////////////////////////////
	uint8_t *oammsg = (uint8_t *) pvPortMalloc(sizeof(uint8_t) * MAX_RESPSTRING_SIZE);
	if (oammsg == NULL)
	{
		debug_msg("\r\nFAIL Alloc Mem - 'oammsg'\r\n");
		while (1)
			;	//stuck here
	}
	VALIDRESP_T *oamresp = (VALIDRESP_T *) pvPortMalloc(sizeof(VALIDRESP_T));
	if(oamresp == NULL)
	{
		debug_msg("\r\nFAIL Alloc Mem - 'VALIDRESP_T'\r\n");
		while (1)
			;	//stuck here
	}
	//////////////////////////////////////////////////////////////////////////
	///////////////////////////////
    //arg check and help
    if (argc != 1 && argc != 2 && argc != 3)
    {
		memset(oammsg,0,MAX_RESPSTRING_SIZE);
    	sprintf((char *)oammsg, "Incorrect Argument Count !\r\n");
        strcat((char *)oammsg, "SYNTAX ERROR\r\n@FAIL@\r\n");
		construct_oam_responsepkt(oammsg, oamresp);
#if DEBUG
        debug_msg((char *)oamresp->resppkt);
#endif
        memcpy(response_buf, oamresp->resppkt, oamresp->totallen );
	    goto endx;
    }
    //check...this could be 'help'
    selhelp = 0;
    if (argc == 1)
    {
        selhelp = 1;
    }
    else if (argc == 2)
    {
        if ((argc == 2) && (argv[1] == NULL))
        {
			memset(oammsg,0,MAX_RESPSTRING_SIZE);
			sprintf((char *)oammsg, "Execution Error !\r\n@FAIL@\r\n");
			construct_oam_responsepkt(oammsg, oamresp);
#if DEBUG
	       debug_msg((char *)oamresp->resppkt);
#endif
	       memcpy(response_buf, oamresp->resppkt, oamresp->totallen );
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
    			construct_oam_responsepkt(oammsg, oamresp);
    #if DEBUG
    	       debug_msg((char *)oamresp->resppkt);
    #endif
    	       memcpy(response_buf, oamresp->resppkt, oamresp->totallen );
    		   goto endx;
            }
        }
    }
    if(selhelp != 0)
    {
		 construct_oam_large_template_responsepkt((uint8_t *)response_buf, RESPONSE_BUF_SIZE);
		 strcat(response_buf, setglitchfilter.help_str);
         //
		 memset(buf,0,sizeof(buf));
         sprintf(buf, "Cmd String <%s> has 2 Args :\r\n", setglitchfilter.command_name);
		 strcat(response_buf,buf);
		 //
         strcat(response_buf, "Arg-1 is <ChanNo> 1-4 or 'all'\r\n");
         strcat(response_buf, "Arg-2 is <FilterValue> is 0.5ms - 5.0ms\r\n");
         strcat(response_buf, "Valid Strings are '0.5ms', '1.0ms', '1.5ms', '2.0ms', '2.5ms'\r\n");
         strcat(response_buf, "                  '3.0ms', '3.5ms', '4.0ms', '5.0ms'\r\n");
		 //
		 memset(buf,0,sizeof(buf));
         sprintf(buf, "E.G. %s all 5.0ms<CR>\r\n", setglitchfilter.command_name);
		 strcat(response_buf,buf);
		 //
 		 strcat((char *)response_buf, "@OK@\r\n##");
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
 #if DEBUG
        debug_msg((char *)response_buf);
 #endif
         /////////////////////
         ret = RET_SUCCESS;
         goto endx;
         //
    }
    //////////////////////////////////////////////////////////////////////
	//check arg[1] input ChanNo 1-4 or 'all'
	lenstr1 = strlen(argv[1]);
	if(lenstr1 < 1 || lenstr1 > MAX_CMDSTRING_CHARS)
	{
		memset(oammsg,0,MAX_RESPSTRING_SIZE);
        sprintf((char *)oammsg, "Illegal Argument 1 String Length !\r\nSYNTAX ERROR !\r\n@FAIL@\r\n");
#if DEBUG
        debug_msg((char *)oammsg);
#endif
        construct_oam_responsepkt(oammsg, oamresp);
        memcpy(response_buf, oamresp->resppkt, oamresp->totallen );
		goto endx;
	}
	if(lenstr1 == 3)
	{
		c1 = strncmp(argv[1], "all",strlen("all"));
		if(c1 == 0)
		{
			u16channo = 99;
		}
		goto allchans;
	}
	////////////////////////////////////////////////////////////////
	flg = ConvertStrtoInt16(argv[1], lenstr1, &u16channo);
	if(flg != RET_SUCCESS)
	{
		memset(oammsg,0,MAX_RESPSTRING_SIZE);
		sprintf((char *)oammsg,"Illegal Param CHAN No !\r\nSYSTEM ERROR !\r\n@FAIL@\r\n");
#if DEBUG
        debug_msg((char *)oammsg);
#endif
        construct_oam_responsepkt(oammsg, oamresp);
        memcpy(response_buf, oamresp->resppkt, oamresp->totallen );
		goto endx;
	}
	///////////////////////////////////////////////////////////////////////////
	if(u16channo < MIN_INCHANS || u16channo > MAX_INCHANS)
	{
		memset(oammsg,0,MAX_RESPSTRING_SIZE);
        sprintf((char *)oammsg, "Illegal <ChanNo> %d-%d or 'all' expected !\r\nSYNTAX ERROR !\r\n@FAIL@\r\n", MIN_INCHANS, MAX_INCHANS);
#if DEBUG
        debug_msg((char *)oammsg);
#endif
        construct_oam_responsepkt(oammsg, oamresp);
        memcpy(response_buf, oamresp->resppkt, oamresp->totallen );
		goto endx;
	}
	//////////////////////////////////////////////////////////////////////
	allchans:
	u8channo = u16channo;
	///////////////////////////////////////////////////////////////////////////
	//check arg[2]
	lenstr2 = strlen(argv[2]);
	if(lenstr2 < MIN_CMDSTRING_CHARS || lenstr2 > MAX_CMDSTRING_CHARS)
	{
		memset(oammsg,0,MAX_RESPSTRING_SIZE);
        sprintf((char *)oammsg, "Illegal Argument 2 String Length !\r\nSYNTAX ERROR !\r\n@FAIL@\r\n");
#if DEBUG
        debug_msg((char *)oammsg);
#endif
        construct_oam_responsepkt(oammsg, oamresp);
        memcpy(response_buf, oamresp->resppkt, oamresp->totallen );
		goto endx;
	}
	////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////
	//expected 5 chars
	// x.yms               5 chars
	if(lenstr2 != 5)
	{
        memset(oammsg,0,MAX_RESPSTRING_SIZE);
        sprintf((char *)oammsg, "Illegal Argument String ('X.Yms' see help for valid values) !\r\nSYNTAX ERROR !\r\n@FAIL@\r\n");
#if DEBUG
        debug_msg((char *)oammsg);
#endif
        construct_oam_responsepkt(oammsg, oamresp);
        memcpy(response_buf, oamresp->resppkt, oamresp->totallen );
        goto endx;

	}
	//++++++
	memset(buf,0,sizeof(buf));
	strncpy(buf,argv[2], lenstr2);
	if(buf[1] != '.' || buf[3] != 'm' || buf[4] != 's' )
	{
        memset(oammsg,0,MAX_RESPSTRING_SIZE);
        sprintf((char *)oammsg, "Illegal Argument String ('X.Yms' see help for valid values) !\r\nSYNTAX ERROR !\r\n@FAIL@\r\n");
#if DEBUG
        debug_msg((char *)oammsg);
#endif
        construct_oam_responsepkt(oammsg, oamresp);
        memcpy(response_buf, oamresp->resppkt, oamresp->totallen );
        goto endx;
	}
    if(buf[2] != '0' && buf[2] != '5' )
    {
        memset(oammsg,0,MAX_RESPSTRING_SIZE);
        sprintf((char *)oammsg, "Illegal Argument String ('X.Yms' see help for valid values) !\r\nSYNTAX ERROR !\r\n@FAIL@\r\n");
#if DEBUG
        debug_msg((char *)oammsg);
#endif
        construct_oam_responsepkt(oammsg, oamresp);
        memcpy(response_buf, oamresp->resppkt, oamresp->totallen );
        goto endx;
    }
    if(buf[0] != '0' && buf[0] != '1' && buf[0] != '2' && buf[0] != '3' && buf[0] != '4' && buf[0] != '5')
    {
        memset(oammsg,0,MAX_RESPSTRING_SIZE);
        sprintf((char *)oammsg, "Illegal Argument String ('X.Yms' see help for valid values) !\r\nSYNTAX ERROR !\r\n@FAIL@\r\n");
#if DEBUG
        debug_msg((char *)oammsg);
#endif
        construct_oam_responsepkt(oammsg, oamresp);
        memcpy(response_buf, oamresp->resppkt, oamresp->totallen );
        goto endx;
    }
    if(buf[2] == '5' && buf[0] == '5')
    {
        memset(oammsg,0,MAX_RESPSTRING_SIZE);
        sprintf((char *)oammsg, "Illegal Argument String ('MAX 5.0ms') !\r\nSYNTAX ERROR !\r\n@FAIL@\r\n");
#if DEBUG
        debug_msg((char *)oammsg);
#endif
        construct_oam_responsepkt(oammsg, oamresp);
        memcpy(response_buf, oamresp->resppkt, oamresp->totallen );
        goto endx;
    }
    if(buf[2] == '0' && buf[0] == '0')
    {
        memset(oammsg,0,MAX_RESPSTRING_SIZE);
        sprintf((char *)oammsg, "Illegal Argument String ('MIN 0.5ms') !\r\nSYNTAX ERROR !\r\n@FAIL@\r\n");
#if DEBUG
        debug_msg((char *)oammsg);
#endif
        construct_oam_responsepkt(oammsg, oamresp);
        memcpy(response_buf, oamresp->resppkt, oamresp->totallen );
        goto endx;
    }
    ////////////////////////////////////////////////////
    num = buf[0] - 0x30;
    den = buf[2] - 0x30;
    ////////////////////////////////////////////////////
    if(num == 0 && den == 5)
    {
        u8gfvalue = 0;
    }
    else if(num == 1 && den == 0)
    {
        u8gfvalue = 1;
    }
    else if(num == 1 && den == 5)
    {
        u8gfvalue = 2;
    }
    else if(num == 2 && den == 0)
    {
        u8gfvalue = 3;
    }
    else if(num == 2 && den == 5)
    {
        u8gfvalue = 4;
    }
    else if(num == 3 && den == 0)
    {
        u8gfvalue = 5;
    }
    else if(num == 3 && den == 5)
    {
        u8gfvalue = 6;
    }
    else if(num == 4 && den == 0)
    {
        u8gfvalue = 7;
    }
    else //if(num == 5 && den == 0)
    {
        u8gfvalue = 8;
    }
	////////////////////////////////////////////////////////////////////////////////
    flg = EEPROM_OAMConfig_glitchfilter_factor(u8channo, u8gfvalue);
    if(flg != 0)
    {
        memset(oammsg,0,MAX_RESPSTRING_SIZE);
        sprintf((char *)oammsg, "Error Saving Config in EEPROM !\r\nSYSTEM ERROR !\r\n@FAIL@\r\n");
        construct_oam_responsepkt(oammsg, oamresp);
#if DEBUG
       debug_msg((char *)oamresp->resppkt);
#endif
       memcpy(response_buf, oamresp->resppkt, oamresp->totallen );
       goto endx;
    }
	///////////////////////////////////////////////////////////////////////////////
#if DEBUG
	if(u8channo == 99)
	{
		memset(buf,0,sizeof(buf));
		get_glitchfilterstr(glitchfilter[0], buf, sizeof(buf));
		debug_msg((char *)"ALL CHANS, GLITCH FILTER  %s\r\n", buf);
	}
	else
	{
		memset(buf,0,sizeof(buf));
		get_glitchfilterstr(glitchfilter[u8channo - 1], buf, sizeof(buf));
		debug_msg((char *)"CHAN NO %d, GLITCH FILTER %s\r\n", u8channo, buf);
	}
#endif
	//////////////////////////////////////////////////////////////////////
	//Normal Execution...APPLY
	flg = program_gf(u8channo, u8gfvalue, 1);
	if(flg != 0)
	{
        memset(oammsg,0,MAX_RESPSTRING_SIZE);
        sprintf((char *)oammsg, "Error Config GF Logic !\r\nSYSTEM ERROR !\r\n@FAIL@\r\n");
        construct_oam_responsepkt(oammsg, oamresp);
#if DEBUG
       debug_msg((char *)oamresp->resppkt);
#endif
       memcpy(response_buf, oamresp->resppkt, oamresp->totallen );
       goto endx;

	}
	//////////////////////////////////////////////////////////////////////
	//VALID RESPONSE
	memset(oammsg,0,MAX_RESPSTRING_SIZE);
	sprintf((char *)oammsg,"SUCCESS\r\n@OK@\r\n");
	construct_oam_responsepkt(oammsg, oamresp);
	memcpy(response_buf, oamresp->resppkt, oamresp->totallen );
	#if DEBUG
	debug_msg("Sent Response to OAM...\r\n");
	debug_msg((char *)response_buf);
	ajUART0crlf();
	#endif
	//
	ret = RET_SUCCESS;
	//////////////////////////////////////////////////////////////////////
	///////////////////////////////////////
	endx:
	vPortFree(oammsg);
	vPortFree(oamresp);
	return ret;
	///////////////////////////////////////
}


int handle_setoutputpulse(int argc, char **argv, uint8_t cmdkwlen, char *response_buf)
{
	//possibilities
	//setoutputpulse
	//setoutputpulse help
	//setoutputpulse ?
	//setoutputpulse <ChanNo> <ValueStr> <CR>
	int c1 = 0;
	int c1x = 0;
	int selhelp = 0;
	uint16_t u16channo;
	uint16_t u16pwvalue;
	uint8_t lenstr1; //len of string argument 1
	uint8_t lenstr2; //len of string argument 2
	//
	int ret = RET_FAIL;
	uint16_t superlen,msglen ;
	char buf[100];
	int j;
	int flg;
	//
	uint8_t u8channo;
	uint8_t u8pwset;
	//
	//////////////////////////////////////////////////////////////////////////
	uint8_t *oammsg = (uint8_t *) pvPortMalloc(sizeof(uint8_t) * MAX_RESPSTRING_SIZE);
	if (oammsg == NULL)
	{
		debug_msg("\r\nFAIL Alloc Mem - 'oammsg'\r\n");
		while (1)
			;	//stuck here
	}
	VALIDRESP_T *oamresp = (VALIDRESP_T *) pvPortMalloc(sizeof(VALIDRESP_T));
	if(oamresp == NULL)
	{
		debug_msg("\r\nFAIL Alloc Mem - 'VALIDRESP_T'\r\n");
		while (1)
			;	//stuck here
	}
	//////////////////////////////////////////////////////////////////////////
	///////////////////////////////
    //arg check and help
    if (argc != 1 && argc != 2 && argc != 3)
    {
		memset(oammsg,0,MAX_RESPSTRING_SIZE);
    	sprintf((char *)oammsg, "Incorrect Argument Count !\r\n");
        strcat((char *)oammsg, "SYNTAX ERROR\r\n@FAIL@\r\n");
		construct_oam_responsepkt(oammsg, oamresp);
#if DEBUG
        debug_msg((char *)oamresp->resppkt);
#endif
        memcpy(response_buf, oamresp->resppkt, oamresp->totallen );
	    goto endx;
    }
    //check...this could be 'help'
    selhelp = 0;
    if (argc == 1)
    {
        selhelp = 1;
    }
    else if (argc == 2)
    {
        if ((argc == 2) && (argv[1] == NULL))
        {
			memset(oammsg,0,MAX_RESPSTRING_SIZE);
			sprintf((char *)oammsg, "Execution Error !\r\n@FAIL@\r\n");
			construct_oam_responsepkt(oammsg, oamresp);
#if DEBUG
	       debug_msg((char *)oamresp->resppkt);
#endif
	       memcpy(response_buf, oamresp->resppkt, oamresp->totallen );
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
    			construct_oam_responsepkt(oammsg, oamresp);
    #if DEBUG
    	       debug_msg((char *)oamresp->resppkt);
    #endif
    	       memcpy(response_buf, oamresp->resppkt, oamresp->totallen );
    		   goto endx;
            }
        }
    }
    if(selhelp != 0)
    {
		construct_oam_large_template_responsepkt((uint8_t *)response_buf, RESPONSE_BUF_SIZE);
		strcat(response_buf, setoutputpulse.help_str);
        //
		memset(buf,0,sizeof(buf));
        sprintf(buf, "Cmd String <%s> has 2 Args :\r\n", setoutputpulse.command_name);
		strcat(response_buf,buf);
		//
        strcat(response_buf, "Arg-1 is <ChanNo> 1-16 or 'all'\r\n");
        strcat(response_buf, "Arg-2 is <PulseValue> String \r\n");
        strcat(response_buf, "Valid Settings are: \r\n");
        strcat(response_buf, "5ms XXms YYYms or 1000ms \r\n");
        strcat(response_buf, "where XX  is 10,  20,  50\r\n");
        strcat(response_buf, "where YYY is 100, 150, 200, 250, 300, 350\r\n");
        strcat(response_buf, "             400, 450, 500, 600, 750\r\n");
		 //
		 memset(buf,0,sizeof(buf));
         sprintf(buf, "E.G. %s 5 5ms<CR>\r\n", setoutputpulse.command_name);
		 strcat(response_buf,buf);
		 //
         memset(buf,0,sizeof(buf));
         sprintf(buf, "E.G. %s 8 50ms<CR>\r\n", setoutputpulse.command_name);
         strcat(response_buf,buf);
         //
         memset(buf,0,sizeof(buf));
         sprintf(buf, "E.G. %s all 600ms<CR>\r\n", setoutputpulse.command_name);
         strcat(response_buf,buf);
         //
         memset(buf,0,sizeof(buf));
         sprintf(buf, "E.G. %s 16 1000ms<CR>\r\n", setoutputpulse.command_name);
         strcat(response_buf,buf);
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
	//check arg[1] ChanNo 1-16 or 'all'
	lenstr1 = strlen(argv[1]);
	if(lenstr1 < 1 || lenstr1 > MAX_CMDSTRING_CHARS)
	{
		memset(oammsg,0,MAX_RESPSTRING_SIZE);
        sprintf((char *)oammsg, "Illegal Argument 1 String Length !\r\nSYNTAX ERROR !\r\n@FAIL@\r\n");
#if DEBUG
        debug_msg((char *)oammsg);
#endif
        construct_oam_responsepkt(oammsg, oamresp);
        memcpy(response_buf, oamresp->resppkt, oamresp->totallen );
		goto endx;
	}
	if(lenstr1 == 3)
	{
		c1 = strncmp(argv[1], "all",strlen("all"));
		if(c1 == 0)
		{
			u16channo = 99;
		}
		goto allchans;
	}
	////////////////////////////////////////////////////////////////
	flg = ConvertStrtoInt16(argv[1], lenstr1, &u16channo);
	if(flg != RET_SUCCESS)
	{
		memset(oammsg,0,MAX_RESPSTRING_SIZE);
		sprintf((char *)oammsg,"Illegal Param CHAN No !\r\nSYSTEM ERROR !\r\n@FAIL@\r\n");
#if DEBUG
        debug_msg((char *)oammsg);
#endif
        construct_oam_responsepkt(oammsg, oamresp);
        memcpy(response_buf, oamresp->resppkt, oamresp->totallen );
		goto endx;
	}
	///////////////////////////////////////////////////////////////////////////
	if(u16channo < MIN_OUTCHANS || u16channo > MAX_OUTCHANS)
	{
		memset(oammsg,0,MAX_RESPSTRING_SIZE);
        sprintf((char *)oammsg, "Illegal <ChanNo> %d-%d or 'all' expected !\r\nSYNTAX ERROR !\r\n@FAIL@\r\n", MIN_OUTCHANS, MAX_OUTCHANS);
#if DEBUG
        debug_msg((char *)oammsg);
#endif
        construct_oam_responsepkt(oammsg, oamresp);
        memcpy(response_buf, oamresp->resppkt, oamresp->totallen );
		goto endx;
	}
	//////////////////////////////////////////////////////////////////////
	allchans:
	u8channo = u16channo;
	///////////////////////////////////////////////////////////////////////////
	//check arg[2]
	lenstr2 = strlen(argv[2]);
	if(lenstr2 < MIN_CMDSTRING_CHARS || lenstr2 > MAX_CMDSTRING_CHARS)
	{
		memset(oammsg,0,MAX_RESPSTRING_SIZE);
        sprintf((char *)oammsg, "Illegal Argument 2 String Length !\r\nSYNTAX ERROR !\r\n@FAIL@\r\n");
#if DEBUG
        debug_msg((char *)oammsg);
#endif
        construct_oam_responsepkt(oammsg, oamresp);
        memcpy(response_buf, oamresp->resppkt, oamresp->totallen );
		goto endx;
	}
	////////////////////////////////////////////////////////////////
	//extpected one of 2
	//A. 5ms                 3 chars
	//B. XXms                4 chars
	//C. YYYms               5 chars
	//D. 1000ms              6 chars
	if(lenstr2 == 3)
	{
		flg = strncmp(argv[2], "5ms",strlen("5ms"));
		if(flg == 0)
		{
			u8pwset = 0;
		}
		else 
		{
			memset(oammsg,0,MAX_RESPSTRING_SIZE);
			sprintf((char *)oammsg, "Illegal Argument String ('5ms' expected) !\r\nSYNTAX ERROR !\r\n@FAIL@\r\n");
	#if DEBUG
			debug_msg((char *)oammsg);
	#endif
			construct_oam_responsepkt(oammsg, oamresp);
			memcpy(response_buf, oamresp->resppkt, oamresp->totallen );
			goto endx;		
		}
	}
	else if(lenstr2 == 4)
	{
		//XXms
		 memset(buf,0,sizeof(buf));
		 memcpy(buf,argv[2], lenstr2);
		 buf[0] = 'X';
		 buf[1] = 'X';
		flg = strncmp(buf, "XXms",lenstr2);
		if(flg == 0)
		{
		 	memset(buf,0,sizeof(buf));
		 	memcpy(buf,argv[2], 2);
			buf[2] = NULL;
			////////////////////////////////////////////////////////////////
			flg = ConvertStrtoInt16(buf, 2, &u16pwvalue);
			if(flg != RET_SUCCESS)
			{
				memset(oammsg,0,MAX_RESPSTRING_SIZE);
				sprintf((char *)oammsg,"Illegal Param Value !\r\nSYSTEM ERROR !\r\n@FAIL@\r\n");
		#if DEBUG
				debug_msg((char *)oammsg);
		#endif
				construct_oam_responsepkt(oammsg, oamresp);
				memcpy(response_buf, oamresp->resppkt, oamresp->totallen );
				goto endx;
			}
			///////////////////////////////////////////////////////////////////////////
			if(u16pwvalue != 10 && u16pwvalue != 20 && u16pwvalue != 50)
			{
				memset(oammsg,0,MAX_RESPSTRING_SIZE);
				sprintf((char *)oammsg,"Illegal Param PULSE WIDTH Value !\r\nSYSTEM ERROR !\r\n@FAIL@\r\n");
		#if DEBUG
				debug_msg((char *)oammsg);
		#endif
				construct_oam_responsepkt(oammsg, oamresp);
				memcpy(response_buf, oamresp->resppkt, oamresp->totallen );
				goto endx;
			}
			switch(u16pwvalue)
			{
				case 10:
				u8pwset = 1;
				break;
				case 20:
				u8pwset = 2;
				break;
				default:
				u8pwset = 3;
				break;
			}
			///////////////////////////////////////////////////////////////////////////			
		}
		else
		{
            memset(oammsg,0,MAX_RESPSTRING_SIZE);
            sprintf((char *)oammsg, "Illegal Argument String ('XXms' expected) !\r\nSYNTAX ERROR !\r\n@FAIL@\r\n");
    #if DEBUG
            debug_msg((char *)oammsg);
    #endif
            construct_oam_responsepkt(oammsg, oamresp);
            memcpy(response_buf, oamresp->resppkt, oamresp->totallen );
            goto endx;

		}
	}
	else if(lenstr2 == 5)
	{
		//YYYms
		 memset(buf,0,sizeof(buf));
		 memcpy(buf,argv[2], lenstr2);
		 buf[0] = 'Y';
		 buf[1] = 'Y';
		 buf[2] = 'Y';
		flg = strncmp(buf, "YYYms",lenstr2);
		if(flg == 0)
		{
		 	memset(buf,0,sizeof(buf));
		 	memcpy(buf,argv[2], lenstr2);
			buf[3] = NULL;
			////////////////////////////////////////////////////////////////
			flg = ConvertStrtoInt16(buf, 3, &u16pwvalue);
			if(flg != RET_SUCCESS)
			{
				memset(oammsg,0,MAX_RESPSTRING_SIZE);
				sprintf((char *)oammsg,"Illegal Param PULSE WIDTH Value !\r\nSYSTEM ERROR !\r\n@FAIL@\r\n");
#if DEBUG
				debug_msg((char *)oammsg);
#endif
				construct_oam_responsepkt(oammsg, oamresp);
				memcpy(response_buf, oamresp->resppkt, oamresp->totallen );
				goto endx;
			}
			///////////////////////////////////////////////////////////////////////////
			uint8_t err = 0;
			switch(u16pwvalue)
			{
				case 100:
				u8pwset = 4;
				break;
				case 150:
				u8pwset = 5;
				break;
				case 200:
				u8pwset = 6;
				break;
				case 250:
				u8pwset = 7;
				break;
				case 300:
				u8pwset = 8;
				break;
				case 350:
				u8pwset = 9;
				break;
				case 400:
				u8pwset = 10;
				break;
				case 450:
				u8pwset = 11;
				break;
				case 500:
				u8pwset = 12;
				break;
				case 600:
				u8pwset = 13;
				break;
				case 750:
				u8pwset = 14;
				break;
				default:
				u8pwset = 0;
				err = 1;
				break;
			}
			if(err == 1)
			{
				memset(oammsg,0,MAX_RESPSTRING_SIZE);
				sprintf((char *)oammsg,"Illegal Param PULSE WIDTH Value (see HELP) !\r\nSYSTEM ERROR !\r\n@FAIL@\r\n");
		#if DEBUG
				debug_msg((char *)oammsg);
		#endif
				construct_oam_responsepkt(oammsg, oamresp);
				memcpy(response_buf, oamresp->resppkt, oamresp->totallen );
				goto endx;
			}
			///////////////////////////////////////////////////////////////////////////			
		}
		else
		{
            memset(oammsg,0,MAX_RESPSTRING_SIZE);
            sprintf((char *)oammsg, "Illegal Argument String ('YYYms' expected) !\r\nSYNTAX ERROR !\r\n@FAIL@\r\n");
    #if DEBUG
            debug_msg((char *)oammsg);
    #endif
            construct_oam_responsepkt(oammsg, oamresp);
            memcpy(response_buf, oamresp->resppkt, oamresp->totallen );
            goto endx;
		}
	}
	else if( lenstr2 == 6)
	{
        memset(buf,0,sizeof(buf));
        memcpy(buf,argv[2], lenstr2);
		flg = strncmp(buf, "1000ms",6);
		if(flg == 0)
		{
			u8pwset = 15;
		}
		else
		{
			memset(oammsg,0,MAX_RESPSTRING_SIZE);
			sprintf((char *)oammsg, "Illegal PULSE WIDTH VALUE ('1000ms' expected) !\r\nSYNTAX ERROR !\r\n@FAIL@\r\n");
#if DEBUG
			debug_msg((char *)oammsg);
#endif
			construct_oam_responsepkt(oammsg, oamresp);
			memcpy(response_buf, oamresp->resppkt, oamresp->totallen );
			goto endx;			
		}
	}
	else 
	{
		memset(oammsg,0,MAX_RESPSTRING_SIZE);
		sprintf((char *)oammsg, "Illegal Argument String (see HELP) !\r\nSYNTAX ERROR !\r\n@FAIL@\r\n");
#if DEBUG
		debug_msg((char *)oammsg);
#endif
		construct_oam_responsepkt(oammsg, oamresp);
		memcpy(response_buf, oamresp->resppkt, oamresp->totallen );
		goto endx;			
	}
	////////////////////////////////////////////////////////////////////////////////
	flg = EEPROM_OAMConfig_outputpulse(u8channo, u8pwset);
    if(flg != 0)
    {
        memset(oammsg,0,MAX_RESPSTRING_SIZE);
        sprintf((char *)oammsg, "Error Saving Config in EEPROM !\r\nSYSTEM ERROR !\r\n@FAIL@\r\n");
        construct_oam_responsepkt(oammsg, oamresp);
#if DEBUG
       debug_msg((char *)oamresp->resppkt);
#endif
       memcpy(response_buf, oamresp->resppkt, oamresp->totallen );
       goto endx;
    }
	//////////////////////////////////////////////////////////////////////
#if DEBUG
	if(u8channo == 99)
	{
		debug_msg((char *)"ALL CHANS, OUTPUT PULSE  %s\r\n", argv[2]);
	}
	else
	{
		memset(buf,0,sizeof(buf));
		debug_msg((char *)"CHAN NO %d, OUTPUT PULSE %s\r\n", u8channo, argv[2] );
	}
#endif
	//////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    //Normal Execution...APPLY
    flg = program_pw(u8channo, u8pwset, 1);
    if(flg != 0)
    {
        memset(oammsg,0,MAX_RESPSTRING_SIZE);
        sprintf((char *)oammsg, "Error Config PW Logic !\r\nSYSTEM ERROR !\r\n@FAIL@\r\n");
        construct_oam_responsepkt(oammsg, oamresp);
#if DEBUG
       debug_msg((char *)oamresp->resppkt);
#endif
       memcpy(response_buf, oamresp->resppkt, oamresp->totallen );
       goto endx;

    }
    //////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////
	//VALID RESPONSE
	memset(oammsg,0,MAX_RESPSTRING_SIZE);
	sprintf((char *)oammsg,"SUCCESS\r\n@OK@\r\n");
	construct_oam_responsepkt(oammsg, oamresp);
	memcpy(response_buf, oamresp->resppkt, oamresp->totallen );
	#if DEBUG
	debug_msg("Sent Response to OAM...\r\n");
	debug_msg((char *)response_buf);
	ajUART0crlf();
	#endif
	//
	ret = RET_SUCCESS;
	//////////////////////////////////////////////////////////////////////
	///////////////////////////////////////
	endx:
	vPortFree(oammsg);
	vPortFree(oamresp);
	return ret;
	///////////////////////////////////////
}

int handle_setoutputsource(int argc, char **argv, uint8_t cmdkwlen, char *response_buf)
{
    //possibilities
    //setoutputsource
    //setoutputsource help
    //setoutputsource ?
    //setoutputsource <ChanNo> <MapInput> <CR>
    int c1 = 0;
    int c1x = 0;
    int selhelp = 0;
    uint16_t u16channo;
    uint16_t u16mapinput;
    uint8_t lenstr1; //len of string argument 1
    uint8_t lenstr2; //len of string argument 2
    //
    int ret = RET_FAIL;
    uint16_t superlen,msglen ;
    char buf[100];
    int j;
    int flg;
    //
    uint8_t u8channo;
    uint8_t u8mapinput;
    //
    //////////////////////////////////////////////////////////////////////////
    uint8_t *oammsg = (uint8_t *) pvPortMalloc(sizeof(uint8_t) * MAX_RESPSTRING_SIZE);
    if (oammsg == NULL)
    {
        debug_msg("\r\nFAIL Alloc Mem - 'oammsg'\r\n");
        while (1)
            ;   //stuck here
    }
    VALIDRESP_T *oamresp = (VALIDRESP_T *) pvPortMalloc(sizeof(VALIDRESP_T));
    if(oamresp == NULL)
    {
        debug_msg("\r\nFAIL Alloc Mem - 'VALIDRESP_T'\r\n");
        while (1)
            ;   //stuck here
    }
    //////////////////////////////////////////////////////////////////////////
    ///////////////////////////////
    //arg check and help
    if (argc != 1 && argc != 2 && argc != 3)
    {
        memset(oammsg,0,MAX_RESPSTRING_SIZE);
        sprintf((char *)oammsg, "Incorrect Argument Count !\r\n");
        strcat((char *)oammsg, "SYNTAX ERROR\r\n@FAIL@\r\n");
        construct_oam_responsepkt(oammsg, oamresp);
#if DEBUG
        debug_msg((char *)oamresp->resppkt);
#endif
        memcpy(response_buf, oamresp->resppkt, oamresp->totallen );
        goto endx;
    }
    //check...this could be 'help'
    selhelp = 0;
    if (argc == 1)
    {
        selhelp = 1;
    }
    else if (argc == 2)
    {
        if ((argc == 2) && (argv[1] == NULL))
        {
            memset(oammsg,0,MAX_RESPSTRING_SIZE);
            sprintf((char *)oammsg, "Execution Error !\r\n@FAIL@\r\n");
            construct_oam_responsepkt(oammsg, oamresp);
#if DEBUG
           debug_msg((char *)oamresp->resppkt);
#endif
           memcpy(response_buf, oamresp->resppkt, oamresp->totallen );
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
                construct_oam_responsepkt(oammsg, oamresp);
    #if DEBUG
               debug_msg((char *)oamresp->resppkt);
    #endif
               memcpy(response_buf, oamresp->resppkt, oamresp->totallen );
               goto endx;
            }
        }
    }
    if(selhelp != 0)
    {
        construct_oam_large_template_responsepkt((uint8_t *)response_buf, RESPONSE_BUF_SIZE);
        strcat(response_buf, setoutputsource.help_str);
        //
        memset(buf,0,sizeof(buf));
        sprintf(buf, "Cmd String <%s> has 2 Args :\r\n", setoutputsource.command_name);
        strcat(response_buf,buf);
        //
        strcat(response_buf, "Arg-1 is <ChanNo> 1-16 or 'all'\r\n");
        strcat(response_buf, "Arg-2 is <MapInputNo> \r\n");
        strcat(response_buf, "Valid Settings are (any of the 4 INPUTS): 1 - 4\r\n");
        //
         memset(buf,0,sizeof(buf));
         sprintf(buf, "E.G. %s 16 4 <CR>\r\n", setoutputsource.command_name);
         strcat(response_buf,buf);
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
    //check arg[1] ChanNo 1-16 or 'all'
    lenstr1 = strlen(argv[1]);
    if(lenstr1 < 1 || lenstr1 > MAX_CMDSTRING_CHARS)
    {
        memset(oammsg,0,MAX_RESPSTRING_SIZE);
        sprintf((char *)oammsg, "Illegal Argument 1 String Length !\r\nSYNTAX ERROR !\r\n@FAIL@\r\n");
#if DEBUG
        debug_msg((char *)oammsg);
#endif
        construct_oam_responsepkt(oammsg, oamresp);
        memcpy(response_buf, oamresp->resppkt, oamresp->totallen );
        goto endx;
    }
    if(lenstr1 == 3)
    {
        c1 = strncmp(argv[1], "all",strlen("all"));
        if(c1 == 0)
        {
            u16channo = 99;
        }
        goto allchans;
    }
    ////////////////////////////////////////////////////////////////
    flg = ConvertStrtoInt16(argv[1], lenstr1, &u16channo);
    if(flg != RET_SUCCESS)
    {
        memset(oammsg,0,MAX_RESPSTRING_SIZE);
        sprintf((char *)oammsg,"Illegal Param CHAN No !\r\nSYSTEM ERROR !\r\n@FAIL@\r\n");
#if DEBUG
        debug_msg((char *)oammsg);
#endif
        construct_oam_responsepkt(oammsg, oamresp);
        memcpy(response_buf, oamresp->resppkt, oamresp->totallen );
        goto endx;
    }
    ///////////////////////////////////////////////////////////////////////////
    if(u16channo < MIN_OUTCHANS || u16channo > MAX_OUTCHANS)
    {
        memset(oammsg,0,MAX_RESPSTRING_SIZE);
        sprintf((char *)oammsg, "Illegal <ChanNo> %d-%d or 'all' expected !\r\nSYNTAX ERROR !\r\n@FAIL@\r\n", MIN_OUTCHANS, MAX_OUTCHANS);
#if DEBUG
        debug_msg((char *)oammsg);
#endif
        construct_oam_responsepkt(oammsg, oamresp);
        memcpy(response_buf, oamresp->resppkt, oamresp->totallen );
        goto endx;
    }
    //////////////////////////////////////////////////////////////////////
    allchans:
    u8channo = u16channo;
    ///////////////////////////////////////////////////////////////////////////
    //check arg[2]
    lenstr2 = strlen(argv[2]);
    if(lenstr2 < 1 || lenstr2 > MAX_CMDSTRING_CHARS)
    {
        memset(oammsg,0,MAX_RESPSTRING_SIZE);
        sprintf((char *)oammsg, "Illegal Argument 2 String Length !\r\nSYNTAX ERROR !\r\n@FAIL@\r\n");
#if DEBUG
        debug_msg((char *)oammsg);
#endif
        construct_oam_responsepkt(oammsg, oamresp);
        memcpy(response_buf, oamresp->resppkt, oamresp->totallen );
        goto endx;
    }
    ////////////////////////////////////////////////////////////////
    flg = ConvertStrtoInt16(argv[2], lenstr2, &u16mapinput);
    if(flg != RET_SUCCESS)
    {
        memset(oammsg,0,MAX_RESPSTRING_SIZE);
        sprintf((char *)oammsg,"Illegal Param MapInput No !\r\nSYSTEM ERROR !\r\n@FAIL@\r\n");
#if DEBUG
        debug_msg((char *)oammsg);
#endif
        construct_oam_responsepkt(oammsg, oamresp);
        memcpy(response_buf, oamresp->resppkt, oamresp->totallen );
        goto endx;
    }
    ///////////////////////////////////////////////////////////////////////////
    if(u16mapinput < MIN_INCHANS || u16mapinput > MAX_INCHANS)
    {
        memset(oammsg,0,MAX_RESPSTRING_SIZE);
        sprintf((char *)oammsg, "Illegal <ChanNo> %d-%d expected !\r\nSYNTAX ERROR !\r\n@FAIL@\r\n", MIN_INCHANS, MAX_INCHANS);
#if DEBUG
        debug_msg((char *)oammsg);
#endif
        construct_oam_responsepkt(oammsg, oamresp);
        memcpy(response_buf, oamresp->resppkt, oamresp->totallen );
        goto endx;
    }
    ///////////////////////////////////////////////////////////////////////////
    u8mapinput = u16mapinput;
    ///////////////////////////////////////////////////////////////////////////
    flg = EEPROM_OAMConfig_outputsource(u8channo, u8mapinput);
    if(flg != 0)
    {
       memset(oammsg,0,MAX_RESPSTRING_SIZE);
        sprintf((char *)oammsg, "Error Saving Config in EEPROM !\r\nSYSTEM ERROR !\r\n@FAIL@\r\n");
        construct_oam_responsepkt(oammsg, oamresp);
#if DEBUG
       debug_msg((char *)oamresp->resppkt);
#endif
       memcpy(response_buf, oamresp->resppkt, oamresp->totallen );
       goto endx;
    }
    //////////////////////////////////////////////////////////////////////
#if DEBUG
    if(u8channo == 99)
    {
        debug_msg((char *)"ALL CHANS, OUTPUT SOURCE  %d\r\n", u8mapinput);
    }
    else
    {
        memset(buf,0,sizeof(buf));
        debug_msg((char *)"CHAN NO %d, OUTPUT SOURCE %d\r\n", u8channo, u8mapinput );
    }
#endif
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    //Normal Execution...APPLY
    flg = program_map(u8channo, u8mapinput, 1);
    if(flg != 0)
    {
        memset(oammsg,0,MAX_RESPSTRING_SIZE);
        sprintf((char *)oammsg, "Error Config MAP Logic !\r\nSYSTEM ERROR !\r\n@FAIL@\r\n");
        construct_oam_responsepkt(oammsg, oamresp);
#if DEBUG
       debug_msg((char *)oamresp->resppkt);
#endif
       memcpy(response_buf, oamresp->resppkt, oamresp->totallen );
       goto endx;

    }
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    //VALID RESPONSE
    memset(oammsg,0,MAX_RESPSTRING_SIZE);
    sprintf((char *)oammsg,"SUCCESS\r\n@OK@\r\n");
    construct_oam_responsepkt(oammsg, oamresp);
    memcpy(response_buf, oamresp->resppkt, oamresp->totallen );
    #if DEBUG
    debug_msg("Sent Response to OAM...\r\n");
    debug_msg((char *)response_buf);
    ajUART0crlf();
    #endif
    //
    ret = RET_SUCCESS;
    //////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////
    endx:
    vPortFree(oammsg);
    vPortFree(oamresp);
    return ret;
    ///////////////////////////////////////
}

int handle_resetcard(int argc, char **argv, uint8_t cmdkwlen, char *response_buf)
{
    //possibilities
    //setcomdefaults
    //setcomdefaults help
    //setcomdefaults ?
    //setcomdefaults exe
    int c1 = 0;
    int c1x = 0;
    int c2 = 0;
    int selhelp = 0;
    //
    int ret = RET_FAIL;
    uint16_t superlen,msglen ;
    char buf[100];
    int j;
    //
    int flg;
    //================================================================================
    //
    //////////////////////////////////////////////////////////////////////////
    uint8_t *oammsg = (uint8_t *) pvPortMalloc(sizeof(uint8_t) * MAX_RESPSTRING_SIZE);
    if (oammsg == NULL)
    {
        debug_msg("\r\nFAIL Alloc Mem - 'oammsg'\r\n");
        while (1)
            ;   //stuck here
    }
    ///////////////////////////////////////////////////////////////////////////
    VALIDRESP_T *oamresppkt = (VALIDRESP_T *) pvPortMalloc(sizeof(VALIDRESP_T));
    if(oamresppkt == NULL)
    {
        debug_msg("\r\nFAIL Alloc Mem - 'VALIDRESP_T'\r\n");
        while (1)
            ;   //stuck here
    }
    ///////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////
    ///////////////////////////////
    //arg check and help
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
        selhelp = 1;
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
                c2 = strncmp(argv[1], "exe",strlen("exe"));
                if(c2 != 0)
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
    }
    if(selhelp != 0)
    {
        construct_oam_large_template_responsepkt((uint8_t *)response_buf, RESPONSE_BUF_SIZE);
         strcat(response_buf, setcomdefaults.help_str);
         strcat(response_buf, "\r\n");
        //
        memset(buf,0,sizeof(buf));
        sprintf(buf, "Cmd String <%s> has 1 Arg :\r\n", setcomdefaults.command_name);
        strcat(response_buf, buf);
        //
        //ARG-1, <Group>
        strcat(response_buf, "Arg-1 a fixed string for execution 'exe'\r\n");
        memset(buf,0,sizeof(buf));
        sprintf(buf, "E.G. %s exe\r\n", setcomdefaults.command_name);
        strcat(response_buf, buf);
         //
         strcat(response_buf, "Device shall Re-START Card stored configuration is applied\r\n");
         //
        strcat((char *)response_buf, "@OK@\r\n##");
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
 #if DEBUG
        debug_msg((char *)response_buf);
 #endif
         /////////////////////
         ret = RET_SUCCESS;
         goto endx;
    }
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    //////////////////////////////////////////////////////////////////////
    //Normal Execution
    //do nothing
    /////////////////////////////
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //////////////////////////////////////////////////////////////////////
    //VALID RESPONSE
    ret = RET_SUCCESS;
    memset(oammsg,0,MAX_RESPSTRING_SIZE);
    sprintf((char *)oammsg,"SUCCESS\r\n@OK@\r\n");
    construct_oam_responsepkt(oammsg, oamresppkt);
    memcpy(response_buf, oamresppkt->resppkt, oamresppkt->totallen );
    //
    //////////////////////////////////////////////////////////////////////
    //Normal Execution, first send response then...just BOOTUP
    //VALID RESPONSE
    memset(oammsg,0,MAX_RESPSTRING_SIZE);
    sprintf((char *)oammsg,"Card Boot up under progress...\r\nPlease Wait...\r\n@OK@\r\n");
    construct_oam_responsepkt(oammsg, oamresppkt);
    memcpy(response_buf, oamresppkt->resppkt, oamresppkt->totallen );
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    ajUART0Write(response_buf);
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    vPortFree(oammsg);
    vPortFree(oamresppkt);
    vTaskDelay(500);
    //drive_cccard_ready(0);
    vTaskDelay( 500 );
    //
    HWREG(NVIC_APINT) = NVIC_APINT_VECTKEY | NVIC_APINT_SYSRESETREQ;
    //
    ////////////////
    endx:
    vPortFree(oammsg);
    vPortFree(oamresppkt);
    return ret;
    ////////////////
    //
}

int handle_cleartripcounters(int argc, char **argv, uint8_t cmdkwlen, char *response_buf)
{
    //possibilities
    //cleartripcounters
    //cleartripcounters help
    //cleartripcounters ?
    //cleartripcounters exe
    int c1 = 0;
    int c1x = 0;
    int c2 = 0;
    int selhelp = 0;
    //
    int ret = RET_FAIL;
    uint16_t superlen,msglen ;
    char buf[100];
    int j;
    //
    int flg;
    //================================================================================
    //
    //////////////////////////////////////////////////////////////////////////
    uint8_t *oammsg = (uint8_t *) pvPortMalloc(sizeof(uint8_t) * MAX_RESPSTRING_SIZE);
    if (oammsg == NULL)
    {
        debug_msg("\r\nFAIL Alloc Mem - 'oammsg'\r\n");
        while (1)
            ;   //stuck here
    }
    ///////////////////////////////////////////////////////////////////////////
    VALIDRESP_T *oamresppkt = (VALIDRESP_T *) pvPortMalloc(sizeof(VALIDRESP_T));
    if(oamresppkt == NULL)
    {
        debug_msg("\r\nFAIL Alloc Mem - 'VALIDRESP_T'\r\n");
        while (1)
            ;   //stuck here
    }
    ///////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////
    ///////////////////////////////
    //arg check and help
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
        selhelp = 1;
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
                c2 = strncmp(argv[1], "exe",strlen("exe"));
                if(c2 != 0)
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
    }
    if(selhelp != 0)
    {
        construct_oam_large_template_responsepkt((uint8_t *)response_buf, RESPONSE_BUF_SIZE);
         strcat(response_buf, cleartripcounters.help_str);
         strcat(response_buf, "\r\n");
        //
        memset(buf,0,sizeof(buf));
        sprintf(buf, "Cmd String <%s> has 1 Arg :\r\n", cleartripcounters.command_name);
        strcat(response_buf, buf);
        //
        //ARG-1, <Group>
        strcat(response_buf, "Arg-1 a fixed string for execution 'exe'\r\n");
        memset(buf,0,sizeof(buf));
        sprintf(buf, "E.G. %s exe\r\n", cleartripcounters.command_name);
        strcat(response_buf, buf);
         //
         strcat(response_buf, "All TRIP Counts are cleared and Fetch Alarm Queue is cleared\r\n");
         //
        strcat((char *)response_buf, "@OK@\r\n##");
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
 #if DEBUG
        debug_msg((char *)response_buf);
 #endif
         /////////////////////
         ret = RET_SUCCESS;
         goto endx;
    }
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    //////////////////////////////////////////////////////////////////////
    //Normal Execution
    init_tripcntrs_mutex();
    flush_fetchalarms_mutex();
    clear_alarmrelay();
    /////////////////////////////
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //////////////////////////////////////////////////////////////////////
    //VALID RESPONSE
    ret = RET_SUCCESS;
    memset(oammsg,0,MAX_RESPSTRING_SIZE);
    sprintf((char *)oammsg,"SUCCESS\r\n@OK@\r\n");
    construct_oam_responsepkt(oammsg, oamresppkt);
    memcpy(response_buf, oamresppkt->resppkt, oamresppkt->totallen );
    //
    //////////////////////////////////////////////////////////////////////
    ///////////////////////////////
    endx:
    vPortFree(oammsg);
    vPortFree(oamresppkt);
    return ret;
    ///////////////////////////////
    //
}



void init_difcommand_list_core_comsettings(void)
{
	//cli group CLIGROUP_CORECOMSET
	//1- 7(7nos)
	add_command_to_list(&setcomdefaults);
	add_command_to_list(&setcomtitle);
	add_command_to_list(&setglitchfilter);
	add_command_to_list(&setoutputpulse);
	add_command_to_list(&setoutputsource);
	add_command_to_list(&resetcard);
	add_command_to_list(&cleartripcounters);
}


