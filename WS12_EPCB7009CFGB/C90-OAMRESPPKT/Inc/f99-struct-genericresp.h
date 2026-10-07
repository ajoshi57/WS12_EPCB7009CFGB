/****************************************************
  FILE NAME		: f99-struct-genericresp.h
  ORIGINATOR	: A Joshi
  LAST EDITED   : 18 OCT 2025
  PROJECT       : WS12_EPCB7011
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/


#ifndef INC_F99_STRUCT_GENERICRESP_H_
#define INC_F99_STRUCT_GENERICRESP_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "f01-defines.h"
#include "f01-console.h"



#define RESPIDX_START    0
#define MIN_RESPPKT_SIZE 11  //@@[xxxx]mmmm...mm## where [xxxx] is the msg char count and 'mmmm...mm' is Message
#define MAX_RESPPKT_SIZE 896 //(64 X 14)
#define MIN_RESPSTRING_SIZE 1
#define MAX_RESPSTRING_SIZE 768 //(64 X 12)
//#define MAX_RESPSTRING_SIZE 1280 //(64 X 20)
//#define MAX_RESPPKT_SIZE 1408    //(64 X 22)
//
#define ERRCODE_RESP_SIZETOOSMALL -1
#define ERRCODE_RESP_SIZETOOBIG   -2
#define ERRCODE_RESP_START_TAG    -3
#define ERRCODE_RESP_FORMAT       -4
#define ERRCODE_RESP_VALCHAR      -5
#define ERRCODE_RESP_VALRANGE     -6
#define ERRCODE_RESP_END_TAG      -7
#define ERRCODE_RESP_CHARCOUNT    -8

/**********************************************************************************
 * GENERIC RESPONSE STRUCTURE
 * LIF RESPONSE STRUCTURE IS SAME AS CC RESPONSE STRUCTURE
 * THEREFORE THE LIF RESPONSES CAN BE VALIDATED HERE AND FORWARDED TO OAM.
 * THE SAME METHODOLOGY FOR VALIDATION CAN BE USED IN OAM
 *
 * ITEM NO          DESC                NO OF CHAR             Allowed VALUES
 * 1                RESP START          2                      $$
 * 2                RESP COUNT          6                      [NNNN]  1-MAX_RESPSTRING_SIZE (0001-0384)
 * 3                RESP STRING CHARS   1-768                  ASCII
 *
 * 4                RESP END            2                      ##
 *
 * example
 * $$[0059]{Response String 59 chars long}##
 *
 **********************************************************************************/
typedef struct valid_resp
{
	uint8_t isvalid;
	uint16_t totallen;
	uint16_t respstartidx;
	uint16_t respendidx;
	//
	uint8_t resppkt[MAX_RESPSTRING_SIZE + 1];
	//
}VALIDRESP_T;




#endif /* INC_F99_STRUCT_GENERICRESP_H_ */
