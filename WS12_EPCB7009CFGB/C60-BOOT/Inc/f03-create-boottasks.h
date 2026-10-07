/****************************************************
  FILE NAME     : f03-create-boottask.h
  ORIGINATOR    : A Joshi
  LAST EDITED   : 18 OCT 2025
  PROJECT       : WS12_EPCB7011
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/

#ifndef C60_BOOT_INC_F03_CREATE_BOOTTASKS_H_
#define C60_BOOT_INC_F03_CREATE_BOOTTASKS_H_


#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "f01-boot-task.h"

//fn protos defined here...
BaseType_t ajCreateBootEPCB7011Task( void );
//





#endif /* C60_BOOT_INC_F03_CREATE_BOOTTASKS_H_ */
