/****************************************************
  FILE NAME		: f02-dif-cmdlist.c
  ORIGINATOR	: A Joshi
  LAST EDITED   : 18 OCT 2025
  PROJECT       : WS12_EPCB7011
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/
/* Includes ------------------------------------------------------------------*/

#include "f02-dif-cmdlist.h"
#include "string.h"

CMDMSG_T *COMMAND_LIST[MAX_NO_COMMANDS + 1] =
{ NULL };
//
int GEN_CMD_END_INDEX;

/////////////////////////////////////////////////////////
//fns defined here...
int add_command_to_list(CMDMSG_T *cmd);
bool find_command(CMDMSG_T **command_list, char *cmd_name, uint8_t *cmdkwlen, uint16_t *cmdno);
/////////////////////////////////////////////////////////


int add_command_to_list(CMDMSG_T *cmd)
{
	int i;
	if (cmd == NULL)
	{
		return -1;
	}
	for (i = 0; i < MAX_NO_COMMANDS; i++)
	{
		if (COMMAND_LIST[i] == NULL)
		{
			break;
		}
	}
	if (i == MAX_NO_COMMANDS)
		return -1;
	COMMAND_LIST[i] = cmd;
	COMMAND_LIST[i + 1] = NULL;
	GEN_CMD_END_INDEX=i;
	return 0;
}


bool find_command(CMDMSG_T **command_list, char *cmd_name, uint8_t *cmdkwlen, uint16_t *cmdno)
{
	bool ret = false;
	int i;
	uint8_t len = strlen(cmd_name);
	//
	for (i = 0; i < MAX_NO_COMMANDS && command_list[i] != NULL; i++)
	{
		if (strcmp(command_list[i]->command_name, cmd_name) == 0)
		{
			*cmdno = i;
			ret = true;
		}
	}
	*cmdkwlen = len;
	return ret;
}



