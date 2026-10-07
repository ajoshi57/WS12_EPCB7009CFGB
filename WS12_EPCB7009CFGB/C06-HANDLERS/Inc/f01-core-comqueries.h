/****************************************************
  FILE NAME		: f01-core-comqueries.h
  ORIGINATOR	: A Joshi
  LAST EDITED   : 18 OCT 2025
  PROJECT       : WS12_EPCB7011
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/

#ifndef INC_F01_CORE_COMQUERIES_H_
#define INC_F01_CORE_COMQUERIES_H_

#include <string.h>
#include "f02-dif-cmdlist.h"
#include "f01-defines.h"
#include "f01-console.h"
#include "f01-boot-task.h"
#include "f01-constructoamresp.h"
#include "f90-handler-utils.h"
//
#include "f99-struct-configdbase.h"
#include "f04-configdbase.h"
#include "f02-dbase-access.h"

void init_difcommand_list_core_comqueries(void);


#endif /* INC_F01_CORE_COMQUERIES_H_ */
