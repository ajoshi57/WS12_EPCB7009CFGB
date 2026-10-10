/****************************************************
  FILE NAME     : f72-comfactory-queries.c
   ORIGINATOR   : A Joshi
  LAST EDITED   : 07 OCT 2026
  PROJECT       : WS12_EPCB7009CFGB
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/

#include "f72-comfactory-queries.h"
//
#define DEBUG 1
//

int handle_alarmsfetch(int argc, char **argv, uint8_t cmdkwlen, char *response_buf);
static int construct_alarmsfetch_response(char *response_buf);
void init_difcommand_list_comfactory_queries(void);

CMDMSG_T alarmsfetch =
{
        "#ffalarmsfetch#", //cmdstr
        1,
        "GET pending system ALARMS (used internally by ETH AUX)\r\n", //help_str
        (CLIGROUP_COMFACTORYQUERY + 1), //command_id
        AUTH_VIEWUSER,         //auth_level
        &handle_alarmsfetch      //handler fn
};

int handle_alarmsfetch(int argc, char **argv, uint8_t cmdkwlen, char *response_buf)
{
    //possibilities
    //alarmsfetch
    //alarmsfetch help
    //alarmsfetch ?
    //alarmsfetch exe
    //
    int c1 = 0;
    int c1x = 0;
    int selhelp = 0;
    int ret = RET_FAIL;
    uint16_t superlen,msglen ;
    char buf[20];
    char msgstr[100];
    int j;
    int flg;
    //
    //=====================================
    //char cmdbuf[100];
    //int szcmdbuf = 100;
    //int cmdlen;
    //bool timeoutlif = false;
    //bool isvalidlif;
    //=====================================
    //
    //================================================================================
    uint8_t *oammsg = (uint8_t *) pvPortMalloc(sizeof(uint8_t) * MAX_RESPSTRING_SIZE);
    if (oammsg == NULL)
    {
        debug_msg("\r\nhandle_showslots: FAIL Alloc Mem - 'oammsg'\r\n");
        while (1)
            ;   //stuck here
    }
    ///////////////////////////////////////////////////////////////////////////
    VALIDRESP_T *oamresppkt = (VALIDRESP_T *) pvPortMalloc(sizeof(VALIDRESP_T));
    if(oamresppkt == NULL)
    {
        debug_msg("\r\nhandle_showslots: FAIL Alloc Mem - 'VALIDRESP_T'\r\n");
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
        strncat(response_buf, alarmsfetch.help_str, strlen(alarmsfetch.help_str));
        strcat(response_buf, "\r\n");
        //////////////
        memset(msgstr,0,sizeof(msgstr));
        sprintf(msgstr, "Cmd String <%s> has NO Arguments\r\n",argv[0]);
        strcat(response_buf,(char *)msgstr);
        //////////////
        //////////////
        memset(msgstr,0,sizeof(msgstr));
        sprintf(msgstr, "E.G. %s <CR> \r\n",argv[0]);
        strcat(response_buf,(char *)msgstr);
        //////////////
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
    //NORMAL EXECUTION
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //////////////////////////////////////////////////////////////////////
    //Construct VALID RESPONSE
    flg = construct_alarmsfetch_response(response_buf);
    if(flg != 0)
    {
        ret = RET_FAIL;
    }
    else
    {
        ret = RET_SUCCESS;
    }
    //////////////////////////////////////////////////////////////////////
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    ////////////////
    endx:
    vPortFree(oammsg);
    vPortFree(oamresppkt);
    return ret;
    ////////////////
}

static int construct_alarmsfetch_response(char *response_buf)
{
    int ret = RET_SUCCESS;
    uint16_t superlen,msglen ;
    char buf[100];
    char buf1[100];
    int j;
    //
    //================================================================================
    ALARMENTRY_T *alarmentry = (ALARMENTRY_T *) pvPortMalloc(sizeof(ALARMENTRY_T));
    if(alarmentry == NULL)
    {
        debug_msg("\r\nFAIL Alloc Mem - 'ALARMENTRY_T'\r\n");
        while (1)
            ;   //stuck here
    }
    //================================================================================

    //
    /////////////////////////////////////////////////////////////////////////////////
    //Construct VALID RESPONSE
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    construct_oam_large_template_responsepkt((uint8_t *)response_buf, RESPONSE_BUF_SIZE);
    ///////////////////////////////////////////////////////////////
    //
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //Show Slot Config and also detected Cards
    strcat((char *)response_buf, "\r\n");
    /////////////////////////////////////////////////////////////////////////
    uint8_t avail;
    fetchalarms_available_check_mutex(&avail);
    if(avail == 0)
    {
        strcat((char *)response_buf, "EMPTY");
    }
    else
    {
        if(extract_fetchalarms_mutex(alarmentry) == 0)
        {
            memset(buf,0,sizeof(buf));
            memset(buf1,0,sizeof(buf1));
            sprintf((char *)buf,"%s", get_alarmeventstr(alarmentry, &buf1[0], sizeof(buf1)) );
            strcat((char *)response_buf, buf);
        }
        else
        {
            strcat((char *)response_buf, "EMPTY");
        }
    }
    /////////////////////////////////////////////////////////////////////////
    vPortFree(alarmentry);
    /////////////////////////////////////////////////////////////////////////
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
    /////////////////////
#if DEBUG
    debug_msg("Sent Response to OAM...\r\n");
    debug_msg((char *)response_buf);
#endif
    //
    //////////////////////////////////////////////////////////////////////
    //
    return ret;

}


void init_difcommand_list_comfactory_queries(void)
{
    //cli group CLIGROUP_COMFACTORYQUERIES
    //1- 1(1nos)
    add_command_to_list(&alarmsfetch);
     //
}

