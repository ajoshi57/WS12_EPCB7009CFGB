/****************************************************
  FILE NAME		: f99-struct-bootstatus.h
  ORIGINATOR	: A Joshi
  LAST EDITED   : 18 OCT 2025
  PROJECT       : WS12_EPCB7011
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/



#ifndef INC_F99_STRUCT_BOOTSTATUS_H_
#define INC_F99_STRUCT_BOOTSTATUS_H_

#include <stdbool.h>
#include <stdint.h>
#include "f01-defines.h"
#include "f99-logic-struct.h"
//


//this will progressively grow aswe develop boot steps
#define BOOTSTATUS_STEPS 6

/*************************************************************************
 * Step       Description
 *  1         TEST DATABASE
 *  2         FACTORY CONFIG DATABASE (SERIALDBASE_T)
 *  3         OAMCONFIG
 *  4         LOGIC
 *  5         LOGIC UID
 *  5         LOGIC CONFIG REGS
 *
 *************************************************************************/



typedef struct __attribute__((__packed__))  _bootstatus_
{
	uint8_t boot_done;
	uint8_t overallresult;
	uint8_t result[BOOTSTATUS_STEPS]; //steps 1-BOOTSTATUS_STEPS
	uint16_t fail_step[BOOTSTATUS_STEPS]; //fail step valid in case of FAIL
	//
	//detailed status of serialdbaase and cfgdbase
	uint8_t sdbaseresult;
	uint8_t oamcfgdbaseresult;
	//
	//Logic details
	LOGICPARAMS_T logicparams;
	//
	//
	//record of config options etc
	//

} BOOTSTATUS_T;

#define BOOTRESULT_UNKNOWN 99
#define BOOTRESULT_PASS 0xAA
#define BOOTRESULT_FAIL 1
#define BOOTRESULT_CRITICAL 2
#define BOOTRESULT_SKIP 3
#define BOOTRESULT_ERROR 4
//


#endif /* INC_F99_STRUCT_BOOTSTATUS_H_ */
