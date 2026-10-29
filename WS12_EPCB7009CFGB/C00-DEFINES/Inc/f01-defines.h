/****************************************************
  FILE NAME		: f01-defines.h
  ORIGINATOR	: A Joshi
  LAST EDITED   : 29 OCT 2026
  PROJECT		: WS12_EPCB7009CFGB
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/

#ifndef INC_F01_DEFINES_H_
#define INC_F01_DEFINES_H_


//
#define MAX_RESPONSE_PAGE 2048
#define RESPONSE_BUF_SIZE MAX_RESPONSE_PAGE

/////////////////////////////////////////////////////////
//GENERAL CLI LIMITS
/////////////////////////////////////////////////////////
#define MIN_CMDSTRING_CHARS 2
#define MAX_CMDSTRING_CHARS 20
#define MIN_ARGSTRING_CHARS 2
#define MAX_ARGSTRING_CHARS 20

/////////////////////////////////////////////////////////
//valid msg related
//
//this defines the limits of CLI cmd supported
//This is cmd string including arg strings seperated by spaces
#define MIN_ARGC 1
#define MAX_ARGC 11

//
#define MAX_RESPONSE_PAGE 2048
#define RESPONSE_BUF_SIZE MAX_RESPONSE_PAGE

#define SIZE_RXLARGE_BUF 1024
//#define SIZE_RXLARGE_BUF 1536 //**AJ JUN25 (64 x 24)

#define MAX_MSGPKT_SIZE 384
#define MIN_MSGPKT_SIZE 44
#define MAX_SENDBUF_SIZE 256
#define MAX_MSGPKT_CUTOFF 350
//
#define  MAX_TABLEENTRY_BUF 30
//

//
#define IDX_START 0
//
#define ARGTYPE_NONE 0   /* used as default in CMDMSG_T */
#define ARGTYPE_BOOL 1   /* bool true or false          */
#define ARGTYPE_NUM  2   /* uint16_t in range 0-65535   */
#define ARGTYPE_STRING 3 /* Alpha Num String            */
//
#define MIN_ARGNUM_VALUE 0
#define MAX_ARGNUM_VALUE 65535
//
//
#define CMDSTRING_MINLEN 7
#define CMDSTRING_MAXLEN 25
//
#define RESPCTRL_LEN 6
//
#define HELPCTRL_LEN 6
//
#define NEXTITEMCTRL_MINLEN 6
#define NEXTITEMCTRL_MAXLEN 25
//
#define CRC16MODE_LEN 6
#define CRC16VAL_LEN 9
//
#define ARBOOL_LEN 6
//
#define ARGNUM_MINLEN 6
#define ARGNUM_MAXLEN 10
//
#define ARGSTRING_MINLEN 7
#define ARGSTRING_MAXLEN 25
//
#define STANDARD_TAGCOLON_SIZE 5
/////////////////////////////////////
#define MAX_CMDKEYWORD_ADDSPACES 25
/////////////////////////////////////

//
//CLI
//
#define SYSPROMPT "EPCB7009-CFG-B>"
//
/////////////////////////////////////////////////////////
#define CFG_SYSTYPE  "EPCB7009 Configuration-B"
#define CFG_DESC01 "Configuration supports:"
#define CFG_DESC02 "4  Input Channels with Glitch Filter user configured on a per Channel Basis"
#define CFG_DESC03 "16 Output Channels with user configured (per channel basis) Pulse Width"
#define CFG_DESC04 "16 Output Channels mapped to 4 Inputs in any combination "
#define CFG_DESC05 "Same input maybe mapped to multiple outputs"
//
#define CFG_FWVER "F/W Ver:1.0.0 Rel 29OCT2026 1800Hrs"
/////////////////////////////////////////////////////////

//UARTS AVAIL
#define PORT_DEBUG 3
#define PORT_USER  2
#define PORT_LIF   5
//
#define RET_SUCCESS 0
#define RET_FAIL   -1
//
////////////////////////////////////////////////////////////////////////
//defines used for CONFIGDBASE
//==================================================
#define MAX_TITLE_SIZE 20
#define MAX_SERIAL_SIZE 20
//
//GlitchFilter Values 0 - 8
#define MAX_GFFACTOR 8
//==================================================
////////////////////////////////////////////////////////////////////////
//CLI RELATED DEFINES
#define MIN_INCHANS 1
#define MAX_INCHANS 4
#define MIN_OUTCHANS 1
#define MAX_OUTCHANS 16
//
#define MIN_TESTCOUNT 1
#define MAX_TESTCOUNT 99
//
//OUTPUT PULSE OPTIONS
#define SET_PULSE005MS 0
#define SET_PULSE010MS 1
#define SET_PULSE020MS 2
#define SET_PULSE050MS 3
#define SET_PULSE100MS 4
#define SET_PULSE150MS 5
#define SET_PULSE200MS 6
#define SET_PULSE250MS 7
#define SET_PULSE300MS 8
#define SET_PULSE350MS 9
#define SET_PULSE400MS 10
#define SET_PULSE450MS 11
#define SET_PULSE500MS 12
#define SET_PULSE600MS 13
#define SET_PULSE750MS 14
#define SET_PULSE1SEC  15

//=====================================================================
//OAMSER LOGIC EXPECTED UID
#define OAMSERLOGICUID1 0x0B //CFG-B
#define OAMSERLOGICUID2 0x70 //Cardno Byte-1
#define OAMSERLOGICUID3 0x09 //Cardno Byte-2
#define OAMSERLOGICUID4 0x69 //VCL Unique ID
#define OAMSERLOGICUID5 0x00 //Ver
//=====================================================================
//Valid Flags are combination of #A5 (Test Byte) & 0A (Config Byte)
#define VALIDSERNO 0xA50B
#define VALIDDESC  0xA50B
#define VALIDDBASE 0xA50B
//
#define WIPEMEM_FACT 1
#define WIPEMEM_CFG  2
#define WIPEMEM_BOTH 3
////////////////////////////////////////////////////////////////////////






#endif /* INC_F01_DEFINES_H_ */
