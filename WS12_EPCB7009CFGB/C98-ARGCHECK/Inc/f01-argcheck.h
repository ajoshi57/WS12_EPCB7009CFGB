/****************************************************
  FILE NAME     : f01-argcheck.h
  ORIGINATOR    : A Joshi
  LAST EDITED   : 18 OCT 2025
  PROJECT       : WS12_EPCB7011
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/

#ifndef INC_F01_ARGCHECK_H_
#define INC_F01_ARGCHECK_H_


#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//CLI ARGUMENT CHARS - CATAGORIES
//
#define CHR_NULL 0
#define CHR_SPACE 1
#define CHR_DIGIT 2
#define CHR_ALPHA 3
#define CHR_SPL   4
//
#define GENERIC_MAXCMDSIZE 100
#define GENERIC_MAXARGSIZE 100
#define GENERIC_MINPORTLIST 1
#define GENERIC_MAXPORTLIST 12

int check_cli_chartype(char c, char *retc);
int ConvertStrtoInt16(char *Str,uint8_t lenstr, uint16_t *Value);
int ConvertStrtoPortorPortList(char *Str,uint8_t lenstr,uint8_t *IsList,uint16_t *Value1,uint16_t *Value2);


#endif /* INC_F01_ARGCHECK_H_ */
