/****************************************************
  FILE NAME		: f01-i2clogic.c
  ORIGINATOR	: A Joshi
  LAST EDITED   : 18 OCT 2025
  PROJECT       : WS12_EPCB7011
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/



#include "f01-i2clogic.h"

//
#define DEBUG 0
//


//fns defined here...

//=================================================
//**AJ
static SemaphoreHandle_t g_mutex_access_i2c;
//=================================================
int create_mutex_access_i2c(void);
void logic_reset_pulse(void);
int logic_write_cycle(uint8_t page, uint8_t addr,uint8_t data);
int logic_read_cycle(uint8_t page, uint8_t addr,uint8_t *data);



//fns defined here...
int create_mutex_access_i2c(void)
{
	int ret = 0;
	///////////////////////////////////////////////////////////////////////
	g_mutex_access_i2c = xSemaphoreCreateMutex();
	if( g_mutex_access_i2c == NULL )
	{
		debug_msg("ERROR create, 'g_mutex_access_i2c' !\r\n");
		ret = -1;
	}
	else
	{
		//The semaphore can now be used.
		debug_msg("SUCCESS create, 'g_mutex_access_i2c' !\r\n");
	}
	///////////////////////////////////////////////////////////////////////
	////////////////////
    return ret;
    ////////////////////
    //
}



//#################################################################
//Fx used to write one byte to LOGIC with up to 256 pages, 256 addrs
//each page address 0-255
//Data = 8-bit
//#################################################################
int logic_write_cycle(uint8_t page, uint8_t addr,uint8_t data)
{
	int ret = 0;
	uint8_t array[5];
	memset(array,0,sizeof(array));
	array[0] = addr;
	array[1] = data;
	///////////////////////////////////////////////////////////////////
	if( xSemaphoreTake( g_mutex_access_i2c, ( TickType_t ) 50 ) == pdTRUE )
    {
		I2C1SendArray(&array[0]);

		///////////////////////////////////////////////////////
		xSemaphoreGive( g_mutex_access_i2c);
		///////////////////////////////////////////////////////
    }
	else
	{
	   	debug_msg("ERROR set, 'g_mutex_access_i2c', mutex not obtained !\r\n");
	    	ret = -1;
	}
	//
#if DEBUG
	debug_msg("WR Logic Page %02X Addr %02X Data %02X\r\n", page, addr, data);
#endif
	return ret;
}
//#################################################################


//#################################################################
//Fx used to read one byte fm LOGIC with up to 256 pages, 256 addrs
//each page address 0-255
//Data = 8-bit
//#################################################################
int logic_read_cycle(uint8_t page, uint8_t addr,uint8_t *data)
{
	int ret = 0;
	//
	if( xSemaphoreTake( g_mutex_access_i2c, ( TickType_t ) 50 ) == pdTRUE )
    {
		*data = I2C1Receive(addr);
		///////////////////////////////////////////////////////
		xSemaphoreGive( g_mutex_access_i2c);
		///////////////////////////////////////////////////////
    }
	else
	{
	   	debug_msg("ERROR set, 'g_mutex_access_i2c', mutex not obtained !\r\n");
	    	ret = -1;
	}
	//
#if DEBUG
	debug_msg("READ Logic Page %02X Addr %02X Data %02X\r\n", page, addr, rddata);
#endif
	return ret;
}
//#################################################################


