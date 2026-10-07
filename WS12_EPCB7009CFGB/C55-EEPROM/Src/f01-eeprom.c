/****************************************************
  FILE NAME     : f01-eeprom.c
  ORIGINATOR    : A Joshi
  LAST EDITED   : 21 APR 2026
  PROJECT       : WS12_EPCB7011
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/

#include "f01-eeprom.h"

/////////////////////////////////////////////////////////////////////
//FNs protos defined here...
void init_eeprom(void);



/////////////////////////////////////////////////////////////////////
//FNs defined here...
void init_eeprom(void)
{
    uint32_t returnCode;
    uint32_t e2size,e2block;
    /*******************************/
     /* EEPROM SETTINGS */
     SysCtlPeripheralEnable(SYSCTL_PERIPH_EEPROM0); // EEPROM activate
     do
     {
         returnCode=EEPROMInit(); // EEPROM start
     }
     while(returnCode!=EEPROM_INIT_OK);
     //
     e2size = EEPROMSizeGet(); // Get EEPROM Size
     //debug_msg();
     debug_msg("EEPROM Size %d bytes\r\n", e2size);
     //
     e2block = EEPROMBlockCountGet(); // Get EEPROM Block Count
     debug_msg("EEPROM Block Count: %d\r\n", e2block);
     /*******************************/
}

