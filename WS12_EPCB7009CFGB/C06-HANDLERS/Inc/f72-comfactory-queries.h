/****************************************************
  FILE NAME     : f72-comfactory-queries.h
   ORIGINATOR   : A Joshi
  LAST EDITED   : 07 OCT 2026
  PROJECT       : WS12_EPCB7009CFGB
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/

#ifndef C06_HANDLERS_INC_F72_COMFACTORY_QUERIES_H_
#define C06_HANDLERS_INC_F72_COMFACTORY_QUERIES_H_

#include <string.h>
#include "f02-dif-cmdlist.h"
#include "f01-defines.h"
#include "f01-argcheck.h"
#include "f01-console.h"
#include "f01-constructoamresp.h"
#include "f01-constructoamresp.h"
#include "f01-watchdog.h"
#include "f90-handler-utils.h"
#include "f03-fetchalarm-task.h"
#include "f02-uart7.h"

//
int handle_alarmsfetch(int argc, char **argv, uint8_t cmdkwlen, char *response_buf);

void init_difcommand_list_comfactory_queries(void);



#endif /* C06_HANDLERS_INC_F72_COMFACTORY_QUERIES_H_ */
