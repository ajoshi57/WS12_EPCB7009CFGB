/****************************************************
  FILE NAME		: f99-help.c
  ORIGINATOR	: A Joshi
  LAST EDITED   : 12 OCT 2026
  PROJECT       : WS12_EPCB7011
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
//
#define DEBUG 0
#define TESTLOGIC 0
#define TESTFLASH 1
//
#include "f99-help.h"


int handle_difhelp(int argc, char **argv, uint8_t cmdkwlen, char *response_buf);
void init_dif_clicommand_list_help(void);
void init_dif_cli_command_list_all(void);
int get_help_part(int first,int last,int add_hlp_str, char *response_buf);
int add_spaces(char *source,char *dest,int finalsize);
//
void get_help_hlp(int add_hlp_str,char *response_buf);
//
void get_help_coreset(int add_hlp_str,char *response_buf);
void get_help_coreq(int add_hlp_str,char *response_buf);
//
void get_help_fact(int add_hlp_str,char *response_buf);
//
//extern int tryflashwritedata(void);


CMDMSG_T difhelp =
{
		"#help#", //cmdstr
		1,
		"Display MESSAGE PKT Help of Catagory <Cat>\r\n", //help_str
		(CLIGROUP_HELP + 1),          //command_id
		AUTH_VIEWUSER,                   //auth_level
		&handle_difhelp                     //handler fn
};


int handle_difhelp(int argc, char **argv, uint8_t cmdkwlen, char *response_buf)
{
	//possibilities
	//help
	//help help
	//help ?
	//help <cat> which can be one of these [all, core]
	int c1 = 0;
	int c1x = 0;
	int catall,catcoreset,catcoreq;
	int catfactory;
	int selhelp = 0;
	int ret = RET_FAIL;
	int lenstr;
	uint16_t superlen,msglen ;
	char buf[20];
	int j;
	//
	uint8_t addhelpstr = 0;
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
	///////////////////////////////
	//
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
	//////////////////////////////////////////////////////////////////////
    if(selhelp != 0)
    {
    	construct_oam_large_template_responsepkt((uint8_t *)response_buf, RESPONSE_BUF_SIZE);
    	strcat(response_buf, difhelp.help_str);
    	strcat(response_buf, "\r\n");
        //
        strcat(response_buf, "Cmd String <#help#> has 1 Arg :\r\n");
        //ARG-1
        strcat(response_buf, "Arg-1 <cat> is one of the following:\r\n");
        strcat(response_buf, "<all>, <coreset>, <coreq> \r\n");
        strcat(response_buf, "E.G. #help# all\r\n");
        //

#if TESTLOGIC
        //////////////////////////////////////////////////////////////////////////////////////
        uint8_t addr,rdata,exdata,wdata1,wdata2;
		int flg;
       //////////////////////////////////////////////////////////////////////////////////////
    	addr = 0x00;
    	exdata = 0x99;
    	flg = logic_read_cycle(0,addr,&rdata);
    	if(flg != 0)
    	{
    		debug_msg("ERROR, 'logic_read_cycle' !\r\n");
    	}
    	debug_msg("Logic Read Addr %02X, Data %02X, exdata %02X\r\n", addr, rdata, exdata);
     	//////////////////////////////////////////////////////////////////////////////////////

    	addr = 0x01;
    	exdata = 0x22;
    	flg = logic_read_cycle(0,addr,&rdata);
    	if(flg != 0)
    	{
    		debug_msg("ERROR, 'logic_read_cycle' !\r\n");
    	}

    	debug_msg("Logic Read Addr %02X, Data %02X, exdata %02X\r\n", addr, rdata, exdata);
    	//////////////////////////////////////////////////////////////////////////////////////
    	addr = 0x04;
    	exdata = 0x55;
    	flg = logic_read_cycle(0,addr,&rdata);
    	if(flg != 0)
    	{
    		debug_msg("ERROR, 'logic_read_cycle' !\r\n");
    	}

    	debug_msg("Logic Read Addr %02X, Data %02X, exdata %02X\r\n", addr, rdata, exdata);
    	////

    	//////////////////////////////////////////////////////////////////////////////////////
    	addr = 0x05;
    	exdata = 0xAA;
    	flg = logic_read_cycle(0,addr,&rdata);
    	if(flg != 0)
    	{
    		debug_msg("ERROR, 'logic_read_cycle' !\r\n");
    	}
    	debug_msg("Logic Read Addr %02X, Data %02X, exdata %02X\r\n", addr, rdata, exdata);
    	//////////////////////////////////////////////////////////////////////////////////////
    	//READ BACK TEST
        //////////////////////////////////////////////////////////////////////////////////////
    	addr = 0x02;
    	wdata1 = 0xA5;
       	flg = logic_write_cycle(0,addr,wdata1);
    	if(flg != 0)
    	{
    		debug_msg("ERROR, 'logic_write_cycle' !\r\n");
    	}
    	addr = 0x03;
    	wdata2 = 0x5A;
       	flg = logic_write_cycle(0,addr,wdata2);
    	if(flg != 0)
    	{
    		debug_msg("ERROR, 'logic_write_cycle' !\r\n");
    	}
    	//////////////////////////////////////////////////////////////////////////////////////
    	addr = 0x02;
    	exdata = wdata1;
    	flg = logic_read_cycle(0,addr,&rdata);
    	if(flg != 0)
    	{
    		debug_msg("ERROR, 'logic_read_cycle' !\r\n");
    	}
    	debug_msg("Logic Read Addr %02X, Data %02X, exdata %02X\r\n", addr, rdata, exdata);
       //////////////////////////////////////////////////////////////////////////////////////
    	addr = 0x03;
    	exdata = wdata2;
    	flg = logic_read_cycle(0,addr,&rdata);
    	if(flg != 0)
    	{
    		debug_msg("ERROR, 'logic_read_cycle' !\r\n");
    	}
    	debug_msg("Logic Read Addr %02X, Data %02X, exdata %02X\r\n", addr, rdata, exdata);
     	//////////////////////////////////////////////////////////////////////////////////////
#endif
#if TESTFLASH
    	//tryflashwritedata();
#endif;


   	    //
    	//======================================================
    	//======================================================
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

    //////////////////////////////////////////////////////////////////////
    //NORMAL EXECUTION
	//check arg[1]
	lenstr = strlen(argv[1]);
	if(lenstr < MIN_CMDSTRING_CHARS || lenstr > MAX_CMDSTRING_CHARS)
	{
		memset(oammsg,0,MAX_RESPSTRING_SIZE);
		sprintf((char *)oammsg, "Illegal Argument String Length !\r\nSYNTAX ERROR !\r\n@FAIL@\r\n");
		construct_oam_responsepkt(oammsg, oamresp);
#if DEBUG
       debug_msg((char *)oamresp->resppkt);
#endif
       memcpy(response_buf, oamresp->resppkt, oamresp->totallen );
	   goto endx;
	}
	catall = strncmp(argv[1], "all",strlen("all"));
	catcoreset = strncmp(argv[1], "coreset",strlen("coreset"));
	catcoreq   = strncmp(argv[1], "coreq",strlen("coreq"));
	catfactory  = strncmp(argv[1], "factory",strlen("factory"));
	addhelpstr = 1;
	if (catcoreset == 0 && lenstr == 7)
	{
		//ok
	}
	else if (catcoreq == 0 && lenstr == 5)
	{
		//ok
	}
	else if (catfactory == 0 && lenstr == 7)
	{
		//ok
	}
	else if (catall == 0 && lenstr == 3)
	{
		//ok
		addhelpstr = 0;
	}
	else
	{
		memset(oammsg,0,MAX_RESPSTRING_SIZE);
		sprintf((char *)oammsg, "Illegal Argument String, see #help# (#help# ?) !\r\nSYNTAX ERROR !\r\n@FAIL@\r\n");
		construct_oam_responsepkt(oammsg, oamresp);
#if DEBUG
       debug_msg((char *)oamresp->resppkt);
#endif
       memcpy(response_buf, oamresp->resppkt, oamresp->totallen );
	   goto endx;
	}
	//////////////////////////////////////////////////////////////////////
	//we need to respond here only
	construct_oam_large_template_responsepkt((uint8_t *)response_buf, RESPONSE_BUF_SIZE);
	get_help_hlp(1,response_buf);
	if(catcoreset == 0 || catall == 0)
	{
		get_help_coreset(addhelpstr,response_buf);
	}
	if(catcoreq == 0 || catall == 0)
	{
		get_help_coreq(addhelpstr,response_buf);
	}
	if(catfactory == 0)
	{
		get_help_fact(addhelpstr,response_buf);
	}
	//
	strcat(response_buf, "<<<<<< End >>>>>>\r\n");
	//
	//////////////////////////////////////////////////////////////
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
    /////////////////////
#if DEBUG
	debug_msg("Sent Response to OAM...\r\n");
	debug_msg((char *)response_buf);
	debug_port_crlf();
#endif
	//
	ret = RET_SUCCESS;
	//////////////////////////////////////////////////////////////////////
	//
	///////////////////////////////
	endx:
	vPortFree(oammsg);
    vPortFree(oamresp);
    //
	return ret;
	///////////////////////////////
}




void init_dif_clicommand_list_help(void)
{
	//cli group CLIGROUP_HELP
	//1 (01nos)
	add_command_to_list(&difhelp);
}

//total cmds 0 - 73
void init_dif_cli_command_list_all(void)
{
	//1 cmd
	init_dif_clicommand_list_help();
	//
	//6 cmds
	init_difcommand_list_core_comsettings();
	//
	//3 cmds
    init_difcommand_list_core_comqueries();
    //
    //3 cmds
    init_difcommand_list_comfactory_settings();
    //
    //1 cmd
    init_difcommand_list_comfactory_queries();

    //
}


int get_help_part(int first,int last,int add_hlp_str, char *response_buf)
{
	char buf[MAX_SENDBUF_SIZE]={'\0'};
	int i,s1,s2;
	int flg;
	//
	if(first>last)
	{
		return -1;
	}
	if(first>=MAX_NO_COMMANDS || last>=MAX_NO_COMMANDS)
	{
		return -1;
	}
	s1=first;
	s2=last;
	for (i = s1; i < (s2+1) && COMMAND_LIST[i] != NULL; i++)
	{
		if(add_hlp_str)
		{
			flg = add_spaces((char *)COMMAND_LIST[i]->command_name, buf, MAX_CMDKEYWORD_ADDSPACES);
			if(flg!=0)
			{
				return -1;
			}
			strncat(response_buf, buf, strlen(buf));
			strncat(response_buf, COMMAND_LIST[i]->help_str, strlen(COMMAND_LIST[i]->help_str));
			//
		}
		else
		{
			strncat(response_buf, COMMAND_LIST[i]->command_name, strlen(COMMAND_LIST[i]->command_name));
			strcat(response_buf, "\r\n");
		}
	}
	return 0;
	//
}


int add_spaces(char *source,char *dest,int finalsize)
{
	int len,add,j;
	char buf[MAX_SENDBUF_SIZE]={'\0'};
	char sp[MAX_SENDBUF_SIZE]={'\0'};
	//
	if(source==NULL)
	{
		return -1;
	}
	if(dest==NULL)
	{
		return -1;
	}
	len=strlen(source);
	if(len<finalsize)
		add=finalsize-len;
	else
		add=5;
	for(j=0;j<add;j++)
	{
		sp[j]=' ';
	}
	strncpy(buf,source,MAX_SENDBUF_SIZE);
	//
	strncat(buf,sp,add);
	strncpy(dest,buf,MAX_SENDBUF_SIZE);
	return 0;

}

void get_help_hlp(int add_hlp_str,char *response_buf)
{
	char buf[10] = {'\0'};
	sprintf(buf,"[%d] :\r\n", TOTAL_HELP_CMDS );
	strcat(response_buf,"GENERIC HELP CLI Command ");
	strncat(response_buf,buf, strlen(buf));
	//
	get_help_part(HELP_HELP_FIRST,HELP_HELP_LAST,add_hlp_str,response_buf);
	//
}

void get_help_coreset(int add_hlp_str,char *response_buf)
{
	char buf[10] = {'\0'};
	sprintf(buf,"[%d] :\r\n", TOTAL_COREALLSET_CMDS  );
	strcat(response_buf,"CORE CLI Setting Commands ");
	strncat(response_buf,buf, strlen(buf));
	//
	get_help_part(HELP_COREALLSET_FIRST,HELP_COREALLSET_LAST,add_hlp_str,response_buf);
}

void get_help_coreq(int add_hlp_str,char *response_buf)
{
	char buf[10] = {'\0'};
	sprintf(buf,"[%d] :\r\n", TOTAL_COREALLQ_CMDS );
	strcat(response_buf,"CORE CLI Query Commands ");
	strncat(response_buf,buf, strlen(buf));
	//
	get_help_part(HELP_COREALLQ_FIRST,HELP_COREALLQ_LAST,add_hlp_str,response_buf);
	//
}


void get_help_fact(int add_hlp_str,char *response_buf)
{
	char buf[10] = {'\0'};
	sprintf(buf,"[%d] :\r\n", TOTAL_ALLFACTSET_CMDS + TOTAL_ALLFACTQ_CMDS);
	strcat(response_buf,"FACTORY CLI Commands ");
	strncat(response_buf,buf, strlen(buf));
	//
	get_help_part(HELP_ALLFACTSET_FIRST,HELP_ALLFACTQ_LAST,add_hlp_str,response_buf);
	//
}

