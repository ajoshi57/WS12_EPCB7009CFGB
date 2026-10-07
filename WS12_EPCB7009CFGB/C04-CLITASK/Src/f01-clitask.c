/****************************************************
  FILE NAME		: f01-clitask.c
 ORIGINATOR	    : A Joshi
  LAST EDITED   : 12 OCT 2025
  PROJECT		: TM4C_CLIRTOS
  Target 	  	: TM4C129DNCPDT
  Crystal	    : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/


#include "f01-clitask.h"

/*
 * Called by main() to create the UART output tasks.
 */
void ajCreateCLITask( void );
static void ajTestUART0OutputTask( void *pvParameters );


/*-----------------------------------------------------------*/

void ajCreateCLITask( void )
{
    int flg;
    //
    /* Configure UART0 to send messages to terminal. */
    //PinoutSet_UART0Console();
    //
    /* Before a semaphore is used it must be explicitly created. In this example a
     * mutex type semaphore is created. */
 
    if( flg == 0 )
    {
        /* Create the tasks as described in the comments at the top of this
         * file. */
        xTaskCreate( ajTestUART0OutputTask,
                     "TestUART0",
                     configMINIMAL_STACK_SIZE,
                     "AJ1-Task 1 Output - TM4C1294NCPDT UART thread safe "
                     "demonstration\r\n",
                     tskIDLE_PRIORITY + 2,
                     NULL );
        

        /* The xTaskCreate parameters in order are:
         *  - The function that implements the task.
         *  - The text name for the output task - for debug only as it is not
         *    used by the kernel.
         *  - The size of the stack to allocate to the task.
         *  - The parameter passed to the task - just to check the functionality.
         *  - The priority assigned to the task.
         *  - The task handle is not required, so NULL is passed.
        xTaskCreate( prvDataOutputTask1,
                     "Output1",
                     configMINIMAL_STACK_SIZE,
                     "AJ1-Task 1 Output - TM4C1294NCPDT UART thread safe "
                     "demonstration\r\n",
                     tskIDLE_PRIORITY + 2,
                     NULL ); */

        /* The xTaskCreate parameters in order are:
         *  - The function that implements the task.
         *  - The text name for the output task - for debug only as it is not
         *    used by the kernel.
         *  - The size of the stack to allocate to the task.
         *  - The parameter passed to the task - just to check the functionality.
         *  - The priority assigned to the task.
         *  - The task handle is not required, so NULL is passed. 
        xTaskCreate( prvDataOutputTask2,
                     "Output2",
                     configMINIMAL_STACK_SIZE,
                     NULL,
                     tskIDLE_PRIORITY + 1,
                     NULL ); */
    }
}
/*-----------------------------------------------------------*/

/*-----------------------------------------------------------*/

static void ajTestUART0OutputTask( void *pvParameters )
{
TickType_t xTimeNow;

    for( ;; )
    {
        /* Obtain the current tick count. */
        xTimeNow = xTaskGetTickCount();

        /* Print out the string using the newly defined function. */
        ajUART0printf( "AJ-Task TEST UART0 OUTPUT "
                       "thread safety, time stamp = %d\n", xTimeNow);

        vTaskDelay(2000);
    }
}
/*-----------------------------------------------------------*/


void vApplicationTickHook( void )
{
    /* This function will be called by each tick interrupt if
        configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h.  User code can be
        added here, but the tick hook is called from an interrupt context, so
        code must not attempt to block, and only the interrupt safe FreeRTOS API
        functions can be used (those that end in FromISR()). */

    /* Only the full demo uses the tick hook so there is no code is
        executed here. */
}

