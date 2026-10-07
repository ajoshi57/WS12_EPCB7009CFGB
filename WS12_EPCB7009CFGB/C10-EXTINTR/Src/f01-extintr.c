/****************************************************
  FILE NAME     : f01-extintr.c
  ORIGINATOR    : A Joshi
  LAST EDITED   : 25 SEPT 2026
  PROJECT       : WS12_EPCB7009
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/

#include "f01-extintr.h"
//
#define DEBUG 0
//

extern QueueHandle_t QueueEXTIntrHandle;



void configure_ext_gpio_inputs(void);
void setup_ext_gpio_intrs(void);
void GPIO_PortH_Ext_Intr_Handler(void);
void post_ext_intr(uint8_t inputno);


void configure_ext_gpio_inputs(void)
{
    //Input0
    GPIOPinTypeGPIOInput(GPIO_PORTH_BASE, GPIO_PIN_0);
    GPIOPadConfigSet(GPIO_PORTH_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    //Input1
    GPIOPinTypeGPIOInput(GPIO_PORTH_BASE, GPIO_PIN_1);
    GPIOPadConfigSet(GPIO_PORTH_BASE, GPIO_PIN_1, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    //Input2
    GPIOPinTypeGPIOInput(GPIO_PORTH_BASE, GPIO_PIN_2);
    GPIOPadConfigSet(GPIO_PORTH_BASE, GPIO_PIN_2, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    //Input3
    GPIOPinTypeGPIOInput(GPIO_PORTH_BASE, GPIO_PIN_3);
    GPIOPadConfigSet(GPIO_PORTH_BASE, GPIO_PIN_3, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

}

void setup_ext_gpio_intrs(void)
{
    //////////////////////////////////////////////////////////////////
    // Disable interrupt during setup & clear
    GPIOIntDisable(GPIO_PORTH_BASE, GPIO_PIN_0);
    GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_0);
    GPIOIntDisable(GPIO_PORTH_BASE, GPIO_PIN_1);
    GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_1);
    GPIOIntDisable(GPIO_PORTH_BASE, GPIO_PIN_2);
    GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_2);
    GPIOIntDisable(GPIO_PORTH_BASE, GPIO_PIN_3);
    GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_3);
    //////////////////////////////////////////////////////////////////
    // Rising edge trigger, Enable Intr PH0,1,2,3
    GPIOIntTypeSet(GPIO_PORTH_BASE, GPIO_PIN_0, GPIO_RISING_EDGE);
    GPIOIntEnable(GPIO_PORTH_BASE, GPIO_PIN_0);
    GPIOIntTypeSet(GPIO_PORTH_BASE, GPIO_PIN_1, GPIO_RISING_EDGE);
    GPIOIntEnable(GPIO_PORTH_BASE, GPIO_PIN_1);
    GPIOIntTypeSet(GPIO_PORTH_BASE, GPIO_PIN_2, GPIO_RISING_EDGE);
    GPIOIntEnable(GPIO_PORTH_BASE, GPIO_PIN_2);
    GPIOIntTypeSet(GPIO_PORTH_BASE, GPIO_PIN_3, GPIO_RISING_EDGE);
    GPIOIntEnable(GPIO_PORTH_BASE, GPIO_PIN_3);
    //////////////////////////////////////////////////////////////////
    //Register ISR for Port H
    //Enable GPIOH interrupt in NVIC
    // Enable global interrupts
    GPIOIntRegister(GPIO_PORTH_BASE, GPIO_PortH_Ext_Intr_Handler);
    IntEnable(INT_GPIOH);
    IntMasterEnable();
    //////////////////////////////////////////////////////////////////

}



volatile bool buttonPressed = false;

void GPIO_PortH_Ext_Intr_Handler(void)
{
    uint32_t ui32Status;
    uint8_t inputno = 0;
    //
    // 1. Get the masked interrupt status for Port H
    ui32Status = GPIOIntStatus(GPIO_PORTH_BASE, true);
    // 2. Clear the asserted interrupts immediately
    GPIOIntClear(GPIO_PORTH_BASE, ui32Status);
    // 3. Evaluate which pin triggered the event using a bitwise AND
    if(ui32Status & GPIO_PIN_0)
    {
        // Handle the event for Pin 1
        inputno = 1;
    }
    //
    if(ui32Status & GPIO_PIN_1)
    {
        // Handle the event for Pin 2
        inputno = 2;
    }
    //
    if(ui32Status & GPIO_PIN_2)
    {
        // Handle the event for Pin 3
        inputno = 3;
    }
    //
    if(ui32Status & GPIO_PIN_3)
    {
        // Handle the event for Pin 4
        inputno = 4;
    }
    post_ext_intr( inputno);
}


void post_ext_intr(uint8_t inputno)
{
    //adds received into queue
    xQueueSendToBackFromISR(QueueEXTIntrHandle, &inputno, 0);

}




