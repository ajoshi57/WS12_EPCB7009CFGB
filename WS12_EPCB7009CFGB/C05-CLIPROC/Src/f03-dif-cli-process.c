/****************************************************
 FILE NAME		: f03-dif-cli-process.c
  ORIGINATOR	: A Joshi
  LAST EDITED   : 18 OCT 2025
  PROJECT       : WS12_EPCB7011
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/

/**/
/* Includes ------------------------------------------------------------------*/

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "f05-hexdump.h"
#include "f02-dif-cmdlist.h"
#include "f01-dif-cli-task.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>
//
#include "f01-console.h"
#include "f01-defines.h"
//
//externs defined in f02-dif-cmdlist.c
extern CMDMSG_T *COMMAND_LIST[MAX_NO_COMMANDS + 1];
//
/////////////////////////////////////////////////////////
//fns defined here...
int convert_to_lower(char *command);
int parse_line(char *buf, char **argv);
int process_command(char *command, char *responsebuf);
/////////////////////////////////////////////////////////

//fns defined here...
int convert_to_lower(char *command)
{
	int i;
	for (i = 0; i < strlen(command); i++)
	{
		if (command[i] >= 65 && command[i] <= 90)
		{
			command[i] = command[i] + 32;
		}
	}
	return RET_SUCCESS;
}


int parse_line(char *buf, char **argv)
{
	int argc = 0;
	while (*buf && isspace((unsigned char)*buf))
		buf++;/* eat-up initial spaces */

	while (*buf)
	{
		*argv++ = buf;
		argc++;

		if (argc > SHELL_MAX_ARGS)
		{
			--argc;
			return argc;
		}
		while (*buf && !isspace((unsigned char)*buf) && *buf != '\r' && *buf != '\n')
			buf++;
		if (*buf)
		{
			/* stop after hitting linefeed or carriage return which signifies the end
			 of the message */
			if (*buf == '\r' || *buf == '\n')
			{
				*buf++ = '\0';
				break;
			}
			else
			{
				*buf++ = '\0';
			}
		}

		// skip whitespace between arguments
		while (*buf && isspace((unsigned char)*buf))
		{
			buf++;

			/* stop at cr-lf or cr */
			if (*buf == '\r' || *buf == '\n')
				*buf = 0;
		}

	}
	*argv = (char *) 0;
	return argc;
}

int process_command(char *command, char *responsebuf)
{
	char **argv;
	int argc;
	int i;
	uint16_t cmdno = 0;
	uint8_t cmdkwlen;
	bool result;

	argv = (char **) pvPortMalloc(SHELL_MAX_ARGS * (sizeof(char*)));
	if (argv == NULL)
	{
		//printf("[%s][%d] malloc Failed\n",__FILE__,__LINE__);
		sprintf(responsebuf, "$$[0027]EXECUTION ERROR 1\r\n@FAIL@\r\n##");
		//SendMiscMsg(1,g_response_buf);
		return RET_FAIL;
	}

	if (strncmp(command, "upgrade", 7) == 0)
	{ /* case sensitive commands*/

	}
	else if (strncmp(command, "UPGRADE", 7) == 0)
	{ /* case sensitive commands*/

	}
	else
	{ /* case insensitive commands*/
		convert_to_lower(command);
	}
	memset(responsebuf, '\0', MAX_RESPONSE_PAGE);
	argc = parse_line(command, (char **) argv);
	if (argc == 0)
	{
		sprintf(responsebuf, "$$[0027]EXECUTION ERROR 2\r\n@FAIL@\r\n##");
		//sprintf(responsebuf, "EXECUTION ERROR 2");
		 vPortFree(argv);
		return RET_FAIL;
	}
	convert_to_lower(argv[0]);
	for (i = 0; i < argc; i++)
	{
		//printf("[%s][%d]argv[%d]%s argc =%d\n",__FILE__,__LINE__,i,argv[i],argc);
	}
	result = find_command(COMMAND_LIST, argv[0], &cmdkwlen, &cmdno);
	if(result == true)
	{
		ajUART0crlf();
		if (RET_FAIL
				== COMMAND_LIST[cmdno]->command_handler((int) argc,
						(char **) argv, cmdkwlen, (char *) responsebuf))
		{
			//strncat(responsebuf, "\r\nEXECUTION ERROR 3\r\n",strlen("\r\nEXECUTION ERROR 3\r\n"));
			 vPortFree(argv);
			return RET_FAIL;
		}
	}
	else
	{
		sprintf(responsebuf, "$$[0043]COMMAND NOT FOUND\r\nSYNTAX ERROR !\r\n@FAIL@\r\n##");
		 vPortFree(argv);
		return RET_FAIL;
	}
	vPortFree(argv);
	return RET_SUCCESS;
}



