/****************************************************
 FILE NAME		: f05-hexdump.h
  ORIGINATOR	: A Joshi
  LAST EDITED   : 18 OCT 2025
  PROJECT       : WS12_EPCB7011
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/

#ifndef INC_F05_HEXDUMP_H_
#define INC_F05_HEXDUMP_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "f01-defines.h"
//#include "f01-watchdog.h"
#include "f01-console.h"

void DumpHex(const void* data, size_t size);
void DumpHexDebug(const void* data, size_t size);
void send_asciicode(uint8_t byt);
void iwdg_strobpulse(void);
//void send_asciicode_ITM(uint8_t byt);

#endif /* INC_F05_HEXDUMP_H_ */
