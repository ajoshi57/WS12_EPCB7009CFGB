/*
 * f99-struct-fetchalarms.h
 *
 *  Created on: 28-Sep-2026
 *      Author: Arvind Joshi
 */

#ifndef C66_TRIPCNTRS_INC_F99_STRUCT_FETCHALARMS_H_
#define C66_TRIPCNTRS_INC_F99_STRUCT_FETCHALARMS_H_

#define MAX_PENDING_ALARMS 64
#include "f01-defines.h"
//

typedef struct __attribute__((__packed__)) _alarmentry_
{
    uint8_t curinputno;  //0 if not input, otherwise 1-4
    uint8_t curoutputno; //0 if not input, otherwise 1-16
    uint32_t inputcnt[MAX_INCHANS];
    uint32_t outputcnt[MAX_OUTCHANS];
    //
} ALARMENTRY_T;



typedef struct __attribute__((__packed__)) _fetchalarms_
{
    ALARMENTRY_T entry[MAX_PENDING_ALARMS];
    uint8_t headidx;
    uint8_t tailidx;
    uint8_t content; //Current Content should be max 'MAX_PENDING_ALARMS'
    //
} FETCHALARMS_T;



#endif /* C66_TRIPCNTRS_INC_F99_STRUCT_FETCHALARMS_H_ */
