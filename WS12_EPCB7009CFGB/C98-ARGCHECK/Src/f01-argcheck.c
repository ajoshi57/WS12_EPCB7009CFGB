/****************************************************
  FILE NAME     : f01-argcheck.c
  ORIGINATOR    : A Joshi
  LAST EDITED   : 18 OCT 2025
  PROJECT       : WS12_EPCB7011
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/


#include "f01-defines.h"
#include "f01-argcheck.h"

int check_cli_chartype(char c, char *retc);
int ConvertStrtoInt16(char *Str,uint8_t lenstr, uint16_t *Value);
int ConvertStrtoPortorPortList(char *Str,uint8_t lenstr,uint8_t *IsList,uint16_t *Value1,uint16_t *Value2);


/*****************************************************************
 * Fx checks an ASCII char 'c'. It checks char catagory & returns
 * the 'CharType' as follows:
 * CHR_NULL, CHR_DIGIT,
 * CHR_ALPHA (it also converts CAPS to lower case),
 * CHR_SPACE
 *
 *Returns 0 if SUCCESS
 *Returns -1 if Illegal Char
 *Returns 'retc' returned char (modified case if applicable)
 *
 ***************************************************************/
int check_cli_chartype(char c, char *retc)
{
	int ret = 0;
	char cin;
	//
	cin=c;
	//
	if (c == 0x0)
	{
		ret = CHR_NULL;
	}
	else if (c > 47 && c < 58)
	{
		//0-9
		ret = CHR_DIGIT;
	}
	else if (c > 0x40 && c < 0x5B)
	{
		//A-Z convert to lower
		ret = CHR_ALPHA;
		cin=c+0x20;
	}
	else if (c > 0x60 && c < 0x7B)
	{
		//a-z
		ret = CHR_ALPHA;
	}
	else if (c == 0x20)
	{
		//space
		ret = CHR_SPACE;
	}
	else if(c == ':' || c == ',' || c == 0x20)
	{
		//'#' and '@' are reserved for msgpkt begin & end markers
		//'%' is format char so not used
		// '^' is used as prefix for CC msgpkts
        // '*' is used as prefix for LIF msgpkts
		//
		ret = CHR_SPL;
		cin = c;
	}
	else if(c == '!' || c == '$' || c == '&' )
	{
		ret = CHR_SPL;
	}
	else if(c == '(' || c == ')' || c == '-' || c == '_' || c == '+' || c == '=')
	{
		ret = CHR_SPL;
	}
	else if(c == '{' || c == '}' || c == '[' || c == ']' || c == '|' || c == '?')
	{
		ret = CHR_SPL;
	}
	else if(c == '/' )
	{
		ret = CHR_SPL; //used in portlist
	}
	else
	{
		ret = -1;  //Illegal !
	}
	*retc = cin;
	return ret;
	//
}


int ConvertStrtoInt16(char *Str,uint8_t lenstr, uint16_t *Value)
{
	int j,flg,v;
	char TempStr[GENERIC_MAXARGSIZE+1];
	char retc;
	//
	if(Str==NULL)
		return -1;
	//
	if(lenstr < 1 || lenstr > 5)
		return -1;
	//
	memset(TempStr,0,sizeof(TempStr));
	strncpy(TempStr,Str,lenstr);
	for(j=0;j<lenstr;j++)
	{
		flg=check_cli_chartype(TempStr[j],&retc);
		if(flg!=CHR_DIGIT)
		{
			return -1;
		}
	}
	//
	v=atoi(TempStr);
	*Value=v;
	return 0;
	//
}

int ConvertStrtoPortorPortList(char *Str,uint8_t lenstr,uint8_t *IsList,uint16_t *Value1,uint16_t *Value2)
{
	int j,k,flg;
	int found,posn;
	int v1,v2;
	char TempStr[GENERIC_MAXARGSIZE+1];
	char cstr1[10];
	char cstr2[10];
	char retc;
	//
	if(Str==NULL)
	{
		return RET_FAIL;
	}
    //
	if(lenstr < GENERIC_MINPORTLIST || lenstr>GENERIC_MAXPORTLIST)
	{
		return RET_FAIL;
	}
	memset(TempStr,0,sizeof(TempStr));
	strncpy(TempStr,Str,lenstr);
	//
	//Look for '/'
	found=0;
	posn=0;
	for(j = 0;j < lenstr;j++)
	{
		if(TempStr[j]=='/')
		{
			found=1;
			posn=j;
		}
	}
	//
	if(found==0)
	{
		//single digit
		*IsList=0;
		if(lenstr > 5)
		{
			return RET_FAIL;
		}
		for(j=0;j<lenstr;j++)
		{
			flg=check_cli_chartype(TempStr[j],&retc);
			if(flg != CHR_DIGIT)
			{
				return RET_FAIL;
			}
		}
		//
		v1=atoi(TempStr);
		*Value1=v1;
		*Value2=0;
		//
		return RET_SUCCESS;
	}
	//Come here if it is expected to be a list
	*IsList=1;
	if(posn > 5)
	{
		//first number too big !
		return RET_FAIL;
	}
	if(lenstr > (posn + 6))
	{
		//second number too big !
		return RET_FAIL;
	}
	//
	for(j=0;j<lenstr;j++)
	{
		if(j!=posn)
		{
			flg=check_cli_chartype(TempStr[j],&retc);
			if(flg!=CHR_DIGIT)
			{
				return RET_FAIL;
			}
		}
	}
	//
	for(j=0;j<posn;j++)
	{
		cstr1[j]=TempStr[j];
	}
	cstr1[posn]=0x0;
	//
	k=0;
	for(j=(posn+1);j<lenstr;j++)
	{
		cstr2[k]=TempStr[j];
		k++;
	}
	cstr2[k]=0x0;
	//
	v1=atoi(cstr1);
	v2=atoi(cstr2);
	*Value1=v1;
	*Value2=v2;
	//
	return 0;
	//
}


