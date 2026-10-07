/*
 * f99-struct-tripcntrs.h
 *
 *  Created on: 25-Sep-2026
 *      Author: Arvind Joshi
 */

#ifndef C66_TRIPCNTRS_INC_F99_STRUCT_TRIPCNTRS_H_
#define C66_TRIPCNTRS_INC_F99_STRUCT_TRIPCNTRS_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "f01-defines.h"

typedef struct __attribute__((__packed__)) _tripcntrs_
{
    uint32_t inputcnt[MAX_INCHANS];
    uint32_t outputcnt[MAX_OUTCHANS];
    //
} TRIPCNTRS_T;



#endif /* C66_TRIPCNTRS_INC_F99_STRUCT_TRIPCNTRS_H_ */
