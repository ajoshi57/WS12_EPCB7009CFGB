/****************************************************
  FILE NAME		: f01-create-clitask.h
  ORIGINATOR	: A Joshi
  LAST EDITED   : 18 OCT 2025
  PROJECT       : WS12_EPCB7011
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/

#ifndef INC_F01_CREATE_CLITASKS_H_
#define INC_F01_CREATE_CLITASKS_H_

#include "f01-u0msgtask.h"

BaseType_t ajCreateAutoCompleteCLITaskUART0( void );
int ajCreateUART0Queue(void);

#endif /* INC_F01_CREATE_CLITASKS_H_ */
