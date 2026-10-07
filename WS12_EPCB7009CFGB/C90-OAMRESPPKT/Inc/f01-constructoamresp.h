/****************************************************
  FILE NAME		: f01-constructoamresp.h
  ORIGINATOR	: A Joshi
   LAST EDITED   : 18 OCT 2025
  PROJECT       : WS12_EPCB7011
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/

#ifndef INC_F01_CONSTRUCTOAMRESP_H_
#define INC_F01_CONSTRUCTOAMRESP_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "f01-defines.h"
#include "f99-struct-genericresp.h"
#include "f01-console.h"

int construct_oam_responsepkt(uint8_t *msg,VALIDRESP_T *validresp);
void construct_oam_large_template_responsepkt(uint8_t *largebuf, uint16_t sz);


#endif /* INC_F01_CONSTRUCTOAMRESP_H_ */
