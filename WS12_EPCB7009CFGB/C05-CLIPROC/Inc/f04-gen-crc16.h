/****************************************************
  FILE NAME		: f04-gen-crc16.h
  ORIGINATOR	: A Joshi
  LAST EDITED   : 18 OCT 2025
  PROJECT       : WS12_EPCB7011
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/

#ifndef INC_F04_GEN_CRC16_H_
#define INC_F04_GEN_CRC16_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/////////////////////////////////////////////////////////
//fns defined here...
uint16_t gen_crc16(const uint8_t *data, uint16_t size);
/////////////////////////////////////////////////////////


#endif /* INC_F04_GEN_CRC16_H_ */
