/****************************************************
 FILE NAME		: f05-hexdump.c
  ORIGINATOR	: A Joshi
  LAST EDITED   : 18 OCT 2025
  PROJECT       : WS12_EPCB7011
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/

#include "f05-hexdump.h"
#include "f01-watchdog.h"
///////////////////////////////////////////////////////////////////
//fns defined here...
void DumpHex(const void* data, size_t size);
void send_asciicode(uint8_t byt);

//void send_asciicode_ITM(uint8_t byt);

///////////////////////////////////////////////////////////////////


void DumpHex(const void* data, size_t size)
{
	char ascii[17];
	size_t i, j;
	ascii[16] = '\0';
	for (i = 0; i < size; ++i)
	{
		printf("%02X ", ((unsigned char*)data)[i]);
		if (((unsigned char*)data)[i] >= ' ' && ((unsigned char*)data)[i] <= '~')
		{
			ascii[i % 16] = ((unsigned char*)data)[i];
		}
		else
		{
			ascii[i % 16] = '.';
		}
		if ((i+1) % 8 == 0 || i+1 == size)
		{
			printf(" ");
			if ((i+1) % 16 == 0)
			{
				printf("|  %s \r\n", ascii);
			}
			else if(i+1 == size)
			{
				ascii[(i+1) % 16] = '\0';
				if ((i+1) % 16 <= 8)
				{
					printf(" ");
				}
				for (j = (i+1) % 16; j < 16; ++j)
				{
					printf("   ");
				}
				printf("|  %s \r\n", ascii);
			}
		}
		iwdg_strobpulse();
	}
}

//ajprint(int dest, const char *pcString, ...);

void DumpHexDebug(const void* data, size_t size)
{
	char ascii[17];
	size_t i, j;
	ascii[16] = '\0';
	for (i = 0; i < size; ++i)
	{
	    ajprint(PORT_DEBUG,"%02X ", ((unsigned char*)data)[i]);
		if (((unsigned char*)data)[i] >= ' ' && ((unsigned char*)data)[i] <= '~')
		{
			ascii[i % 16] = ((unsigned char*)data)[i];
		}
		else
		{
			ascii[i % 16] = '.';
		}
		if ((i+1) % 8 == 0 || i+1 == size)
		{
			ajprint(PORT_DEBUG," ");
			if ((i+1) % 16 == 0)
			{
				ajprint(PORT_DEBUG,"|  %s \r\n", ascii);
			}
			else if(i+1 == size)
			{
				ascii[(i+1) % 16] = '\0';
				if ((i+1) % 16 <= 8)
				{
					ajprint(PORT_DEBUG," ");
				}
				for (j = (i+1) % 16; j < 16; ++j)
				{
					ajprint(PORT_DEBUG,"   ");
				}
				ajprint(PORT_DEBUG,"|  %s \r\n", ascii);
			}
		}
		iwdg_strobpulse();
	}
}


void send_asciicode(uint8_t byt)
{
	uint8_t str[5];
	sprintf((char *)str,"%02X ",byt);
	debug_msg((const char *)str);
	iwdg_strobpulse();
}

#if 0
void send_asciicode_ITM(uint8_t byt)
{
	uint8_t str[5];
	sprintf((char *)str,"%02X",byt);
	ITM_SendChar(str[0]);
	ITM_SendChar(str[1]);
	ITM_SendChar(0x20);
	iwdg_strobpulse();
}
#endif

