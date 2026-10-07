/*
 * f01-twi0mast.h
 *
 *  Created on: 19-May-2026
 *      Author: Arvind Joshi
 */

#ifndef C53_TWIMAST0_INC_F01_TWI0MAST_H_
#define C53_TWIMAST0_INC_F01_TWI0MAST_H_

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
//
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom_map.h"
#include "driverlib/gpio.h"
#include "driverlib/i2c.h"
#include "f01-console.h"
#include "f01-defines.h"
#include "f02-uart7.h"

void TWI0_init(void);
void TWI0_start(void);
void TWI0_rep_start(void);
void TWI0_do_transfer(void);
void TWI0_stop(void);
void TWI0_delay(void);



#endif /* C53_TWIMAST0_INC_F01_TWI0MAST_H_ */
