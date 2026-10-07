/*
 * f03-uart0.c
 *
 *  Created on: 04-Jun-2026
 *      Author: Arvind Joshi
 */


#include "f03-uart0.h"

//Functions Defined for UART0 Operation
//*******************************************
void UART0Send(char *pui8Buffer, uint32_t ui32Count);
//*******************************************

//*****************************************************************************
//
// Send a string to the UART.
//
//*****************************************************************************
void UART0Send(char *pui8Buffer, uint32_t ui32Count)
{
    bool flag,result;
    //
    // Loop while there are more characters to send.
    //
    while(ui32Count--)
    {
        //
        // Write the next character to the UART.
        //
        flag=true;
        //while(flag == true)
        //{
          if(*pui8Buffer == '\n')
          {
              while(ROM_UARTCharPutNonBlocking(UART0_BASE, '\r') != true);
          }

          while(ROM_UARTCharPutNonBlocking(UART0_BASE, *pui8Buffer) != true);
          /*if(result==false)
              flag=true;
          else
              flag=false;
         }*/
        *pui8Buffer++;

    }
}





