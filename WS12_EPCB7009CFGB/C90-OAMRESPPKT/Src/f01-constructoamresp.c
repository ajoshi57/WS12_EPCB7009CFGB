/****************************************************
  FILE NAME		: f01-constructoamresp.c
  ORIGINATOR	: A Joshi
  LAST EDITED   : 18 OCT 2025
  PROJECT       : WS12_EPCB7011
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/


//
#include "f01-constructoamresp.h"
//
//
int construct_oam_responsepkt(uint8_t *msg,VALIDRESP_T *validresp)
{
	int ret = RET_SUCCESS;
	uint16_t msglen;
	char buf[20];
	//
	msglen = strlen((char *)msg);
	memset(validresp,0,sizeof(VALIDRESP_T));
	//
	if(msglen < MIN_RESPSTRING_SIZE || msglen > MAX_RESPSTRING_SIZE)
	{
		debug_msg("ERROR-SIZE !\r\n");
		ret = RET_FAIL;
		memcpy(validresp->resppkt,"$$[0014]ERROR-SIZE !\r\n##",24);
		validresp->respstartidx = 8;
		validresp->respendidx = 24 + 7;
		goto end;
	}
	//////////////////
	//
	sprintf(buf,"$$[%04d]",msglen);
	memcpy(validresp->resppkt,buf,8);
	strncat((char *)validresp->resppkt,(char *)msg,msglen);
	strcat((char *)validresp->resppkt,"##");
	validresp->totallen = msglen + 10;
	validresp->respstartidx = 8;
	validresp->respendidx = msglen + 7;
	//
    ////////////////////
	end:
	return ret;
	////////////////////
	//
}

void construct_oam_large_template_responsepkt(uint8_t *largebuf, uint16_t sz)
{
	uint16_t msglen = 100;
	char buf[20];
	//
	memset(largebuf,0,sz);
	sprintf(buf,"$$[%04d]",msglen); //this will be modified later!
	memcpy(largebuf,buf,8);         //header pasted !
	//
}


