/****************************************************
  FILE NAME     : f01-timer0.c
  ORIGINATOR    : A Joshi
  LAST EDITED   : 18 OCT 2025
  PROJECT       : WS12_EPCB7011
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/

#include "f01-timer0.h"

#define EXPIRE_COUNT 5 //5 secs
//
extern uint32_t get_current_clock(void);

//*****************************************************************************
//
// EXPIRY COUNTER which is cleared by strobepulse calls
// interrupt (feed the watchdog).
//
//*****************************************************************************
volatile uint8_t g_expirycounter = 0;
volatile uint8_t g_stopwatchdog = 0;
//*****************************************************************************
//
// Flag to tell the watchdog interrupt handler whether or not to clear the
// interrupt (feed the watchdog).
//
//*****************************************************************************
extern volatile bool g_bFeedWatchdog;



void init_intr_timer0(void);
static void timer0_intr_handler(void);
void iwdg_strobpulse(void);
void stop_watchdog(void);

/*****************************************************************************
** Function name:       init_intr_timer0()
**
** Descriptions:        Initialise TIMER0 (TIMER_A)
**                      used for counting upto EXPIRE_COUNT to ensure
**                      flag set to Watchdog Interrupt...to initiate a reset.
** Parameters:          NONE
** Returned value:      NONE
*****************************************************************************/
void init_intr_timer0(void)
{
    uint32_t clk;
    g_expirycounter = 0;
    clk = get_current_clock();
    debug_msg("SYS CLK = %d\r\n",clk);
    //****************************************************************//
    // Configure the 32-bit periodic timer.
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    ROM_TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    ROM_TimerLoadSet(TIMER0_BASE, TIMER_A, (clk)  ); //1sec timer
    //
    // Setup the interrupts for the timer timeouts.
    TimerIntRegister(TIMER0_BASE, TIMER_A,timer0_intr_handler);
    ROM_IntPrioritySet(INT_TIMER0A,0xC0);
    ROM_IntEnable(INT_TIMER0A);
    ROM_TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    //
    // Enable the timer 0 whenever required (after boot-up is complete & local static global is updated !!)
    ROM_TimerEnable(TIMER0_BASE, TIMER_A);
    //
    //****************************************************************//
}


//*****************************************************************************
//
// interrupt handler : 'event_type_keepalive'
//
//*****************************************************************************
static void timer0_intr_handler(void)
{
    ROM_TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    //
    if(g_expirycounter != EXPIRE_COUNT)
    {
        g_expirycounter++;
    }
    else
    {
        //g_bFeedWatchdog = false;
    }
    //
}

void iwdg_strobpulse(void)
{
    if(g_stopwatchdog == 0)
    {
        g_expirycounter = 0;
    }
}

void stop_watchdog(void)
{
    g_stopwatchdog = 1;
}


