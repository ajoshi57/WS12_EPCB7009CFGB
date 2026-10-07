/****************************************************
  FILE NAME     : f02-twi1logic.c
  ORIGINATOR    : A Joshi
  LAST EDITED   : 13 JUN 2026
  PROJECT       : WS12_EPCB7011
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/

#include "f02-twi1logic.h"
//
#define DEBUG 0
//direction bit (forms part of device addr)
#define TWIrd   1
#define TWIwr   0

//=================================================
//**AJ
//static SemaphoreHandle_t g_mutex_access_i2c;
//=================================================

//Protos of fns defined here....
void init_twi1logic(void);
int twi1logic_writecycle(uint8_t dev,uint8_t addr,uint8_t data);
int twi1logic_readcycle(uint8_t dev, uint8_t addr, uint8_t *data);
//

//Fns defined here....
void init_twi1logic(void)
{
    TWI1_init();
}

//
int twi1logic_writecycle(uint8_t dev,uint8_t addr,uint8_t data)
{
#if DEBUG
    debug_msg("WR TWI1 %02X\r\n", data);
#endif
    //start
    g_TWI1adr= 0x50+ (dev*2)+TWIwr;
    TWI1_start();
    //first word addr
    g_TWI1data=addr;
    TWI1_do_transfer();
    //data byte
    g_TWI1data=data;
    TWI1_do_transfer();
    TWI1_stop();
    TWI1_delay();
    return 0;
}

int twi1logic_readcycle(uint8_t dev, uint8_t addr, uint8_t *data)
{
    //start
    g_TWI1adr = 0x50+(dev*2)+TWIwr;
    TWI1_start();
    //first word addr
    g_TWI1data = addr;
    TWI1_do_transfer();
    //rep start
    g_TWI1adr = 0x50+(dev*2)+TWIrd;
    TWI1_rep_start();
    g_TWI1Ackflag = 1;
    //data byte
    TWI1_do_transfer();
    TWI1_stop();
    *data= g_TWI1data;
#if DEBUG
    debug_msg("RD TW1 %02X\r\n", g_TWI1data);
#endif
    //
    return 0;
    //
}




