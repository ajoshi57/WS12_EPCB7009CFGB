/****************************************************
 FILE NAME		: f01-dif-cli-task.c
  ORIGINATOR	: A Joshi
  LAST EDITED   : 18 OCT 2025
  PROJECT       : WS12_EPCB7011
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/
/* Standard includes. */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/* Hardware includes. */
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "drivers/rtos_hw_drivers.h"
#include "utils/uartstdio.h"
/*-----------------------------------------------------------*/
/* Private includes -----------------------------------------*/
/* USER CODE BEGIN Includes */
#include "f01-console.h"
#include "f01-defines.h"
#include "f01-u0msgtask.h"
#include "f99-struct-dif-cmdmsg.h"
#include "f01-dif-cli-task.h"
#include "f02-dif-cmdlist.h"
#include "f01-watchdog.h"
/*-----------------------------------------------------------*/
//
//FNs Protos Defined here...


//
//externs defined in cmdlist.c
extern CMDMSG_T *COMMAND_LIST[MAX_NO_COMMANDS + 1];
extern int GEN_CMD_END_INDEX;

//
extern void init_difcommand_list_all(void);
//
extern void debug_status_char(char c);
//
extern void init_dif_cli_command_list_all(void);
//
extern int process_command(char *command, char *responsebuf);

//
//#define DEBUG 0

static char g_response_buf[MAX_RESPONSE_PAGE];

#define CTRL(s)         ((s)-'@')
#define PromptStr SYSPROMPT
//


////////////////////////////////////////////////////////////////////////////////////////
//Fn defined here....
static int get_maxmatch(int list_size, char *match[MAX_MATCH_COMMANDS + 1]);
static void add_history(char *buffer,
		char hist[MAX_HISTORY_ENTRIES][HISTORY_BUFFER_ENTRY_SIZE],
		int *prev_count, int *no_of_history);
static char* get_history_previous(
		char hist[MAX_HISTORY_ENTRIES][HISTORY_BUFFER_ENTRY_SIZE],
		int *prev_count);
static char* get_history_next(
		char hist[MAX_HISTORY_ENTRIES][HISTORY_BUFFER_ENTRY_SIZE],
		int *prev_count);
//
void StartTaskU0Msg(void *argument);
////////////////////////////////////////////////////////////////////////////////////////

int get_maxmatch(int list_size, char *match[MAX_MATCH_COMMANDS + 1]) {
	int i;
	int j;
	char key;
	for (i = 0; i < strlen(match[0]); i++) {
		key = match[0][i];
		for (j = 1; j < list_size && j < MAX_MATCH_COMMANDS; j++) {
			if (match[j][i] != key)
				return i;
		}
	}
	return i;

}

void add_history(char *buffer,
		char hist[MAX_HISTORY_ENTRIES][HISTORY_BUFFER_ENTRY_SIZE],
		int *prev_count, int *no_of_history)
{
	char temp[100];
	int i;
	memset(temp,0,sizeof(temp));
	strncpy(temp, buffer, HISTORY_BUFFER_ENTRY_SIZE - 1); /* expand to 80 char only if more then chop it off */

	*prev_count = 0;

	if (*no_of_history == 0) {                   // empty list
		strncpy(hist[0], temp, HISTORY_BUFFER_ENTRY_SIZE - 1);                  // add to 0th position
		(*no_of_history)++;
		return;
	}
	if (*no_of_history == MAX_HISTORY_ENTRIES) {
		for (i = MAX_HISTORY_ENTRIES - 1; i != 0; i--) {
			strncpy(hist[i], hist[i - 1],HISTORY_BUFFER_ENTRY_SIZE - 1);         // shift history entries
		}
		strncpy(hist[0], temp, HISTORY_BUFFER_ENTRY_SIZE - 1);                //enter new entry to 0th position
		*prev_count = 0;
		return;
	}
	if (*no_of_history < MAX_HISTORY_ENTRIES) {
		for (i = *no_of_history; i != 0; i--) {
			strncpy(hist[i], hist[i - 1], HISTORY_BUFFER_ENTRY_SIZE - 1);         // shift history entries
		}
		strncpy(hist[0], temp, HISTORY_BUFFER_ENTRY_SIZE - 1);                //enter new entry to 0th position
		(*no_of_history)++;
		*prev_count = 0;
		return;
	}
}

