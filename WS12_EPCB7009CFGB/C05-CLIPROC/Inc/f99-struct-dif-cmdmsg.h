/****************************************************
 FILE NAME		: f99-struct-dif-cmdmsg.h
  ORIGINATOR	: A Joshi
  LAST EDITED   : 18 OCT 2025
  PROJECT       : WS12_EPCB7011
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/

#ifndef INC_F99_STRUCT_DIF_CMDMSG_H_
#define INC_F99_STRUCT_DIF_CMDMSG_H_

typedef struct cmd_msg
{
	const char  *command_name;
	int arguments;
	//+++++++++++++++++++++++++++++++++++++++++++
	const char *help_str;
	int command_id;
	int auth_level; // **AJ added
	int (*command_handler)(int argc,char **argv,uint8_t cmdkwlen, char *response_buf);
} CMDMSG_T;



#endif /* INC_F99_STRUCT_DIF_CMDMSG_H_ */
