/****************************************************
  FILE NAME		: f04-configdbase.h
  ORIGINATOR	: A Joshi
  LAST EDITED   : 10 MAY 2026
  PROJECT       : WS12_EPCB7011CFGB
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/

#ifndef INC_F04_CONFIGDBASE_H_
#define INC_F04_CONFIGDBASE_H_


#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "f01-defines.h"
#include "f01-console.h"
//
#include "f99-struct-configdbase.h"
//
//
//fns defined...........
//-----------------------------------------------------------------
//
void init_testdbase_defaults(TESTDBASE_T *pCfg);
void init_serialdbase_defaults(SERIALDBASE_T *pCfg);
void init_oamcfgdbase_defaults(OAMCFGDBASE_T *data);
////////////////////////////////////////////////////////////////////////////////////
void get_glitchfilterstr(uint8_t val, char *buf, uint8_t szbuf);
char *get_outputpulsestr(uint8_t val);
////////////////////////////////////////////////////////////////////////////////////
//External Global access FNs
void copy_oamcfgdbase_title(uint8_t *title);
void copy_oamcfgdbase_glitchfilter(uint8_t *glitchfilter);
void copy_oamcfgdbase_outputpulse(uint8_t *outputpulse);
void copy_oamcfgdbase_outputsource(uint8_t *outputsource);
void copy_oamcfgdbase_isvalid(uint8_t *isvalid);
////////////////////////////////////////////////////////////////////////////////////
//External Global access FNs, SERIALDBASE
void copy_serialdbase_cardserial(uint8_t *cardserial);
void copy_serialdbase_carddesc(uint8_t *carddesc);
void copy_serialdbase_validcardserial(uint8_t *validcardserial);
void copy_serialdbase_validcarddesc(uint8_t *validcarddesc);
void copy_serialdbase_isvalid(uint8_t *isvalid);
////////////////////////////////////////////////////////////////////////////////////


#endif /* INC_F04_CONFIGDBASE_H_ */
