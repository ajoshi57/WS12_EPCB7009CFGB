/****************************************************
  FILE NAME		: f02-bootlogic.h
  ORIGINATOR	: A Joshi
  LAST EDITED   : 13 JUN 2026
  PROJECT       : WS12_EPCB7011
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/


#ifndef INC_F02_BOOTLOGIC_H_
#define INC_F02_BOOTLOGIC_H_


#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "f01-defines.h"
//#include "f01-i2clogic.h"
#include "f02-twi1logic.h"
#include "f02-uart7.h"
#include "f99-logic-struct.h"
//

int boot_logic(LOGICPARAMS_T *logicparams, int *testfailstep);
void construct_logicparams(LOGICPARAMS_T *logicparams, char *uidstr, char *reldatestr);



#endif /* INC_F02_BOOTLOGIC_H_ */