char* get_history_previous(
		char hist[MAX_HISTORY_ENTRIES][HISTORY_BUFFER_ENTRY_SIZE],
		int *prev_count) {
	int retslotindex;

	if (*prev_count == MAX_HISTORY_ENTRIES - 1) {
		return hist[MAX_HISTORY_ENTRIES - 1];
	}

	retslotindex = *prev_count - 1;

	if (*prev_count < 0) {
		*prev_count = 0;
		return NULL;
	}
	if (retslotindex < 0)
		return NULL;

	return hist[retslotindex];
}

char* get_history_next(
		char hist[MAX_HISTORY_ENTRIES][HISTORY_BUFFER_ENTRY_SIZE],
		int *prev_count) {
	int retslotindex;

	if (*prev_count < 0) {
		*prev_count = 0;
		return NULL;
	}

	retslotindex = *prev_count;

	return hist[retslotindex];

}


void StartTaskU0Msg(void *argument)
{
	BaseType_t status;
	char hist[MAX_HISTORY_ENTRIES][HISTORY_BUFFER_ENTRY_SIZE];
	int no_of_history = 0;
	int prev_count= 0; //stores how many times prev_history is called
	char *ptrHistory;
	char *match[MAX_MATCH_COMMANDS + 1];
	int dest = PORT_USER;
	char c;
	//char PromptStr_bkup[25];
	char pbuffer[MAX_LINE_LENGTH];
	//char PromptStr[25];

	//int proceed;
	int sMessage;
	int i = 0;
	int j =0;
	int state = 0;
	int insert = 0;
	int index = 0;
	int length = 0;
	int cmd_count = 0;
	int index_bkup = 0;
	int break_count = 0;
	int max_match_len = 0;
	//
	//uint8_t login_state = 0;
	//uint8_t user_index = 0;
	//uint8_t password_count = 0;
	//uint8_t ignore_first = 0;
	//
	//login_state = UNAME_ASKED;
	//=============================================
	//memset(pbuffer, '\0', MAX_LINE_LENGTH);
	//uint8_t displayunit;
	uint8_t title[10];
	memset(title, 0, sizeof(title));
	//==============================================
	//ger overall pass fail
#if 0
	size_t iSize;
	uint8_t *pcbuff_bootstatus;
	tbootstatus *bootstatus;
	uint8_t result_overall, serialports;
	//
	iSize = sizeof(tbootstatus);
	pcbuff_bootstatus = (uint8_t *) pvPortMalloc(iSize);
	if (pcbuff_bootstatus == NULL)
	{
		debug_msg("Could not reallocate bootstatus memory\r\n");
		while(1)
		{
		}
	}
	else
	{
		debug_msg("tbootstatus iSize = %d \r\n",iSize);
	}
	//
	//
	bootstatus = (tbootstatus *)pcbuff_bootstatus;
#endif
	//===============================================

#if 0
	if( xSemaphoreTake( g_semaphore_boot_to_CLI, ( TickType_t ) 10000 ) == pdTRUE )
	{
		//We were able to obtain the semaphore so 'proceed'
	    proceed = 1;
	}
	else
	{
		//We could not obtain the semaphore - timeout
	    proceed = 0;
	    UART7Send("EXCEPTION ! - CLI Task BOOT-UP TIMEOUT\r\n",strlen("EXCEPTION ! - CLI Task BOOT-UP TIMEOUT\r\n"));
	}
#endif
	//
	//=============================================================
	//copy_bootstatus_global(bootstatus); //this is already init in 'bootup_task'
	//result_overall = bootstatus->result_overall;
	//vPortFree(pcbuff_bootstatus); //free 'tbootstatus' mem
	debug_msg("Initialising CMD LIST....\r\n");
	iwdg_strobpulse();
	//debug_msg("Boot Result  :%s\r\n",overallbootres_to_str(result_overall));
	//....
	//=============================================================
	//construct_promptstr_global_mutex(&result_overall, &serialports);
	//=============================================================
	//pass device type to decide Which DEV Specific CLI to include
	//
	init_dif_cli_command_list_all();
	//=============================================================
	debug_msg("CMD COUNT %d\r\n", GEN_CMD_END_INDEX);
	//
	//================================================
	//memset(PromptStr,0,PROMPTBUF_LEN);
	//strncpy(PromptStr, UNAME_STR, strlen(UNAME_STR));
	//paste_promptstr_global((uint8_t *)PromptStr);
	//=================================================
	//
	for(j = 0;j < MAX_HISTORY_ENTRIES;j++)
	{
		memset(hist[j],0,HISTORY_BUFFER_ENTRY_SIZE);
	}
	//
	print(dest, PromptStr);

	//
	while (1)
	{
		iwdg_strobpulse();

#if 0

			if (login_state == NOT_AVAILABLE) {
				login_state = UNAME_ASKED;
				strcpy(PromptStr, UNAME_STR);
			} else if (login_state == UNAME_ASKED)
				strcpy(PromptStr, UNAME_STR);
			else if (login_state == PASSWORD_ASKED)
				strcpy(PromptStr, PASSWD_STR);
			else if (login_state == SHELL_LOGIN)
				strcpy(PromptStr, SHELL_STR);
#endif

		do {
			iwdg_strobpulse();
			c = '\0';
			status = xQueueReceive(QueueU0CharsHandle, &sMessage,
					portMAX_DELAY);
			if (status != pdPASS)
			{
				continue;
			}

			//xQueueReceive(g_Uart0Queue, (void*) &sMessage, portMAX_DELAY);
			c = (char) sMessage;
			//debug_msg("rx\r\n");
			//debug_msg(c);
			switch (state) {
			case 0:                 // not an <ESC>
				if (c != '\033') {
					break;
				}
				state = 1;
				break;
			case 1:                 // not an <ESC> [
				if (c != '[') {
					state = 0;
					break;
				}
				state = 2;
				break;
#define CTRL_P 16
#define CTRL_N 14
#define CTRL_F 6
#define CTRL_B 2
#define BACKSPACE  127

			case 2:
				state = 0;
				switch (c) {
				case 'A': // cursor up key......previous
					//c='^P';     //  ascii of ^P  is 16 in decimal
					c = CTRL_P;
					break;
				case 'B':  // cursor down key....next
					//c='^N';     //  ascii of ^N  is 14 in decimal
					c = CTRL_N;
					break;
				case 'C': // cursor right key...forward
					//c = '^F';   //  ascii of ^F is 6 in decimal
					c = CTRL_F;
					break;
				case 'D': // cursor left key....backward
					//c = '^B';   //  ascii of ^B is 2 in decimal
					c = CTRL_B;
					break;
				case '3':							//Delete key pressed
					c = CTRL('D');
					break;
				default:        // not an <ESC> [ C|D
					c = '[';    // kill the <ESC>, return the rest
				} // End switch (c).
				break;
			default:
				print(dest, "Illegal State"); //---print_error function ERROR
				while (1) {
					;  // -------Spin here.
				}
			} // ----

		} while (state != 0);

		switch (c)
		{
		case CTRL('P'): //*********************************************UP ARROW  (previous history)
#if 0
							if ( (login_state < MIN_LOGIN_LVL)||(login_state > MAX_LOGIN_LVL)) {
								break;
							}
#endif
			if (no_of_history == 0) { //no history
				print(dest, "No history\r\n");
				break;
			}
			prev_count++;
			if (prev_count > no_of_history) {
				prev_count--; //restore prev count value
				print(dest, "%c", CTRL('G'));
				break;
			}
			ptrHistory = get_history_previous(hist, &prev_count);
			if (ptrHistory != NULL) {
				strcpy(pbuffer, ptrHistory);
				// move the curser to beginning of line
				if (insert != 0)
					print(dest, "\033[%dD", insert);
				// clear line, print new line
				//print(dest, "\033[K%s", pbuffer);
				print(dest, "\033[K%s", pbuffer);
				length = strlen(pbuffer);
				insert = length;
			} else
				print(dest, "%c", CTRL('G'));
			break;
		case CTRL('N'): //****************************************** DoWn ARROW  (next history)
#if 0
							if ( (login_state < MIN_LOGIN_LVL)||(login_state > MAX_LOGIN_LVL)) {
								break;
							}
#endif
			// get the next command from history
			if (prev_count == 0) {
				prev_count = 0;
				if (insert != 0)
					print(dest, "\033[%dD", insert);   //MOVE CORSOR TO BEGINING
				print(dest, "\033[K");
				memset(pbuffer, '\0', MAX_LINE_LENGTH);
				length = 0;
				insert = 0;
				c = 0;
				break;
			} else if (prev_count > 0) {
				prev_count--;
			}
			ptrHistory = get_history_next(hist, &prev_count);
			if (ptrHistory != NULL) {
				strcpy(pbuffer, ptrHistory);
				// move the curser to beginning of line
				if (insert != 0)
					print(dest, "\033[%dD", insert);
				// clear line, print new line
				print(dest, "\033[K%s", pbuffer);
				length = strlen(pbuffer);
				insert = length;
			} else
				print(dest, "%c", CTRL('G'));
			break;
		case CTRL('L'):
			print(dest, "\033[H\033[J"); //clear screen
			print(dest, PromptStr);
			print(dest, "%s", pbuffer);
			break;
		case CTRL('A'):
#if 0

							if ( (login_state < MIN_LOGIN_LVL)||(login_state > MAX_LOGIN_LVL)) {
								break;
							}
#endif
			//*******************************************move cursor to the beginning of the line
			if (insert != 0) {
				print(dest, "\033[%dD", insert);
				insert = 0;
			} else {
				print(dest, "%c", CTRL('G'));
			}
			break;
		case CTRL('B'):
#if 0

							if ( (login_state < MIN_LOGIN_LVL)||(login_state > MAX_LOGIN_LVL)) {
								break;
							}
#endif
			// ******************************************move cursor left
			if (insert != 0) {
				print(dest, "\033[D");
				insert--;
			} else {
				print(dest, "%c", CTRL('G'));
			}
			break;

		case CTRL('E'):
#if 0

							if ( (login_state < MIN_LOGIN_LVL)||(login_state > MAX_LOGIN_LVL)) {
								break;
							}
#endif
			// ******************************************move cursor to the end of the line
			if (insert != length) {
				print(dest, "\033[%dC", length - insert);
				insert = length;
			} else {
				print(dest, "%c", CTRL('G'));
			}
			break;
		case CTRL('F'):
#if 0

							if ( (login_state < MIN_LOGIN_LVL)||(login_state > MAX_LOGIN_LVL)) {
								break;
							}
#endif
			//******************************************* move cursor right
			if (insert != length) {
				print(dest, "\033[C");
				insert++;
			} else {
				print(dest, "%c", CTRL('G'));
			}
			break;
		case '\177':
		case CTRL('H'):
			//********************************************** delete charactor to the left of the cursor
			if (insert != 0) {
				// adjust insert point
				insert--;
				for (i = insert; i < length; i++)   // shift text in
						{
					*(pbuffer + i) = *(pbuffer + i + 1);    //  buffer
				}
				// backup one, delete under cursor
				print(dest, "\033[D\033[P");
				// adjust line length
				length--;
			} else {
				print(dest, "%c", CTRL('G'));
			}
			break;
		case CTRL('D'):
			//case '\177':
			//************************************************** delete charactor under the cursor
			if (insert < length) {
				for (i = insert; i < length; i++)   // shift text in
						{
					*(pbuffer + i) = *(pbuffer + i + 1);    //  buffer
				}
				// delete under the cursor
				print(dest, "deleting character\r\n");
				print(dest, "\033[P");
				// adjust line length
				length--;
			} else {
				print(dest, "%c", CTRL('G'));
			}
			break;
		case CTRL('U'):
			// *************************************************kill the line
			if (insert != 0) {
				*pbuffer = '\0';
				print(dest, "\033[%dD\033[K", insert);
				length = 0;
				insert = 0;
			}
			break;
		case CTRL('C'):
			//system("reset");
			break;

		case '\r':
		case '\n': //************************************************************************ ENTER
			if (length == 0) {
				*pbuffer = '\0';
				print(dest, PromptStr);
				insert = 0;
				length = 0;
#if 0

	        					if(login_state != PASSWORD_ASKED) {
	        						break;
	        					}
#endif
			}

			memset(g_response_buf, '\0', sizeof(g_response_buf));
#if 0

	        				if ( (login_state < MIN_LOGIN_LVL)||(login_state > MAX_LOGIN_LVL)) {
	        					;
	        				}
#endif
//	        				else {
			if (strcmp(hist[0], pbuffer) != 0) {
				add_history(pbuffer, hist, &prev_count, &no_of_history);
//	        					}
			}
			/*
			 This is old code (now removed) which straight away calls process_command
			 which calls the handler...
			 */
#if 0

	        				if ( login_state < MIN_LOGIN_LVL || login_state > MAX_LOGIN_LVL ) {
	        					if (login_state == NOT_AVAILABLE) {
	        						break;
	        					}
	        					else if(login_state == UNAME_ASKED) {
	        						check_uname(&login_state, &user_index, &password_count,pbuffer, g_response_buf);
	        					}
	        					else if(login_state == PASSWORD_ASKED) {
	        						check_passwd(&login_state, &user_index, &password_count, pbuffer, g_response_buf, &prev_count, &no_of_history);
	        						if ((login_state >= MIN_LOGIN_LVL)) {
	        							ignore_first = 1;
	        						}
	        					}
	        				}
#endif
			////////////////////////////////////////
			//new code
#if 0
	        				if (login_state == NOT_AVAILABLE)
	        				{
	        					login_state = UNAME_ASKED;
	        					memset(PromptStr,0,PROMPTBUF_LEN);
	        					strncpy(PromptStr, UNAME_STR,strlen(UNAME_STR));
	        					paste_promptstr_global((uint8_t *)PromptStr);
	        				}
	        				else if (login_state == UNAME_ASKED)
	        				{
	        					memset(PromptStr,0,PROMPTBUF_LEN);
	        					strncpy(PromptStr, UNAME_STR, strlen(UNAME_STR));
	        					paste_promptstr_global((uint8_t *)PromptStr);
	        				}
	        				else if (login_state == PASSWORD_ASKED)
	        				{
	        					memset(PromptStr,0,PROMPTBUF_LEN);
	        					strncpy(PromptStr, PASSWD_STR, strlen(PASSWD_STR));
	        					paste_promptstr_global((uint8_t *)PromptStr);
	        				}
	        				else if (login_state == SHELL_LOGIN)
	        				{
	        					memset(PromptStr,0,PROMPTBUF_LEN);
	        					strncpy(PromptStr, SHELL_STR,strlen(SHELL_STR));
	        					paste_promptstr_global((uint8_t *)PromptStr);
	        				}
	        				else if ((login_state >= MIN_LOGIN_LVL)
	        					&& (login_state != SHELL_LOGIN))
	        				{
	        					memset(PromptStr,0,PROMPTBUF_LEN);
	        					copy_promptstrbkp_global_mutex((uint8_t *)PromptStr);
	        					paste_promptstr_global((uint8_t *)PromptStr);
	        				}

	        				////////////////////////////////////////

	        				if ( login_state < MIN_LOGIN_LVL || login_state > MAX_LOGIN_LVL || ignore_first) {
	        					ignore_first = 0;
	        				}
	        				else {
	        					if (SUCCESS != process_command(pbuffer, g_response_buf))
	        					{
	        						//print(dest, "\n[%s][%d]process command fail\n", __FILE__, __LINE__);
	        						//Message(pbuffer);
	        					}
	        				}
#endif

			////////////////////////////////////////
	        memset(g_response_buf, '\0', sizeof(g_response_buf));//**AJ
			if (RET_SUCCESS != process_command(pbuffer, g_response_buf)) {
				//print(dest, "\n[%s][%d]process command fail\n", __FILE__, __LINE__);
				//Message(pbuffer);
			}

			////////////////////////////////////////
			if (g_response_buf[0] != 0) {
				g_response_buf[RESPONSE_BUF_SIZE - 1] = '\0';
#ifdef DEBUG
				debug_msg("\n\rExecuted CLI buffer size %d\r\n",
						strlen(g_response_buf));
#endif
				print(dest, "\n\r%s", g_response_buf);
			}
	        memset(g_response_buf, '\0', sizeof(g_response_buf));//**AJ
			g_response_buf[0] = '\0';
			memset(pbuffer, '\0', sizeof(pbuffer));
			print(dest, "\n\r");
			print(dest, PromptStr);   // to be handled by new cmd queue executor
			prev_count = 0;
			insert = 0;
			length = 0;
			break;
		case '\t': //************************************************************************* TAB
#if 0
	        				if ( (login_state < MIN_LOGIN_LVL)||(login_state > MAX_LOGIN_LVL)) {
	        					break;
	        				}
#endif
			/*----------For general commands---------*/
			for (index = 0; index < MAX_MATCH_COMMANDS; index++) {
				match[index] = NULL;
			}

			if (length == 0) {
				print(dest, "\n\r");
				break_count = 0;
				for (index = GEN_CMD_BASE_INDEX; index <= GEN_CMD_END_INDEX;
						index++) {
					print(dest, "%s\t", COMMAND_LIST[index]->command_name);
					break_count++;
					if (break_count == 3) {
						print(dest, "\n\r");
						break_count = 0;
					}
				}
				memset(pbuffer, '\0', MAX_LINE_LENGTH);
				length = 0;
				insert = 0;
				print(dest, PromptStr);
				break;
			}
			/*--------------------------------------------------------------------- invalid position*/
			if ((length != 0) && (length != insert)) {
				print(dest, "%c", CTRL('G'));
				break;
			}
			/*----------------------------------------------------------------------------- <sometxt>TAB  */
			index = 0;
			cmd_count = 0;
			index_bkup = 0;
			//while ( index < MAX_NUM_CMDS ) {
			while (index <= GEN_CMD_END_INDEX) {
				if (strncasecmp(COMMAND_LIST[index]->command_name, pbuffer,
						strlen(pbuffer)) == 0) {
					if (cmd_count < MAX_MATCH_COMMANDS) {
						match[cmd_count] = (char *)COMMAND_LIST[index]->command_name;
					}
					cmd_count++;
					if (cmd_count == 1)
						index_bkup = index;
					if (cmd_count == 2) {
						print(dest, "\n\r%s\t",
								COMMAND_LIST[index_bkup]->command_name); //to print first match
					}
					if (cmd_count >= 2) {
						if ((cmd_count >= 3) && (cmd_count % 3 == 0)) {
							print(dest, "%s\t",
									COMMAND_LIST[index]->command_name);
							print(dest, "\n\r");
						} else {
							print(dest, "%s\t",
									COMMAND_LIST[index]->command_name);
						}
					}
				}
				index++;
			}
			if (cmd_count == 0) {
				print(dest, "%c", CTRL('G'));
				break;
			}
			if (cmd_count == 1) {
				print(dest, "%s",
						COMMAND_LIST[index_bkup]->command_name
								+ strlen(pbuffer));
				memset(pbuffer, '\0', MAX_LINE_LENGTH);
				strcpy(pbuffer, COMMAND_LIST[index_bkup]->command_name);
				insert = length = strlen(pbuffer);
				break;
			}
			if (cmd_count >= 2) {
				//print(dest, "\r\nMatched\r\n");
				for (index = 0; match[index] != NULL; index++) {
					//	print(dest, "%s\r\n",match[index]);
				}
				max_match_len = get_maxmatch(cmd_count, match);
				//printf("max_match_len = %d\r\n",max_match_len);
				print(dest, PromptStr);
				memset(pbuffer, '\0', MAX_LINE_LENGTH);
				strncpy(pbuffer, match[0], max_match_len);
				insert = length = strlen(pbuffer);
				print(dest, "%s", pbuffer);
			}
			break;
		default: // ************************************ Insert a character.

			if (length == (MAX_LINE_LENGTH - 1)) {
				print(dest, "%c", CTRL('G'));
			} else if (insert == length) {
				length++;
				*(pbuffer + insert) = c;
				insert++;
				*(pbuffer + insert) = '\0';
#if 0
	        					if (login_state == PASSWORD_ASKED) {
	        						print(dest, "%c", '*');
	        					} else {
	        						print(dest, "%c", c);
	        					}
#endif
				print(dest, "%c", c); //** added after commenting out
			} else {
				for (i = length; i >= insert; i--) {
					*(pbuffer + i + 1) = *(pbuffer + i);
				}
				*(pbuffer + insert) = c;
#if 0
				if ( login_state == PASSWORD_ASKED ) {
					print(dest, "%c",'*');
				}
				else {
					print(dest, "%s\033[%dD", pbuffer + insert, length - insert);
				}
#endif

				print(dest, "%s\033[%dD", pbuffer + insert, length - insert); //** added after commenting out
				insert++;
				length++;
			}

		}
	}

}



