/****************************************************
  FILE NAME     : f99-struct-epcfgdbase.h
  ORIGINATOR    : A Joshi
  LAST EDITED   : 21 APR 2026
  PROJECT       : WS12_EPCB7011
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/

#ifndef C55_EEPROM_INC_F99_STRUCT_EPCFGDBASE_H_
#define C55_EEPROM_INC_F99_STRUCT_EPCFGDBASE_H_

#include <stdbool.h>
#include <stdint.h>
#include "f01-defines.h"
//
//==================================================
#define MAX_EEPROM_BYTES 0x1800
//
#define MAX_EEPROM_WORDS 0x600 //1536 words of 32 bit (4 Bytes)
//
#define MAX_START_ADDR   0x17FC
//
#define WORD_BYTES 4




#endif /* C55_EEPROM_INC_F99_STRUCT_EPCFGDBASE_H_ */
