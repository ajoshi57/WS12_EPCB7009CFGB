/****************************************************
 FILE NAME		: f01-dif-cli-task.h
  ORIGINATOR	: A Joshi
  LAST EDITED   : 18 OCT 2025
  PROJECT       : WS12_EPCB7011
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/

#ifndef INC_F01_DIF_CLI_TASK_H_
#define INC_F01_DIF_CLI_TASK_H_


#define MAX_MATCH_COMMANDS  50
#define SHELL_MAX_ARGS 16
#define MAX_LINE_LENGTH  81
#define MAX_HISTORY_ENTRIES 8
#define HISTORY_BUFFER_ENTRY_SIZE 81
//
#define MAX_QUEUE 10
#define GEN_CMD_BASE_INDEX 0
//

////////////////////////////////////////////////////////////////////////////////////////
//Fn defined here....
void StartTaskU0Msg(void *argument);
////////////////////////////////////////////////////////////////////////////////////////


#endif /* INC_F01_DIF_CLI_TASK_H_ */
