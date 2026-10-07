/****************************************************
  FILE NAME		: f90-handler-utils.c
   ORIGINATOR	: A Joshi
  LAST EDITED   : 18 OCT 2025
  PROJECT       : WS12_EPCB7011
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/

#include "f90-handler-utils.h"

#define DEBUG 1
//
//fns defined here...
int add_spaces_tostr(char *source,char *dest,int finalsize);

int add_spaces_tostr(char *source,char *dest,int finalsize)
{
	int ret = 0;
	int len,add,j;
	char buf[MAX_TABLEENTRY_BUF]={'\0'};
	char sp[MAX_TABLEENTRY_BUF]={'\0'};
	//
	if(source==NULL)
	{
		ret = -1;
		goto end;
	}
	if(dest==NULL)
	{
		ret = -1;
		goto end;
	}
	if(finalsize > (MAX_TABLEENTRY_BUF - 1))
	{
		ret = -1;
		goto end;
	}
	len=strlen(source);
	if(len < finalsize)
		add = finalsize - len;
	else
	{
		ret = -1;
		goto end;
	}
	for(j=0;j<add;j++)
	{
		sp[j]=' ';
	}
	strncpy(buf,source,MAX_TABLEENTRY_BUF);
	//
	strncat(buf,sp,add);
	strncpy(dest,buf,MAX_TABLEENTRY_BUF);
	//
	if(ret != 0)
	{
		debug_msg("ERROR ! 'add_spaces_tostr'\r\n");
	}
	//////////////
	end:
	return ret;
	//////////////

}

