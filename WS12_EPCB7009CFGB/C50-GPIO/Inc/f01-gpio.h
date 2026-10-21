/****************************************************
  FILE NAME		: f01-gpio.h
  ORIGINATOR	: A Joshi
  LAST EDITED   : 18 OCT 2025
  PROJECT       : WS12_EPCB7011
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/

#ifndef INC_F01_GPIO_H_
#define INC_F01_GPIO_H_

#include <stdint.h>
#include <stdbool.h>
//
#include "f01-defines.h"
#include "f01-console.h"


//Fns protos defined here...
void drive_nresetlogic(uint8_t lev);
void drive_nresetflash(uint8_t lev);
void drive_ncsflash(uint8_t lev);
//
void drive_i2cdevno(uint8_t devno);
void drive_ledselftest(uint8_t lev);
//
void drive_noamcomen(uint8_t lev); //low to enable handsahake signals on CLI RJ45
void drive_cardalarm(uint8_t lev);
void drive_cardready(uint8_t lev);
//
void drive_ncslogic(uint8_t lev); //not used for I2C
//
void setup_gpio_defaults(void);
void logic_reset_pulse(void);
//
void get_alarmrelay_status(uint8_t *status);



#endif /* INC_F01_GPIO_H_ */
