/****************************************************
  FILE NAME     : f02-ep24aa128.c
  ORIGINATOR    : A Joshi
  LAST EDITED   : 19 MAY 2026
  PROJECT       : WS12_EPCB7011
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/

#include "f02-ep24aa128.h"
//
#define DEBUG 0
//direction bit (forms part of device addr)
#define TWIrd   1
#define TWIwr   0


//Protos of fns defined here....
void init_ep24aa128(void);
int ep24aa128_writecycle(uint8_t dev,uint8_t page,uint8_t addr,uint8_t data);
int ep24aa128_readcycle(uint8_t dev, uint8_t page, uint8_t addr, uint8_t *data);
//

//Fns defined here....
void init_ep24aa128(void)
{
    TWI0_init();
}

//
int ep24aa128_writecycle(uint8_t dev,uint8_t page,uint8_t addr,uint8_t data)
{
#if DEBUG
    debug_msg("WR %02X\r\n", data);
#endif
    //start
    g_TWI0adr= 0xA0+ (dev*2)+TWIwr;
    TWI0_start();
    //first word addr
    g_TWI0data=page;
    TWI0_do_transfer();
    //second word addr
    g_TWI0data=addr;
    TWI0_do_transfer();
    //data byte
    g_TWI0data=data;
    TWI0_do_transfer();
    TWI0_stop();
    TWI0_delay();
    return 0;
}

int ep24aa128_readcycle(uint8_t dev, uint8_t page, uint8_t addr, uint8_t *data)
{
    //start
    g_TWI0adr = 0xA0+(dev*2)+TWIwr;
    TWI0_start();
    //first word addr
    g_TWI0data = page;
    TWI0_do_transfer();
    //second word addr
    g_TWI0data = addr;
    TWI0_do_transfer();
    //rep start
    g_TWI0adr = 0xA0+(dev*2)+TWIrd;
    TWI0_rep_start();
    g_TWI0Ackflag = 1;
    //data byte
    TWI0_do_transfer();
    TWI0_stop();
    *data= g_TWI0data;
#if DEBUG
    debug_msg("RD %02X\r\n", g_TWI0data);
#endif
    //
    return 0;
    //
}



