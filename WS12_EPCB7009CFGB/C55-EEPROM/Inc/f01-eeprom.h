/*
 * f01-eeprom.h
 *
 *  Created on: 21-Apr-2026
 *      Author: Arvind Joshi
 */

#ifndef C55_EEPROM_INC_F01_EEPROM_H_
#define C55_EEPROM_INC_F01_EEPROM_H_

//*****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "drivers/pinout.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "driverlib/eeprom.h"
//
#include "f01-console.h"
#include "f99-struct-epcfgdbase.h"


//*****************************************************************************
/////////////////////////////////////////////////////////////////////
//FNs protos defined here...
void init_eeprom(void);
int write_anydbase_eeprom(uint32_t *anydbase, uint32_t baseword, uint32_t total_dbase_bytes);
int read_anydbase_eeprom(uint32_t *anydbase, uint32_t baseword, uint32_t total_dbase_bytes);




#endif /* C55_EEPROM_INC_F01_EEPROM_H_ */
