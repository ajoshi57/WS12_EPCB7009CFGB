/*
 * f01-tripcntrs.h
 *
 *  Created on: 25-Sep-2026
 *      Author: Arvind Joshi
 */

#ifndef C66_TRIPCNTRS_INC_F01_TRIPCNTRS_H_
#define C66_TRIPCNTRS_INC_F01_TRIPCNTRS_H_


#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "f99-struct-tripcntrs.h"
#include "f01-console.h"
#include "f01-defines.h"
#include "f99-struct-configdbase.h"

//fns protos defined here...
int create_mutex_access_tripcntrs(void);
int init_tripcntrs_mutex(void);
int incr_tripcntrs_mutex(uint8_t inputno);
int copy_tripcntrs_mutex(TRIPCNTRS_T *tripcntrs);
//


#endif /* C66_TRIPCNTRS_INC_F01_TRIPCNTRS_H_ */
