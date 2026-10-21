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


#ifndef INC_F99_LOGIC_STRUCT_H_
#define INC_F99_LOGIC_STRUCT_H_


typedef struct __attribute__((__packed__))  _logicparams_
{
	uint8_t valid;
	uint8_t uid[5]; //UID1-5 in HEX
	uint8_t logicrelease[4]; //Logic Rel date bytes in HEX
	uint8_t logictime[2]; //Logic Rel time bytes in HEX
} LOGICPARAMS_T;

typedef struct __attribute__((__packed__))  _logiccfg_
{
    uint8_t valid;
    uint8_t reggf[4];   //Glitch Filter Regs 1-4 #31-#34
    uint8_t regpw[16];  //Pulse Width Regs 1-16 #11-#26
    uint8_t regmap[16]; //Ouput Source Regs 1-16 #51-66
    uint8_t enabalarm;  //4-bits (LSB) define Reg #41 (bits 3-0 enable alarm due to Inputs 4-1)
} LOGICCFG_T;



#endif /* INC_F99_LOGIC_STRUCT_H_ */
