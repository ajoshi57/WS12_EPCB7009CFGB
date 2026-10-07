/****************************************************
  FILE NAME		: f99-struct-configdbase.h
  ORIGINATOR	: A Joshi
  LAST EDITED   : 05 MAY 2026
  PROJECT       : WS12_EPCB7011
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/

#ifndef INC_F99_STRUCT_CONFIGDBASE_H_
#define INC_F99_STRUCT_CONFIGDBASE_H_

#include <stdbool.h>
#include <stdint.h>
#include "f01-defines.h"
//
//
//==================================================
//EEPROM
//Test words starting at 0
#define RESERVED_TEST_BASE_WORD 0
//==================================================
#define EPSERIAL_DBASE_BASE_WORD_MAIN 4
#define EPSERIAL_DBASE_BASE_WORD_SIZE 11   //44 Bytes
#define EPSERIAL_DBASE_BASE_WORD_BKP  15
//
#define EPOAMCFG_DBASE_BASE_WORD_MAIN 26
#define EPOAMCFG_DBASE_BASE_WORD_SIZE 16   //64 Bytes
#define EPOAMCFG_DBASE_BASE_WORD_BKP  42
//



//DBASE READ ERROR FLAGS
#define BOOT_DBASE_RESULT_MAIN_FAIL_BKP_FAIL   3
#define BOOT_DBASE_RESULT_MAIN_FAIL_BKP_OK     2
#define BOOT_DBASE_RESULT_MAIN_OK_BKP_UPDATED  1
//
//==================================================
//EP TEST Word Count 4
typedef struct __attribute__((__packed__))  _testdbase_
{
    uint8_t bytes[16];
} TESTDBASE_T;
//==================================================
//
typedef struct __attribute__((__packed__))  _serialdbase_
{
    //No of words = 15
    uint8_t cardserial[24]; //6,24
    uint32_t validcardserial; //1,4
    //
    uint8_t carddesc[24];   //6,24
    uint32_t validcarddesc; //1,4
    //
    //
    uint32_t isvalid; //1,4
    //
} SERIALDBASE_T;


typedef struct __attribute__((__packed__))  _oamcfgdbase_
{
    //No of words = 16
    uint8_t title[24];  //6,24
    uint8_t glitchfilter[MAX_INCHANS];  //1,4
    uint8_t outputpulse[MAX_OUTCHANS];  //4,16
    uint8_t outputsource[MAX_OUTCHANS]; //4,16
    //
    uint32_t isvalid; //1,4
    //
} OAMCFGDBASE_T;
//





#endif /* INC_F99_STRUCT_CONFIGDBASE_H_ */
