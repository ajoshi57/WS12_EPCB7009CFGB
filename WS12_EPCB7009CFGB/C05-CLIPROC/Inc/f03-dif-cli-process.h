/****************************************************
  FILE NAME		: f03-dif-cli-process.h
  ORIGINATOR	: A Joshi
  LAST EDITED   : 18 OCT 2025
  PROJECT       : WS12_EPCB7011
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/

#ifndef INC_F03_DIF_CLI_PROCESS_H_
#define INC_F03_DIF_CLI_PROCESS_H_

//fn protos defined here...
extern int convert_to_lower(char *command);
extern int parse_line(char *buf, char **argv);
extern int process_command(char *command, char *responsebuf);



#endif /* INC_F03_DIF_CLI_PROCESS_H_ */
