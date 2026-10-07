/****************************************************
  FILE NAME		: f01-console.c
  ORIGINATOR	: A Joshi
  LAST EDITED   : 18 OCT 2025
  PROJECT       : WS12_EPCB7011
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/


#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
//

#include "f01-defines.h"
#include "f01-console.h"
//
#define DEBUG 1
//
static const char * const g_pcHex = "0123456789abcdef";

static SemaphoreHandle_t g_mutex_access_u0 = NULL;
///////////////////////////////////////////////////////////////
//
//fns protos defined here...
int ajCreate_mutex_access_u0(void);
void ajUART0printf( const char *pcString, ... );
void print(int dest, const char *pcString, ...);
void ajUART0Write( const char *pcString );
void send_msg(int dest,char *msg,uint32_t len);
void ajClearTerm(int dest);
void ajUART0crlf(void);
////////
//////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

//fns defined here...
int ajCreate_mutex_access_u0(void)
{
	g_mutex_access_u0 = xSemaphoreCreateMutex();
	if( g_mutex_access_u0 == NULL )
	{
		debug_msg("ERROR create, 'g_mutex_access_u0' !\r\n");
		return -1;
	}
	else
	{
		//The semaphore can now be used.
		debug_msg("SUCCESS create, 'g_mutex_access_u0' !\r\n");
	}
    return 0;
}
/*-----------------------------------------------------------*/


/*-----------------------------------------------------------*/

void ajUART0printf( const char *pcString, ... )
{
/* vaArgP is a variable argument list pointer whose content will depend
 * upon the format string passed in pcString. */
va_list vaArgP;

    /* The mutex is created before the scheduler is started, so already exists
     * by the time this task executes.  Attempt to take the mutex, blocking
     * indefinitely to wait for the mutex if it is not available right away. */
    if ( xSemaphoreTake( g_mutex_access_u0, portMAX_DELAY ) == pdTRUE )
    {
        /* Process any passed variable arguments. */
        va_start(vaArgP, pcString);

        /* Call the UART standard IO API to output the UART message by
         * providing both the string and any received variable arguments. */
#if DEBUG
        UARTvprintf(pcString, vaArgP);
#endif

        /* After the UART message is output, the variable arguments are no
         * longer needed. */
        va_end(vaArgP);
    }

    /* The mutex MUST be given back. */
    xSemaphoreGive( g_mutex_access_u0 );
}
/*-----------------------------------------------------------*/

int
ajUARTwrite(int dest, const char *pcBuf, uint32_t ui32Len)
{
#ifdef UART_BUFFERED
    unsigned int uIdx;

    //
    // Check for valid arguments.
    //
    ASSERT(pcBuf != 0);
    ASSERT(g_ui32Base != 0);

    //
    // Send the characters
    //
    for(uIdx = 0; uIdx < ui32Len; uIdx++)
    {
        //
        // If the character to the UART is \n, then add a \r before it so that
        // \n is translated to \n\r in the output.
        //
        if(pcBuf[uIdx] == '\n')
        {
            if(!TX_BUFFER_FULL)
            {
                g_pcUARTTxBuffer[g_ui32UARTTxWriteIndex] = '\r';
                ADVANCE_TX_BUFFER_INDEX(g_ui32UARTTxWriteIndex);
            }
            else
            {
                //
                // Buffer is full - discard remaining characters and return.
                //
                break;
            }
        }

        //
        // Send the character to the UART output.
        //
        if(!TX_BUFFER_FULL)
        {
            g_pcUARTTxBuffer[g_ui32UARTTxWriteIndex] = pcBuf[uIdx];
            ADVANCE_TX_BUFFER_INDEX(g_ui32UARTTxWriteIndex);
        }
        else
        {
            //
            // Buffer is full - discard remaining characters and return.
            //
            break;
        }
    }

    //
    // If we have anything in the buffer, make sure that the UART is set
    // up to transmit it.
    //
    if(!TX_BUFFER_EMPTY)
    {
        UARTPrimeTransmit(g_ui32Base);
        MAP_UARTIntEnable(g_ui32Base, UART_INT_TX);
    }

    //
    // Return the number of characters written.
    //
    return(uIdx);
#else
    unsigned int uIdx;

    //
    // Check for valid UART base address, and valid arguments.
    //
//    ASSERT(g_ui32Base != 0);
    //ASSERT(pcBuf != 0);

    //
    // Send the characters
    //
    for(uIdx = 0; uIdx < ui32Len; uIdx++)
    {
        //
        // If the character to the UART is \n, then add a \r before it so that
        // \n is translated to \n\r in the output.
        //
        /*if(pcBuf[uIdx] == '\n')
        {
            MAP_UARTCharPut(g_ui32Base, '\r');
        }*/

        //
        // Send the character to the UART output.
        //
        //MAP_UARTCharPut(g_ui32Base, pcBuf[uIdx]);
        send_msg(dest, (const char *)&pcBuf[uIdx], 1);
    }

    //
    // Return the number of characters written.
    //
    return(uIdx);
#endif
}


