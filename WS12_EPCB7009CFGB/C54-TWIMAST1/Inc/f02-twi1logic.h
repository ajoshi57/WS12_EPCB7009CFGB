/****************************************************
  FILE NAME     : f02-twi1logic.h
  ORIGINATOR    : A Joshi
  LAST EDITED   : 09 JUN 2026
  PROJECT       : WS12_EPCB7011
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/

#ifndef C54_TWIMAST1_INC_F02_TWI1LOGIC_H_
#define C54_TWIMAST1_INC_F02_TWI1LOGIC_H_

#include "f01-twi1mast.h"

extern uint8_t g_TWI1adr;
extern uint8_t g_TWI1data;
extern uint8_t g_TWI1Ackflag;

//Protos of fns defined here....
void init_twi1logic(void);
int twi1logic_writecycle(uint8_t dev,uint8_t addr,uint8_t data);
int twi1logic_readcycle(uint8_t dev, uint8_t addr, uint8_t *data);




#endif /* C54_TWIMAST1_INC_F02_TWI1LOGIC_H_ */
