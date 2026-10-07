/****************************************************
  FILE NAME		: f01-i2clogic.h
   ORIGINATOR	: A Joshi
  LAST EDITED   : 18 OCT 2025
  PROJECT       : WS12_EPCB7011
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/


#ifndef INC_F01_I2CLOGIC_H_
#define INC_F01_I2CLOGIC_H_

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "f01-gpio.h"
#include "f99-struct-configdbase.h"
#include "f01-console.h"
#include "f01-gpio.h"
#include "f01-i2cmast.h"

//
//fns defined here...
int create_mutex_access_i2c(void);
void logic_reset_pulse(void);
int logic_write_cycle(uint8_t page, uint8_t addr,uint8_t data);
int logic_read_cycle(uint8_t page, uint8_t addr,uint8_t *data);




#endif /* INC_F01_I2CLOGIC_H_ */
