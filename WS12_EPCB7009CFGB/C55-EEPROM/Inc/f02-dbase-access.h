/*
 * f02-dbase-access.h
 *
 *  Created on: 21-Apr-2026
 *      Author: Arvind Joshi
 */

#ifndef C55_EEPROM_INC_F02_DBASE_ACCESS_H_
#define C55_EEPROM_INC_F02_DBASE_ACCESS_H_

#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "f01-defines.h"
#include "f01-console.h"
//
#include "f01-eeprom.h"
//
//
#include "f99-struct-configdbase.h"
#include "f04-configdbase.h"

/////////////////////////////////////////////////////////////////////
//FNs protos defined here...
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//Config Access
int EEPROM_OAMConfig_clear(void);
int EEPROM_OAMConfig_default(void);
int EEPROM_OAMConfig_title(uint8_t *title, uint8_t len);
int EEPROM_OAMConfig_glitchfilter_factor(uint8_t ch, uint8_t factor);
int EEPROM_OAMConfig_outputpulse(uint8_t ch, uint8_t outputpulse);
int EEPROM_OAMConfig_outputsource(uint8_t ch, uint8_t outputsource);
int EEPROM_WriteOAMConfig(void);
int EEPROM_ReadOAMConfig(void);
//=====================================================================
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//FACTORY Access
int EEPROM_Serial_clear(void);
int EEPROM_Serial_default(void);
int EEPROM_Serial_serno(uint8_t *serno, uint8_t len);
int EEPROM_Serial_desc(uint8_t *desc, uint8_t len);
int EEPROM_WriteSerial(void);
int EEPROM_ReadSerial(void);
//=====================================================================
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int boot_testdbase_eeprom(int *result, uint8_t *failstep);
int boot_serialdbase_eeprom(int *result, uint8_t *failstep);
int boot_oamcfgdbase_eeprom(int *result, uint8_t *failstep);


/////////////////////////////////////////////////////////////////////



#endif /* C55_EEPROM_INC_F02_DBASE_ACCESS_H_ */
