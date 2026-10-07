/****************************************************
  FILE NAME     : f04-logicdefcfg.h
  ORIGINATOR    : A Joshi
  LAST EDITED   : 16 JUN 2026
  PROJECT       : WS12_EPCB7011
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/

#ifndef C70_LOGIC_INC_F04_LOGICDEFCFG_H_
#define C70_LOGIC_INC_F04_LOGICDEFCFG_H_

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
#include "f99-struct-configdbase.h"
//

//fn protos defined here...
int read_logiccfg(LOGICCFG_T *logiccfg, int *testfailstep, int debugon);
int boot_logicconfig(int *failstage, int *testfailstep);
int program_gf(uint8_t channo, uint8_t gfvalue, int debugon);
int program_pw(uint8_t channo, uint8_t pwvalue, int debugon);
int program_map(uint8_t channo, uint8_t mapvalue, int debugon);



#endif /* C70_LOGIC_INC_F04_LOGICDEFCFG_H_ */
