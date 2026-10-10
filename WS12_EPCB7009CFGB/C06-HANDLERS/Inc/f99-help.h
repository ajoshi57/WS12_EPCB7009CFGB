/****************************************************
  FILE NAME		: f99-help.h
  ORIGINATOR	: A Joshi
  LAST EDITED   : 18 OCT 2025
  PROJECT       : WS12_EPCB7011
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/

#ifndef INC_F99_HELP_H_
#define INC_F99_HELP_H_

#include <string.h>
#include "f02-dif-cmdlist.h"
#include "f01-defines.h"
#include "f01-argcheck.h"
#include "f01-console.h"
#include "f01-constructoamresp.h"
#include "f99-struct-configdbase.h"
#include "f04-configdbase.h"
//
#include "f01-core-comsettings.h"
#include "f01-core-comqueries.h"
//
#include "f71-comfactory-settings.h"
//
#define HELP_HELP_FIRST  0
#define HELP_HELP_LAST   0
#define TOTAL_HELP_CMDS  1
//All Core Settings
//Core com = 7
#define TOTAL_COREALLSET_CMDS  7
#define HELP_COREALLSET_FIRST  1
#define HELP_COREALLSET_LAST   7
//
//All Core Queries
//Core com = 4
#define TOTAL_COREALLQ_CMDS  4
#define HELP_COREALLQ_FIRST  HELP_COREALLSET_LAST + 1
#define HELP_COREALLQ_LAST   HELP_COREALLQ_FIRST + TOTAL_COREALLQ_CMDS - 1
//
//
//All FACTORY SETTINGS
//Com Factory Set = 4
#define TOTAL_ALLFACTSET_CMDS 4
#define HELP_ALLFACTSET_FIRST  HELP_COREALLQ_LAST + 1
#define HELP_ALLFACTSET_LAST   HELP_ALLFACTSET_FIRST + TOTAL_ALLFACTSET_CMDS -1
//
//All FACTORY QUERIES
//Com Factory Queries = 1
#define TOTAL_ALLFACTQ_CMDS 1
#define HELP_ALLFACTQ_FIRST  HELP_ALLFACTSET_LAST + 1
#define HELP_ALLFACTQ_LAST   HELP_ALLFACTQ_FIRST + TOTAL_ALLFACTQ_CMDS -1




void init_difcommand_list_core_comsettings(void);
void init_difcommand_list_core_comqueries(void);
void init_difcommand_list_comfactory_settings(void);
void init_difcommand_list_comfactory_queries(void);



#endif /* INC_F99_HELP_H_ */
