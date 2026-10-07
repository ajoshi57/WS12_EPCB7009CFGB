/****************************************************
  FILE NAME		: f02-bootprocedure.h
  ORIGINATOR	: A Joshi
  LAST EDITED   : 18 OCT 2025
  PROJECT       : WS12_EPCB7011
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/


#ifndef INC_F02_BOOTPROCEDURE_H_
#define INC_F02_BOOTPROCEDURE_H_

#include "f01-defines.h"
#include "f01-console.h"
#include "f99-struct-bootstatus.h"
#include "f99-struct-configdbase.h"
#include "f04-configdbase.h"
#include "f02-bootlogic.h"
#include "f01-watchdog.h"
#include "f03-logicuid.h"
//

void boot_procedure(BOOTSTATUS_T * status, LOGICPARAMS_T *logicparams);
//


#endif /* INC_F02_BOOTPROCEDURE_H_ */
