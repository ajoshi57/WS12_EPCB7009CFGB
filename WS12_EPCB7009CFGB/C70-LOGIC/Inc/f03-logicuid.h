/****************************************************
  FILE NAME		: f03-logicuid.h
  ORIGINATOR	: A Joshi
  LAST EDITED   : 18 OCT 2025
  PROJECT       : WS12_EPCB7011
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/

#ifndef INC_F03_LOGICUID_H_
#define INC_F03_LOGICUID_H_


#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "f01-defines.h"

#include "f01-i2clogic.h"
#include "f99-logic-struct.h"


int boot_logicuid(LOGICPARAMS_T *logicparams, int *testfailstep);




#endif /* INC_F03_LOGICUID_H_ */
