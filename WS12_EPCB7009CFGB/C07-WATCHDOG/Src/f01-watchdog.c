/****************************************************
  FILE NAME		: f01-watchdog.c
  ORIGINATOR	: A Joshi
  LAST EDITED   : 18 OCT 2025
  PROJECT       : WS12_EPCB7011
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/

#include "f01-watchdog.h"

static uint8_t g_stopwatchdog = 0;

//****************************************************************************
//
// System clock rate in Hz.
//
//****************************************************************************
extern uint32_t get_current_clock(void);

//*****************************************************************************
//
// Flag to tell the watchdog interrupt handler whether or not to clear the
// interrupt (feed the watchdog).
//
//*****************************************************************************
volatile bool g_bFeedWatchdog = true;

//*****************************************************************************
//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif



/*****************************************************************************
** Function name:       init_intr_watchdog()
**
** Descriptions:        Setup a 1SEC Watchdog for resetting APP
**                      Requires a cmd flag from TIMER0 for reset
**                      (incase timer is not cleared for 5secs by APP fns.)
** Parameters:          NONE
** Returned value:      NONE
*****************************************************************************/
void init_intr_watchdog(void)
{
    uint32_t clk;
    clk = get_current_clock();
    //
    // Enable the watchdog 0 peripheral
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_WDOG0);
    // Enable the watchdog interrupt.
    //
    MAP_IntEnable(INT_WATCHDOG);
    //
    // Set the period of the watchdog timer to 1 second.
    //
    MAP_WatchdogReloadSet(WATCHDOG0_BASE, clk);
    //
    // Enable reset generation from the watchdog timer.
    //
    MAP_WatchdogResetEnable(WATCHDOG0_BASE);
    //
    // Enable the watchdog timer.
    //This will be enabled after bootup is complete!!
    MAP_WatchdogEnable(WATCHDOG0_BASE);

}


//*****************************************************************************
//
// The interrupt handler for the watchdog.  This feeds the dog (so that the
// processor does not get reset)
// THIS FX IS TO BE REGISTEREDD IN STARTUP.
//
//*****************************************************************************
void WatchdogIntHandler(void)
{
    //
    // If we have been told to stop feeding the watchdog, return immediately
    // without clearing the interrupt.  This will cause the system to reset
    // next time the watchdog interrupt fires.
    //
    if(!g_bFeedWatchdog)
    {
        return;
    }

    //
    // Clear the watchdog interrupt.
    //
    MAP_WatchdogIntClear(WATCHDOG0_BASE);

    //
    // Invert the GPIO PN0 value.
    //
    MAP_GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0,
                     (MAP_GPIOPinRead(GPIO_PORTN_BASE, GPIO_PIN_0) ^
                                     GPIO_PIN_0));
}

