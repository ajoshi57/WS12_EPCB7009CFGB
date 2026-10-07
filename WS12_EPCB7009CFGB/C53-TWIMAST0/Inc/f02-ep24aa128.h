/*
 * f02-ep24aa128.h
 *
 *  Created on: 19-May-2026
 *      Author: Arvind Joshi
 */

#ifndef C53_TWIMAST0_INC_F02_EP24AA128_H_
#define C53_TWIMAST0_INC_F02_EP24AA128_H_

#include "f01-twi0mast.h"

extern uint8_t g_TWI0adr;
extern uint8_t g_TWI0data;
extern uint8_t g_TWI0Ackflag;

//Protos of fns defined here....
void init_ep24aa128(void);
int ep24aa128_writecycle(uint8_t dev,uint8_t page,uint8_t addr,uint8_t data);
int ep24aa128_readcycle(uint8_t dev, uint8_t page, uint8_t addr, uint8_t *data);
//


#endif /* C53_TWIMAST0_INC_F02_EP24AA128_H_ */
