/****************************************************
  FILE NAME		: f01-u2msgtask.h
  ORIGINATOR	: A Joshi
  LAST EDITED   : 18 OCT 2025
  PROJECT       : WS12_EPCB7011
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/

#ifndef INC_F01_U0MSGTASK_H_
#define INC_F01_U0MSGTASK_H_


#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"


extern QueueHandle_t QueueU0CharsHandle;
void StartTaskU0Msg(void *argument);
BaseType_t post_u0_char(uint8_t c);


#endif /* INC_F01_U0MSGTASK_H_ */
