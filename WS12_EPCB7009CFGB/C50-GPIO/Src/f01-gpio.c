/****************************************************
  FILE NAME		: f01-gpio.c
  ORIGINATOR	: A Joshi
  LAST EDITED   : 29 OCT 2026
  PROJECT       : WS12_EPCB7011
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/

#include "f01-gpio.h"
//
#define DEBUG 1
//
//Fns protos defined here...
void drive_nresetlogic(uint8_t lev);
void drive_nresetflash(uint8_t lev);
void drive_ncsflash(uint8_t lev);
//
void drive_i2cdevno(uint8_t devno);
void drive_ledselftest(uint8_t lev);
//
void drive_noamcomen(uint8_t lev); //low to enable handsahake signals on CLI RJ45
void drive_cardalarm(uint8_t lev);
void drive_cardready(uint8_t lev);
//
void drive_ncslogic(uint8_t lev); //not used for I2C
//
void setup_gpio_defaults(void);
void logic_reset_pulse(void);
//
void get_alarmrelay_status(uint8_t *status);
//
void set_noutput_enable(void);
//
//Fns defined here...
void drive_nresetlogic(uint8_t lev)
{
	if(lev == 0)
	{
		GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3,0x0);
	}
	else
	{
		GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3,GPIO_PIN_3);
	}
}

void drive_nresetflash(uint8_t lev)
{
	if(lev == 0)
	{
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_4, 0);
	}
	else
	{
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_PIN_4);
	}
}


void drive_ncsflash(uint8_t lev)
{
		if(lev == 0)
		{
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0x0 );

		}
		else
		{
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2 );
		}
}

void drive_i2cdevno(uint8_t devno)
{
	switch(devno)
	{
		case 0:
		GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5, 0x0 ); //dev2
		GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5, 0x0 ); //dev1
		GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0x0 ); //dev0
		break;
		case 1:
		GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5, 0x0 ); //dev2
		GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5, 0x0 ); //dev1
		GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_PIN_4 ); //dev0
		break;
		case 2:
		GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5, 0x0 ); //dev2
		GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5, GPIO_PIN_5 ); //dev1
		GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0x0 ); //dev0
		break;
		case 3:
		GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5, 0x0 ); //dev2
		GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5, GPIO_PIN_5 ); //dev1
		GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_PIN_4 ); //dev0
		break;
		case 4:
		GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5, GPIO_PIN_5 ); //dev2
		GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5, 0x0 ); //dev1
		GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0x0 ); //dev0
		break;
		case 5:
		GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5, GPIO_PIN_5 ); //dev2
		GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5, 0x0 ); //dev1
		GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_PIN_4 ); //dev0
		break;
		case 6:
		GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5, GPIO_PIN_5 ); //dev2
		GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5, GPIO_PIN_5 ); //dev1
		GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0x0 ); //dev0
		break;
		case 7:
		GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5, GPIO_PIN_5 ); //dev2
		GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5, GPIO_PIN_5 ); //dev1
		GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_PIN_4 ); //dev0
		break;
		default:
		GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5, 0x0 ); //dev2
		GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5, 0x0 ); //dev1
		GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0x0 ); //dev0
		break;
	}

}

void drive_ledselftest(uint8_t lev)
{
    if(lev == 0)
    {
        GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, 0x0 );
    }
    else
    {
        GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, GPIO_PIN_1 );
    }
}


//
void drive_noamcomen(uint8_t lev) //low to enable handsahake signals on CLI RJ45
{
	if(lev == 0)
	{
		GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0, 0x0 );
	}
	else
	{
		GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0, GPIO_PIN_0 );
	}
	//
}

void drive_cardalarm(uint8_t lev)
{
	if(lev == 0)
	{
		GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5, 0x0 );
	}
	else
	{
		GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5, GPIO_PIN_5 );
	}
	//

}

void drive_cardready(uint8_t lev)
{
	if(lev == 0)
	{
		GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, 0x0 );
	}
	else
	{
		GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, GPIO_PIN_4 );
	}
	//
}




void drive_ncslogic(uint8_t lev)
{
	if(lev == 0)
	{
		GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, 0x0 );
	}
	else
	{
		GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, GPIO_PIN_4 );
	}
}

void setup_gpio_defaults(void)
{
	drive_nresetlogic(1);
	drive_nresetflash(1);
	drive_ncsflash(1);
	drive_i2cdevno(0);
	drive_ledselftest(0);
	//
    drive_noamcomen(0); //low to enable handsahake signals on CLI RJ45
    drive_cardalarm(0);
    drive_cardready(0);
    //
    drive_ncslogic(1); //not used for I2C
    //
	//////////////////////////////////////
	//
}


//#################################################################
//Fx used initialise
//#################################################################
void logic_reset_pulse(void)
{
	drive_nresetlogic(0);drive_nresetlogic(0);drive_nresetlogic(0);drive_nresetlogic(0);
	drive_nresetlogic(0);drive_nresetlogic(0);drive_nresetlogic(0);drive_nresetlogic(0);
	drive_nresetlogic(0);drive_nresetlogic(0);drive_nresetlogic(0);drive_nresetlogic(0);
	//
	drive_nresetlogic(1);
	//
#if DEBUG
	debug_msg("Logic Reset Applied !\r\n");
#endif
}
//#################################################################



void get_alarmrelay_status(uint8_t *status)
{
    int32_t i32Val;
    i32Val=GPIOPinRead(GPIO_PORTK_BASE,GPIO_PIN_2);
    *status = (i32Val >> 2) & 0x01;
}

void set_noutput_enable(void)
{
    GPIOPinTypeGPIOOutput(GPIO_PORTK_BASE, GPIO_PIN_3);
    GPIOPinWrite(GPIO_PORTK_BASE, GPIO_PIN_3, 0x0 );
}
