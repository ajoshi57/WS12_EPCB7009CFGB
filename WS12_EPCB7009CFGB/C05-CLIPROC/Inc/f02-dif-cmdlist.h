/****************************************************
  FILE NAME		: f02-dif-cmdlist.h
  ORIGINATOR	: A Joshi
  LAST EDITED   : 18 OCT 2025
  PROJECT       : WS12_EPCB7011
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/


#ifndef INC_F02_DIF_CMDLIST_H_
#define INC_F02_DIF_CMDLIST_H_

//
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
//
#include "f99-struct-dif-cmdmsg.h"

#define AUTH_SUPERUSER 1
#define AUTH_ADMINUSER 2
#define AUTH_VIEWUSER  3
//
#define MAX_NO_COMMANDS 150
//
#define CLIGROUP_CORECOMSET 50
#define CLIGROUP_CORECOMQUERY 70
//
#define CLIGROUP_CORECCSET 100
#define CLIGROUP_CORECCQUERY 150
//
#define CLIGROUP_CORELIFSET 160
#define CLIGROUP_CORELIFQUERY 170
//
#define CLIGROUP_T1IFSET   200
#define CLIGROUP_T1IFQUERY 250
//
#define CLIGROUP_T1IFMAPSET 300
#define CLIGROUP_T1IFMAPQ   350
//
#define CLIGROUP_T1IFPMONSET 400
#define CLIGROUP_T1IFPMONQUERY 450
//
#define CLIGROUP_COMFACTORYSET   500
#define CLIGROUP_COMFACTORYQUERY 550
//
#define CLIGROUP_CCFACTORYSET   800
#define CLIGROUP_CCFACTORYQUERY 850
//
#define CLIGROUP_T1FACTORYSET 870
//
#define CLIGROUP_CCDEBUGQUERY 900
//
#define CLIGROUP_HELP    999
//

//externs defined in cmdlist.c
extern CMDMSG_T *COMMAND_LIST[MAX_NO_COMMANDS + 1];
extern int GEN_CMD_END_INDEX;

extern int add_command_to_list(CMDMSG_T *cmd);
extern bool find_command(CMDMSG_T **command_list, char *cmd_name, uint8_t *cmdkwlen, uint16_t *cmdno);


#endif /* INC_F02_DIF_CMDLIST_H_ */
