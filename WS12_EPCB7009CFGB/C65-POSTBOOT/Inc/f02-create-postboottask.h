/*
 * f02-create-postboottask.h
 *
 *  Created on: 18-Oct-2025
 *      Author: Arvind Joshi
 */

#ifndef C65_POSTBOOT_INC_F02_CREATE_POSTBOOTTASK_H_
#define C65_POSTBOOT_INC_F02_CREATE_POSTBOOTTASK_H_


#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "f01-postboot-task.h"

//
//fn protos defined here...
BaseType_t ajCreatePostBootEPCB7011Task( void );
//



#endif /* C65_POSTBOOT_INC_F02_CREATE_POSTBOOTTASK_H_ */