void
ajUARTvprintf(int dest, const char *pcString, va_list vaArgP)
{
    uint32_t ui32Idx, ui32Value, ui32Pos, ui32Count, ui32Base, ui32Neg;
    char *pcStr, pcBuf[16], cFill;

    //
    // Check the arguments.
    //
   // ASSERT(pcString != 0);

    //
    // Loop while there are more characters in the string.
    //
    while(*pcString)
    {
        //
        // Find the first non-% character, or the end of the string.
        //
        for(ui32Idx = 0;
            (pcString[ui32Idx] != '%') && (pcString[ui32Idx] != '\0');
            ui32Idx++)
        {
        }

        //
        // Write this portion of the string.
        //
        ajUARTwrite(dest, pcString, ui32Idx);

        //
        // Skip the portion of the string that was written.
        //
        pcString += ui32Idx;

        //
        // See if the next character is a %.
        //
        if(*pcString == '%')
        {
            //
            // Skip the %.
            //
            pcString++;

            //
            // Set the digit count to zero, and the fill character to space
            // (in other words, to the defaults).
            //
            ui32Count = 0;
            cFill = ' ';

            //
            // It may be necessary to get back here to process more characters.
            // Goto's aren't pretty, but effective.  I feel extremely dirty for
            // using not one but two of the beasts.
            //
again:

            //
            // Determine how to handle the next character.
            //
            switch(*pcString++)
            {
                //
                // Handle the digit characters.
                //
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                {
                    //
                    // If this is a zero, and it is the first digit, then the
                    // fill character is a zero instead of a space.
                    //
                    if((pcString[-1] == '0') && (ui32Count == 0))
                    {
                        cFill = '0';
                    }

                    //
                    // Update the digit count.
                    //
                    ui32Count *= 10;
                    ui32Count += pcString[-1] - '0';

                    //
                    // Get the next character.
                    //
                    goto again;
                }

                //
                // Handle the %c command.
                //
                case 'c':
                {
                    //
                    // Get the value from the varargs.
                    //
                    ui32Value = va_arg(vaArgP, uint32_t);

                    //
                    // Print out the character.
                    //
                    ajUARTwrite(dest, (char *)&ui32Value, 1);

                    //
                    // This command has been handled.
                    //
                    break;
                }

                //
                // Handle the %d and %i commands.
                //
                case 'd':
                case 'i':
                {
                    //
                    // Get the value from the varargs.
                    //
                    ui32Value = va_arg(vaArgP, uint32_t);

                    //
                    // Reset the buffer position.
                    //
                    ui32Pos = 0;

                    //
                    // If the value is negative, make it positive and indicate
                    // that a minus sign is needed.
                    //
                    if((int32_t)ui32Value < 0)
                    {
                        //
                        // Make the value positive.
                        //
                        ui32Value = -(int32_t)ui32Value;

                        //
                        // Indicate that the value is negative.
                        //
                        ui32Neg = 1;
                    }
                    else
                    {
                        //
                        // Indicate that the value is positive so that a minus
                        // sign isn't inserted.
                        //
                        ui32Neg = 0;
                    }

                    //
                    // Set the base to 10.
                    //
                    ui32Base = 10;

                    //
                    // Convert the value to ASCII.
                    //
                    goto convert;
                }

                //
                // Handle the %s command.
                //
                case 's':
                {
                    //
                    // Get the string pointer from the varargs.
                    //
                    pcStr = va_arg(vaArgP, char *);

                    //
                    // Determine the length of the string.
                    //
                    for(ui32Idx = 0; pcStr[ui32Idx] != '\0'; ui32Idx++)
                    {
                    }

                    //
                    // Write the string.
                    //
                    ajUARTwrite(dest, pcStr, ui32Idx);

                    //
                    // Write any required padding spaces
                    //
                    if(ui32Count > ui32Idx)
                    {
                        ui32Count -= ui32Idx;
                        while(ui32Count--)
                        {
                            ajUARTwrite(dest, " ", 1);
                        }
                    }

                    //
                    // This command has been handled.
                    //
                    break;
                }

                //
                // Handle the %u command.
                //
                case 'u':
                {
                    //
                    // Get the value from the varargs.
                    //
                    ui32Value = va_arg(vaArgP, uint32_t);

                    //
                    // Reset the buffer position.
                    //
                    ui32Pos = 0;

                    //
                    // Set the base to 10.
                    //
                    ui32Base = 10;

                    //
                    // Indicate that the value is positive so that a minus sign
                    // isn't inserted.
                    //
                    ui32Neg = 0;

                    //
                    // Convert the value to ASCII.
                    //
                    goto convert;
                }

                //
                // Handle the %x and %X commands.  Note that they are treated
                // identically; in other words, %X will use lower case letters
                // for a-f instead of the upper case letters it should use.  We
                // also alias %p to %x.
                //
                case 'x':
                case 'X':
                case 'p':
                {
                    //
                    // Get the value from the varargs.
                    //
                    ui32Value = va_arg(vaArgP, uint32_t);

                    //
                    // Reset the buffer position.
                    //
                    ui32Pos = 0;

                    //
                    // Set the base to 16.
                    //
                    ui32Base = 16;

                    //
                    // Indicate that the value is positive so that a minus sign
                    // isn't inserted.
                    //
                    ui32Neg = 0;

                    //
                    // Determine the number of digits in the string version of
                    // the value.
                    //
convert:
                    for(ui32Idx = 1;
                        (((ui32Idx * ui32Base) <= ui32Value) &&
                         (((ui32Idx * ui32Base) / ui32Base) == ui32Idx));
                        ui32Idx *= ui32Base, ui32Count--)
                    {
                    }

                    //
                    // If the value is negative, reduce the count of padding
                    // characters needed.
                    //
                    if(ui32Neg)
                    {
                        ui32Count--;
                    }

                    //
                    // If the value is negative and the value is padded with
                    // zeros, then place the minus sign before the padding.
                    //
                    if(ui32Neg && (cFill == '0'))
                    {
                        //
                        // Place the minus sign in the output buffer.
                        //
                        pcBuf[ui32Pos++] = '-';

                        //
                        // The minus sign has been placed, so turn off the
                        // negative flag.
                        //
                        ui32Neg = 0;
                    }

                    //
                    // Provide additional padding at the beginning of the
                    // string conversion if needed.
                    //
                    if((ui32Count > 1) && (ui32Count < 16))
                    {
                        for(ui32Count--; ui32Count; ui32Count--)
                        {
                            pcBuf[ui32Pos++] = cFill;
                        }
                    }

                    //
                    // If the value is negative, then place the minus sign
                    // before the number.
                    //
                    if(ui32Neg)
                    {
                        //
                        // Place the minus sign in the output buffer.
                        //
                        pcBuf[ui32Pos++] = '-';
                    }

                    //
                    // Convert the value into a string.
                    //
                    for(; ui32Idx; ui32Idx /= ui32Base)
                    {
                        pcBuf[ui32Pos++] =
                            g_pcHex[(ui32Value / ui32Idx) % ui32Base];
                    }

                    //
                    // Write the string.
                    //
                    ajUARTwrite(dest, pcBuf, ui32Pos);

                    //
                    // This command has been handled.
                    //
                    break;
                }

                //
                // Handle the %% command.
                //
                case '%':
                {
                    //
                    // Simply write a single %.
                    //
                    ajUARTwrite(dest, pcString - 1, 1);

                    //
                    // This command has been handled.
                    //
                    break;
                }

                //
                // Handle all other commands.
                //
                default:
                {
                    //
                    // Indicate an error.
                    //
                    ajUARTwrite(dest, "ERROR", 5);

                    //
                    // This command has been handled.
                    //
                    break;
                }
            }
        }
    }
}


