/****************************************************
  FILE NAME		: f01-create-clitask.c
  ORIGINATOR	: A Joshi
  LAST EDITED   : 18 OCT 2025
  PROJECT       : WS12_EPCB7011
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/


#include "f01-create-clitasks.h"
//
QueueHandle_t QueueU0CharsHandle;

//
//fn protos defined here...
BaseType_t ajCreateAutoCompleteCLITaskUART0( void );
int ajCreateUART0Queue(void);

//
//fns defined here...
BaseType_t ajCreateAutoCompleteCLITaskUART0( void )
{
    BaseType_t ret;
     //
    //////////////////////////////////////////////////////////////////////////////////////////
   /* creation of TaskU0Msg */
   ret = xTaskCreate(      StartTaskU0Msg,
                     "TaskU0Msg",
                     1024,
                     "TM4C1294NCPDT UART0 msg task ",
                     tskIDLE_PRIORITY + 2,
                     NULL );
    return ret;
}

int ajCreateUART0Queue(void)
{
    int ret = 0;
    //////////////////////////////////////////////////////////////////////////////////////////
    /* creation of Queue0Chars */
    //QueueU2CharsHandle = osMessageQueueNew (64, sizeof(uint8_t), &QueueU2Chars_attributes);
    // Create a queue that holds one uint32_t item
    QueueU0CharsHandle = xQueueCreate(64, sizeof(uint8_t));
    //////////////////////////////////////////////////////////////////////////////////////////
    if(QueueU0CharsHandle == NULL)
    {
        ret = -1;
    }
    return ret;
    //
}

