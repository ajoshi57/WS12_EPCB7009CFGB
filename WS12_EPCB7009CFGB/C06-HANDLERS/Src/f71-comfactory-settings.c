/****************************************************
  FILE NAME		: f71-comfactory-settings.c
   ORIGINATOR	: A Joshi
  LAST EDITED   : 10 OCT 2026
  PROJECT       : WS12_EPCB7011CFGB
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/

#include "f71-comfactory-settings.h"
//
#define DEBUG 1
//
extern SERIALDBASE_T SERIAL_DBASE;
extern OAMCFGDBASE_T OAM_CONFIG_DBASE;

int handle_setcomserial(int argc, char **argv, uint8_t cmdkwlen, char *response_buf);
int handle_setcomdesc(int argc, char **argv, uint8_t cmdkwlen, char *response_buf);
int handle_wipemem(int argc, char **argv, uint8_t cmdkwlen, char *response_buf);
int handle_logici2cwrrd(int argc, char **argv, uint8_t cmdkwlen, char *response_buf);
//

void init_difcommand_list_comfactory_settings(void);


CMDMSG_T setcomserial =
{
		"#ffsetserial#", //cmdstr
		2,
		"Set Card Serial Number String\r\n", //help_str
		(CLIGROUP_COMFACTORYSET + 1), //command_id
		AUTH_SUPERUSER,         //auth_level
		&handle_setcomserial      //handler fn
};

CMDMSG_T setcomdesc =
{
		"#ffsetdesc#", //cmdstr
		2,
		"Set Card Description String\r\n", //help_str
		(CLIGROUP_COMFACTORYSET + 2), //command_id
		AUTH_SUPERUSER,         //auth_level
		&handle_setcomdesc     //handler fn
};

CMDMSG_T wipemem =
{
        "#ffwipemem#", //cmdstr
        2,
        "CLEAR FACTORY and/or CONFIGURATION Memory\r\n", //help_str
        (CLIGROUP_COMFACTORYSET + 3), //command_id
        AUTH_SUPERUSER,        //auth_level
        &handle_wipemem       //handler fn
};

CMDMSG_T logici2cwrrd =
{
        "#fflogici2cwrrd#", //cmdstr
        2,
        "FACTORY TEST LOGIC I2C Registers Write/Read TEST repeated <Count> times\r\n", //help_str
        (CLIGROUP_COMFACTORYSET + 4), //command_id
        AUTH_SUPERUSER,        //auth_level
        &handle_logici2cwrrd       //handler fn
};


int handle_setcomserial(int argc, char **argv, uint8_t cmdkwlen, char *response_buf)
{
	//possibilities
	//setcomserial
	//setcomserial help
	//setcomserial ?
	//setcomserial <Serial No String>
	int c1 = 0;
	int c1x = 0;
	int selhelp = 0;
	int ret = RET_FAIL;
	//////////////////////////////////////////////////////////////////////////
	uint8_t lenarg1;
	//////////////////////////////////////////////////////////////////////////
	//
	uint16_t superlen,msglen ;
	char buf[100];
	int j;
	int flg;
	uint8_t cardserial[24];
	uint8_t tcardserial[24];
	///////////////////////////////////////////////////////////////////////////
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
    else if (argc == 2)
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
        }
    }
    if(selhelp != 0)
    {
		 construct_oam_large_template_responsepkt((uint8_t *)response_buf, RESPONSE_BUF_SIZE);
         strcat(response_buf, "\r\n");
         strcat(response_buf, setcomserial.help_str);
         strcat(response_buf, "\r\n");
         //
         memset(buf,0,sizeof(buf));
         sprintf(buf, "Cmd String <%s> has 2 Args :\r\n", setcomserial.command_name);
         strcat(response_buf, buf);
         //
         strcat(response_buf, "Arg-1 is <CardNo> '1-2'\r\n");
         strcat(response_buf, "Arg-2 is a String Argument representing Card Serial Number\r\n");
         strcat(response_buf, "Serial String could be the card unique serial identity\r\n");
         strcat(response_buf, "This string shall displayed by the #ffsetcomserial# command\r\n");
         strcat(response_buf, "String of upto 2-20 chars without spaces\r\n");
         strcat(response_buf, "Use Underscore chars where spaces are required\r\n");
         //
         memset(buf,0,sizeof(buf));
         sprintf(buf, "E.G. %s <CardNo> <Serial No String> <CR>\r\n", setcomserial.command_name);
         strcat(response_buf,buf);
         //
         memset(buf,0,sizeof(buf));
         sprintf(buf, "E.G. %s 1 SERNO_1001 <CR>\r\n", setcomserial.command_name);
         strcat(response_buf,buf);
         //
         memset(buf,0,sizeof(buf));
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
    }
    //
    //////////////////////////////////////////////////////////////////////
    //Check Args
    //////////////////////////////////////////////////
	//check arg[1]
    //////////////////////////////////////////////////
    memset(tcardserial,0,sizeof(tcardserial));
    memset(cardserial,0,sizeof(cardserial));
    //////////////////////////////////////////////////
	lenarg1 = strlen((char *)argv[1]);
	debug_msg("ser len %d\r\n", lenarg1);
	/////////////////////////////////////////////
	if(lenarg1 < MIN_CMDSTRING_CHARS || lenarg1 > MAX_CMDSTRING_CHARS)
	{
		memset(oammsg,0,MAX_RESPSTRING_SIZE);
		sprintf((char *)oammsg,"Illegal Serial String size, 2-20 expected !\r\nSYNTAX ERROR !\r\n@FAIL@\r\n");
		ret = RET_FAIL;
		construct_oam_responsepkt(oammsg, oamresppkt);
#if DEBUG
       debug_msg((char *)oamresppkt->resppkt);
#endif
       memcpy(response_buf, oamresppkt->resppkt, oamresppkt->totallen );
	   goto endx;
	}
	//
	//////////////////////////////////////////////////////////////////////
	strcpy((char *)tcardserial, argv[1]);
	//convert to CAPS
	//////////////////////////////
	for(j=0;j<lenarg1;j++)
	{
		if(tcardserial[j] >=0x61 && tcardserial[j] <= 0x7a)
		{
			//a-z...make cAPS
			cardserial[j] = tcardserial[j] - 0x20;
		}
		else
		{
			cardserial[j] = tcardserial[j];
		}
	}
	debug_msg("new cardserial %s len %d\r\n", cardserial, lenarg1);
	////////////////////////////////////////////////////////////////
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	////////////////////////////////////////////////////////////////
	//Normal Execution...Modify CARD Serial
	flg = EEPROM_Serial_serno(&cardserial[0], lenarg1);
	if(flg != 0)
	{
		ret = RET_FAIL;
		memset(oammsg,0,MAX_RESPSTRING_SIZE);
		sprintf((char *)oammsg, "Error Saving in EEPROM !\r\nSYSTEM ERROR !\r\n@FAIL@\r\n");
		construct_oam_responsepkt(oammsg, oamresppkt);
#if DEBUG
       debug_msg((char *)oamresppkt->resppkt);
#endif
       memcpy(response_buf, oamresppkt->resppkt, oamresppkt->totallen );
       goto endx;
	}
	//////////////////////////////////////////////////////////////////////
#if DEBUG
	debug_msg((char *)"Card serial number %s\r\n", cardserial);
#endif
	//VALID RESPONSE
	memset(oammsg,0,MAX_RESPSTRING_SIZE);
	sprintf((char *)oammsg,"\r\nSUCCESS\r\n@OK@\r\n");
	construct_oam_responsepkt(oammsg, oamresppkt);
	memcpy(response_buf, oamresppkt->resppkt, oamresppkt->totallen );
	#if DEBUG
	debug_msg("Sent Response to OAM...\r\n");
	debug_msg((char *)response_buf);
	ajUART0crlf();
	#endif
	//
	ret = RET_SUCCESS;
	//////////////////////////////////////////////////////////////////////
	///////////////////////////////
	endx:
	vPortFree(oammsg);
	vPortFree(oamresppkt);
	return ret;
	///////////////////////////////
}


int handle_setcomdesc(int argc, char **argv, uint8_t cmdkwlen, char *response_buf)
{
	//possibilities
	//setcomdesc
	//setcomdesc help
	//setcomdesc ?
	//setcomdesc <Description String>
	int c1 = 0;
	int c1x = 0;
	int selhelp = 0;
	int ret = RET_FAIL;
	//////////////////////////////////////////////////////////////////////////
	uint8_t lenarg1;
	//////////////////////////////////////////////////////////////////////////
	//
	uint16_t superlen,msglen ;
	char buf[100];
	int j;
	int flg;
	uint8_t tcarddesc[24];
	uint8_t carddesc[24];
	///////////////////////////////////////////////////////////////////////////
	uint8_t *oammsg = (uint8_t *) pvPortMalloc(sizeof(uint8_t) * MAX_RESPSTRING_SIZE);
	if (oammsg == NULL)
	{
		debug_msg("\r\nhandle_difsetcarddesc: FAIL Alloc Mem - 'oammsg'\r\n");
		while (1)
			;	//stuck here
	}
	///////////////////////////////////////////////////////////////////////////
	VALIDRESP_T *oamresppkt = (VALIDRESP_T *) pvPortMalloc(sizeof(VALIDRESP_T));
	if(oamresppkt == NULL)
	{
		debug_msg("\r\nhandle_difsetcarddesc: FAIL Alloc Mem - 'VALIDRESP_T'\r\n");
		while (1)
			;	//stuck here
	}
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
    else if (argc == 2)
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
        }
    }
    if(selhelp != 0)
    {
		 construct_oam_large_template_responsepkt((uint8_t *)response_buf, RESPONSE_BUF_SIZE);
         strcat(response_buf, "\r\n");
         strcat(response_buf, setcomdesc.help_str);
         strcat(response_buf, "\r\n");
         //
         memset(buf,0,sizeof(buf));
         sprintf(buf, "Cmd String <%s> has 2 Args :\r\n", setcomdesc.command_name);
         strcat(response_buf, buf);
         //
         strcat(response_buf, "Arg-1 is <CardNo> '1-2'\r\n");
         strcat(response_buf, "Arg-2 is a String Argument representing Card Description\r\n");
         strcat(response_buf, "Description String could be the card h/w identity\r\n");
         strcat(response_buf, "This string shall displayed by the #ffsetcomdesc# command\r\n");
         strcat(response_buf, "String of upto 2-20 chars without spaces\r\n");
         strcat(response_buf, "Use Underscore chars where spaces are required\r\n");
         //
         memset(buf,0,sizeof(buf));
         sprintf(buf, "E.G. %s <CardNo> <Desc String><CR>\r\n", setcomdesc.command_name );
         strcat(response_buf, buf);
         //
         memset(buf,0,sizeof(buf));
         sprintf(buf, "E.G. %s 1 EPCB-2737-O-00<CR>\r\n", setcomdesc.command_name);
         strcat(response_buf, buf);
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
    //////////////////////////////////////////////////////////////////////
    //Check Args
	//check arg[1]
    //////////////////////////////////////////////////
    memset(tcarddesc,0,sizeof(tcarddesc));
    memset(carddesc,0,sizeof(carddesc));
    //////////////////////////////////////////////////
	lenarg1 = strlen((char *)argv[1]);
	debug_msg("Card Desc len %d\r\n", lenarg1);
	/////////////////////////////////////////////
	if(lenarg1 < MIN_CMDSTRING_CHARS || lenarg1 > MAX_CMDSTRING_CHARS)
	{
		memset(oammsg,0,MAX_RESPSTRING_SIZE);
		sprintf((char *)oammsg,"Illegal Description String size, 2-20 expected !\r\nSYNTAX ERROR !\r\n@FAIL@\r\n");
		ret = RET_FAIL;
		construct_oam_responsepkt(oammsg, oamresppkt);
#if DEBUG
       debug_msg((char *)oamresppkt->resppkt);
#endif
       memcpy(response_buf, oamresppkt->resppkt, oamresppkt->totallen );
	   goto endx;
	}
	//
	//////////////////////////////////////////////////////////////////////
	strcpy((char *)tcarddesc, (char *)argv[1]);
	//convert to CAPS
	//////////////////////////////
	for(j=0;j<lenarg1;j++)
	{
		if(tcarddesc[j] >=0x61 && tcarddesc[j] <= 0x7a)
		{
			//a-z...make cAPS
			carddesc[j] = tcarddesc[j] - 0x20;
		}
		else
		{
			carddesc[j] = tcarddesc[j];
		}
	}
	debug_msg("new card desc %s len %d\r\n", carddesc, lenarg1);
	//////////////////////////////////////////////////////////////////////
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//////////////////////////////////////////////////////////////////////
	//Normal Execution...Modify CARD Serial
	flg = EEPROM_Serial_desc(&carddesc[0], lenarg1);
	if(flg != 0)
	{
		ret = RET_FAIL;
		memset(oammsg,0,MAX_RESPSTRING_SIZE);
		sprintf((char *)oammsg, "Error Saving in EEPROM !\r\nSYSTEM ERROR !\r\n@FAIL@\r\n");
		construct_oam_responsepkt(oammsg, oamresppkt);
#if DEBUG
       debug_msg((char *)oamresppkt->resppkt);
#endif
       memcpy(response_buf, oamresppkt->resppkt, oamresppkt->totallen );
       goto endx;
	}
	//
	//////////////////////////////////////////////////////////////////////
	//VALID RESPONSE
	memset(oammsg,0,MAX_RESPSTRING_SIZE);
	sprintf((char *)oammsg,"\r\nSUCCESS\r\n@OK@\r\n");
	construct_oam_responsepkt(oammsg, oamresppkt);
	memcpy(response_buf, oamresppkt->resppkt, oamresppkt->totallen );
	#if DEBUG
	debug_msg("Sent Response to OAM...\r\n");
	debug_msg((char *)response_buf);
	ajUART0crlf();
	#endif
	//
	ret = RET_SUCCESS;
	//////////////////////////////////////////////////////////////////////
	///////////////////////////////
	endx:
	vPortFree(oammsg);
	vPortFree(oamresppkt);
	return ret;
	///////////////////////////////
}

int handle_wipemem(int argc, char **argv, uint8_t cmdkwlen, char *response_buf)
{
    //possibilities
    //wipemem
    //wipemem help
    //wipemem ?
    //wipemem <MemID>
    int c1 = 0;
    int c1x = 0;
    int selhelp = 0;
    int ret = RET_FAIL;
    //////////////////////////////////////////////////////////////////////////
    uint8_t lenarg1;
    //////////////////////////////////////////////////////////////////////////
    //
    uint16_t superlen,msglen ;
    char buf[100];
    int j;
    int flg;
    //
    uint8_t memid;
    int fact,cfg,both;
    //////////////////////////////
    ///////////////////////////////////////////////////////////////////////////
    uint8_t *oammsg = (uint8_t *) pvPortMalloc(sizeof(uint8_t) * MAX_RESPSTRING_SIZE);
    if (oammsg == NULL)
    {
        debug_msg("\r\nhandle_difsetcarddesc: FAIL Alloc Mem - 'oammsg'\r\n");
        while (1)
            ;   //stuck here
    }
    ///////////////////////////////////////////////////////////////////////////
    VALIDRESP_T *oamresppkt = (VALIDRESP_T *) pvPortMalloc(sizeof(VALIDRESP_T));
    if(oamresppkt == NULL)
    {
        debug_msg("\r\nhandle_difsetcarddesc: FAIL Alloc Mem - 'VALIDRESP_T'\r\n");
        while (1)
            ;   //stuck here
    }
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
    else if (argc == 2)
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
        }
    }
    if(selhelp != 0)
    {
         construct_oam_large_template_responsepkt((uint8_t *)response_buf, RESPONSE_BUF_SIZE);
         strcat(response_buf, "\r\n");
         strcat(response_buf, wipemem.help_str);
         strcat(response_buf, "\r\n");
         //
         memset(buf,0,sizeof(buf));
         sprintf(buf, "Cmd String <%s> has 2 Args :\r\n", wipemem.command_name);
         strcat(response_buf, buf);
         //
         strcat(response_buf, "Arg-1 is <MemID> 'fact', 'cfg' or 'both'\r\n");
         strcat(response_buf, "WIPES one or both Memory spaces clean\r\n");
         strcat(response_buf, "CLI cmd '#setdefault#' maybe used for DEFAULT Configuration\r\n");
         strcat(response_buf, "CLI cmds for FACTORY SETTINGS maybe used to setup desired serial and description\r\n");
         //
         memset(buf,0,sizeof(buf));
         sprintf(buf, "E.G. %s fact <CR>\r\n", wipemem.command_name );
         strcat(response_buf, buf);
         //
         memset(buf,0,sizeof(buf));
         sprintf(buf, "E.G. %s cfg <CR>\r\n", wipemem.command_name );
         strcat(response_buf, buf);
         //
         memset(buf,0,sizeof(buf));
         sprintf(buf, "E.G. %s both <CR>\r\n", wipemem.command_name );
         strcat(response_buf, buf);
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
    //////////////////////////////////////////////////////////////////////
    //check arg[1]
    //////////////////////////////////////////////////
    lenarg1 = strlen((char *)argv[1]);
    debug_msg("MemID len %d\r\n", lenarg1);
    /////////////////////////////////////////////
    if(lenarg1 < MIN_CMDSTRING_CHARS || lenarg1 > MAX_CMDSTRING_CHARS)
    {
        memset(oammsg,0,MAX_RESPSTRING_SIZE);
        sprintf((char *)oammsg,"Illegal Description String size, 2-20 expected !\r\nSYNTAX ERROR !\r\n@FAIL@\r\n");
        ret = RET_FAIL;
        construct_oam_responsepkt(oammsg, oamresppkt);
#if DEBUG
       debug_msg((char *)oamresppkt->resppkt);
#endif
       memcpy(response_buf, oamresppkt->resppkt, oamresppkt->totallen );
       goto endx;
    }
    //
    //////////////////////////////////////////////////////////////////////
    fact = strncmp(argv[1], "fact",strlen("fact"));
    cfg = strncmp(argv[1], "cfg",strlen("cfg"));
    both = strncmp(argv[1], "both",strlen("both"));
    if (fact == 0 && lenarg1 == 4)
    {
        memid = WIPEMEM_FACT;
    }
    else if (cfg == 0 && lenarg1== 3)
    {
        memid = WIPEMEM_CFG;
    }
    else if (both == 0 && lenarg1== 4)
    {
        memid = WIPEMEM_BOTH;
    }
    else
    {
        memset(oammsg,0,MAX_RESPSTRING_SIZE);
        sprintf((char *)oammsg, "Illegal Argument String ('fact' or 'cfg' or 'both' expected) !\r\nSYNTAX ERROR !\r\n@FAIL@\r\n");
#if DEBUG
        debug_msg((char *)oammsg);
#endif
        construct_oam_responsepkt(oammsg, oamresppkt);
        memcpy(response_buf, oamresppkt->resppkt, oamresppkt->totallen );
        goto endx;
    }
    //////////////////////////////////////////////////////////////////////
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //////////////////////////////////////////////////////////////////////
    //Normal Execution...wipe mem
    /////////////////////////////
    //setup test memory to default
    flg = EEPROM_TEST_default();
    if(flg != 0)
    {
        ret = RET_FAIL;
        memset(oammsg,0,MAX_RESPSTRING_SIZE);
        sprintf((char *)oammsg, "Error Saving in EEPROM !\r\nSYSTEM ERROR !\r\n@FAIL@\r\n");
        construct_oam_responsepkt(oammsg, oamresppkt);
#if DEBUG
       debug_msg((char *)oamresppkt->resppkt);
#endif
       memcpy(response_buf, oamresppkt->resppkt, oamresppkt->totallen );
       goto endx;
    }
    if(memid == WIPEMEM_FACT || memid == WIPEMEM_BOTH)
    {
        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        //wipe serial...but program valid flag
        flg = EEPROM_Serial_default();
        if(flg != 0)
        {
            ret = RET_FAIL;
            memset(oammsg,0,MAX_RESPSTRING_SIZE);
            sprintf((char *)oammsg, "Error Saving in EEPROM !\r\nSYSTEM ERROR !\r\n@FAIL@\r\n");
            construct_oam_responsepkt(oammsg, oamresppkt);
#if DEBUG
           debug_msg((char *)oamresppkt->resppkt);
#endif
           memcpy(response_buf, oamresppkt->resppkt, oamresppkt->totallen );
           goto endx;
        }
        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    }
    /////////////////////////////
    if(memid == WIPEMEM_CFG || memid == WIPEMEM_BOTH)
    {
        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        //wipe config
        //////////////////////////////////////////////////////////////////////
        flg = EEPROM_OAMConfig_clear();
        if(flg != 0)
        {
            ret = RET_FAIL;
            memset(oammsg,0,MAX_RESPSTRING_SIZE);
            sprintf((char *)oammsg, "Error Saving in EEPROM !\r\nSYSTEM ERROR !\r\n@FAIL@\r\n");
            construct_oam_responsepkt(oammsg, oamresppkt);
#if DEBUG
           debug_msg((char *)oamresppkt->resppkt);
#endif
           memcpy(response_buf, oamresppkt->resppkt, oamresppkt->totallen );
           goto endx;
        }
        //
        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    }
    //////////////////////////////////////////////////////////////////////
#if DEBUG
    debug_msg("serialdbase:\r\n");
    //DumpHexDebug((const void *)&SERIAL_DBASE, sizeof(SERIAL_DBASE));
    debug_msg("configdbase:\r\n");
    //DumpHexDebug((const void *)&OAM_CONFIG_DBASE, sizeof(OAM_CONFIG_DBASE));
#endif

    //VALID RESPONSE
    memset(oammsg,0,MAX_RESPSTRING_SIZE);
    sprintf((char *)oammsg,"\r\nSUCCESS\r\n@OK@\r\n");
    construct_oam_responsepkt(oammsg, oamresppkt);
    memcpy(response_buf, oamresppkt->resppkt, oamresppkt->totallen );
    #if DEBUG
    debug_msg("Sent Response to OAM...\r\n");
    debug_msg((char *)response_buf);
    ajUART0crlf();
    #endif
    //
    ret = RET_SUCCESS;
    //////////////////////////////////////////////////////////////////////
    ///////////////////////////////
    endx:
    vPortFree(oammsg);
    vPortFree(oamresppkt);
    return ret;
    ///////////////////////////////
}

int handle_logici2cwrrd(int argc, char **argv, uint8_t cmdkwlen, char *response_buf)
{
    //possibilities
    //logici2cwrrd
    //logici2cwrrd help
    //logici2cwrrd ?
    //logici2cwrrd <Count> 1-99
    int c1 = 0;
    int c1x = 0;
    int c2 = 0;
    int selhelp = 0;
    uint16_t u16count;
    uint8_t lenstr1; //len of string argument 1
    //
    //
    int ret = RET_FAIL;
    uint16_t superlen,msglen ;
    char buf[100];
    int j;
    int failstage, testfailstep;
    //
    uint8_t u8count;
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
        }
    }
    if(selhelp != 0)
    {
        construct_oam_large_template_responsepkt((uint8_t *)response_buf, RESPONSE_BUF_SIZE);
         strcat(response_buf, logici2cwrrd.help_str);
         strcat(response_buf, "\r\n");
        //
        memset(buf,0,sizeof(buf));
        sprintf(buf, "Cmd String <%s> has 1 Arg :\r\n", logici2cwrrd.command_name);
        strcat(response_buf, buf);
        //
        //ARG-1, <Group>
        memset(buf,0,sizeof(buf));
        sprintf(buf, "Arg-1 is <Count> %d-%d \r\n", MIN_TESTCOUNT, MAX_TESTCOUNT);
        strcat(response_buf, buf);
        memset(buf,0,sizeof(buf));
        sprintf(buf, "E.G. %s 10\r\n", logici2cwrrd.command_name);
        strcat(response_buf, buf);
         //
         strcat(response_buf, "Device shall carry out LOGIC I2C Registers Write/Read/Compare sequence <Count> times\r\n");
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
    //check arg[1] ChanNo 1-99
    lenstr1 = strlen(argv[1]);
    if(lenstr1 < 1 || lenstr1 > MAX_CMDSTRING_CHARS)
    {
        memset(oammsg,0,MAX_RESPSTRING_SIZE);
        sprintf((char *)oammsg, "Illegal Argument 1 String Length !\r\nSYNTAX ERROR !\r\n@FAIL@\r\n");
#if DEBUG
        debug_msg((char *)oammsg);
#endif
        construct_oam_responsepkt(oammsg, oamresppkt);
        memcpy(response_buf, oamresppkt->resppkt, oamresppkt->totallen );
        goto endx;
    }
    ////////////////////////////////////////////////////////////////
    flg = ConvertStrtoInt16(argv[1], lenstr1, &u16count);
    if(flg != RET_SUCCESS)
    {
        memset(oammsg,0,MAX_RESPSTRING_SIZE);
        sprintf((char *)oammsg,"Illegal Param COUNT !\r\nSYSTEM ERROR !\r\n@FAIL@\r\n");
#if DEBUG
        debug_msg((char *)oammsg);
#endif
        construct_oam_responsepkt(oammsg, oamresppkt);
        memcpy(response_buf, oamresppkt->resppkt, oamresppkt->totallen );
        goto endx;
    }
    ///////////////////////////////////////////////////////////////////////////
    if(u16count < MIN_TESTCOUNT || u16count > MAX_TESTCOUNT)
    {
        memset(oammsg,0,MAX_RESPSTRING_SIZE);
        sprintf((char *)oammsg, "Illegal <ChanNo> %d-%d or 'all' expected !\r\nSYNTAX ERROR !\r\n@FAIL@\r\n", MIN_TESTCOUNT, MAX_TESTCOUNT);
#if DEBUG
        debug_msg((char *)oammsg);
#endif
        construct_oam_responsepkt(oammsg, oamresppkt);
        memcpy(response_buf, oamresppkt->resppkt, oamresppkt->totallen );
        goto endx;
    }
    //////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////
    //Normal Execution
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //BOOTSTEP-6, LOGIC Config
    u8count = u16count;
    ///////////////////////////////////////////////////////////////////
    iwdg_strobpulse();
    //
    uint16_t etimes;
    uint16_t etimem;
    uint16_t rem;
    etimes = u8count * 11;
    if(etimes > 59)
    {
        etimem = etimes / 60;
        rem = etimes % 60;
        debug_msg("\r\nAPPROX TEST TIME: %d mins %d secs !\r\n", etimem, rem);
    }
    else
    {
        debug_msg("\r\nAPPROX TEST TIME: %d secs !\r\n", etimes);
    }

    /////////////////////////////////////////////////////////////////////////////////

    uint8_t fcnt, pcnt;
    fcnt = 0;
    pcnt = 0;
    for(j=0;j<u8count;j++)
    {
        flg = boot_logicconfig(&failstage, &testfailstep);
        if(flg != 0)
        {
            debug_msg("Iteration %d, WRITE / READ (LOGIC Config Regs)..**FAILURE !, Stage %d, FailStep %d\r\n", j + 1, failstage, testfailstep);
            fcnt++;
        }
        else
        {
            debug_msg("Iteration %d, WRITE / READ (LOGIC Config Regs)..PASS !\r\n", j + 1);
            pcnt++;
        }
    }
    ///////////////////////////////////////////////////////
    //
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //VALID RESPONSE
    if(fcnt == 0)
    {

        ret = RET_SUCCESS;
        memset(oammsg,0,MAX_RESPSTRING_SIZE);
        sprintf((char *)oammsg,"\r\nPASS COUNT:%d\r\nFAIL COUNT:%d\r\nSUCCESS\r\n@OK@\r\n",pcnt,fcnt);
    }
    else
    {
        memset(oammsg,0,MAX_RESPSTRING_SIZE);
        sprintf((char *)oammsg,"PASS COUNT:%d\r\nFAIL COUNT:%d\r\nFAILURE !\r\n@FAIL@\r\n",pcnt,fcnt);
    }
    construct_oam_responsepkt(oammsg, oamresppkt);
    memcpy(response_buf, oamresppkt->resppkt, oamresppkt->totallen );
    //
    //////////////////////////////////////////////////////////////////////
    ////////////////
    endx:
    vPortFree(oammsg);
    vPortFree(oamresppkt);
    return ret;
    ////////////////



}




void init_difcommand_list_comfactory_settings(void)
{
	//cli group CLIGROUP_COMFACTORYSET
	//1- 4(4nos)
	add_command_to_list(&setcomserial);
	add_command_to_list(&setcomdesc);
	add_command_to_list(&wipemem);
	add_command_to_list(&logici2cwrrd);
	//
}