void print(int dest, const char *pcString, ...)
{
/* vaArgP is a variable argument list pointer whose content will depend
 * upon the format string passed in pcString. */
va_list vaArgP;

    /* The mutex is created before the scheduler is started, so already exists
     * by the time this task executes.  Attempt to take the mutex, blocking
     * indefinitely to wait for the mutex if it is not available right away. */
    if ( xSemaphoreTake( g_mutex_access_u0, portMAX_DELAY ) == pdTRUE )
    {
        /* Process any passed variable arguments. */
        va_start(vaArgP, pcString);

        /* Call the UART standard IO API to output the UART message by
         * providing both the string and any received variable arguments. */
        UARTvprintf(pcString, vaArgP);

        /* After the UART message is output, the variable arguments are no
         * longer needed. */
        va_end(vaArgP);
    }

    /* The mutex MUST be given back. */
    xSemaphoreGive( g_mutex_access_u0 );
}




void ajUART0Write( const char *pcString )
{
uint32_t ui32Idx;

    /* The mutex is created before the scheduler is started, so already exists
     * by the time this task executes.  Attempt to take the mutex, blocking
     * indefinitely to wait for the mutex if it is not available right away. */
    if ( xSemaphoreTake( g_mutex_access_u0, portMAX_DELAY ) == pdTRUE )
    {
        /* Loop through until reaching the end of the string. */
        for(ui32Idx = 0; pcString[ui32Idx] != '\0'; ui32Idx++)
        {
            /* Output the full string. */
            UARTCharPut(UART0_BASE, pcString[ui32Idx]);
        }
    }

    /* The mutex MUST be given back. */
    xSemaphoreGive( g_mutex_access_u0 );
}
/*-----------------------------------------------------------*/
void send_msg(int dest,char *msg,uint32_t len)
{
    //
    if(msg == NULL)
     {
        UART7Send("ERROR ! - 'debug_msg', NULL \r\n",strlen("ERROR ! - 'debug_msg', NULL \r\n"));//debug port
        goto end;
    }
    //
    if(dest == PORT_DEBUG)
    {
        UART7Send(msg,len);
    }
    else if(dest == PORT_USER)
    {
        UART0Send(msg,len);
    }
    else
    {
        UART7Send(msg,len);
        UART7Send("ERROR ! - 'dest' Illegal ! \r\n",strlen("ERROR ! - 'dest' Illegal ! \r\n"));//debug port
   }
   //
   end:
   return;
   //
}

void ajClearTerm(int dest)
{
	ajUART0Write("\33[2J\r");
}

/*-----------------------------------------------------------*/

void ajUART0crlf(void)
{
    ajUART0Write("\r\n");
}

void
ajprint(int dest, const char *pcString, ...)
{
    va_list vaArgP;

    //
    // Start the varargs processing.
    //
    va_start(vaArgP, pcString);

    ajUARTvprintf(dest, pcString, vaArgP);

    //
    // We're finished with the varargs now.
    //
    va_end(vaArgP);
}

/////////////////////////////////////////////////////////////


