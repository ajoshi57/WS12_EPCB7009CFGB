/****************************************************
  FILE NAME		: f02-u0inthandler.c
  ORIGINATOR	: A Joshi
  LAST EDITED   : 18 OCT 2025
  PROJECT       : WS12_EPCB7011
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/

#include "f02-u0inthandler.h"
#include "f01-console.h"
#include "f01-defines.h"

extern uint32_t g_ui32SysClock;

void Post_Uart0_char(int c);
void UART0IntHandler(void);

void UART0IntHandler(void)
{
    uint32_t ui32Status;
	BaseType_t ret;

    int32_t i32RxChr;
    int c;
    //
    // Get the interrrupt status.
     //
    ui32Status = MAP_UARTIntStatus(UART0_BASE, true);
    //
    // Clear the asserted interrupts.
    //
    MAP_UARTIntClear(UART0_BASE, ui32Status);
    //
    // Loop while there are characters in the receive FIFO.
    //
    while(MAP_UARTCharsAvail(UART0_BASE))
    {
        //
        // Read the next character from the UART and write it back to the UART.
        //
    	i32RxChr=MAP_UARTCharGetNonBlocking(UART0_BASE);
        //
        //MAP_UARTCharPutNonBlocking(UART0_BASE,i32RxChr);
        //
        // Delay for 1 millisecond.  Each SysCtlDelay is about 3 clocks.
        //
       // SysCtlDelay(g_ui32SysClock / (1000 * 3));
        //
    	if(i32RxChr!=-1)
        {
            c = i32RxChr;
            //debug_msg("%c ");
        	//Valid Char received
            ret = xQueueSendToBackFromISR(QueueU0CharsHandle, &c, 0);
    		//post_u0_char(c);
        }
    }
}

void Post_Uart0_char(int c)
{
    //adds received into queue
    xQueueSendToBackFromISR(QueueU0CharsHandle, &c, 0);
}

