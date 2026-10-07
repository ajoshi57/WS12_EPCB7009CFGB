/****************************************************
  FILE NAME		: f04-configdbase.c
  ORIGINATOR	: A Joshi
  LAST EDITED   : 10 MAY 2026
  PROJECT       : WS12_EPCB7011CFGB
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/

#include "f99-struct-configdbase.h"
#include "f04-configdbase.h"
//
#define DEBUG 1
//
extern OAMCFGDBASE_T g_CFG_DBASE;
extern SERIALDBASE_T g_SERIAL_DBASE;

//FNs defined here............
//-----------------------------------------------------------------
void init_testdbase_defaults(TESTDBASE_T *pCfg);
void init_serialdbase_defaults(SERIALDBASE_T *pCfg);
void init_oamcfgdbase_defaults(OAMCFGDBASE_T *data);
//-----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////
void get_glitchfilterstr(uint8_t val, char *buf, uint8_t szbuf);
char *get_outputpulsestr(uint8_t val);
////////////////////////////////////////////////////////////////////////////////////
//External Global access FNs, CONFIGDBASE
void copy_oamcfgdbase_global(uint8_t *data);
void copy_oamcfgdbase_title(uint8_t *title);
void copy_oamcfgdbase_glitchfilter(uint8_t *glitchfilter);
void copy_oamcfgdbase_outputpulse(uint8_t *outputpulse);
void copy_oamcfgdbase_outputsource(uint8_t *outputsource);
void copy_oamcfgdbase_isvalid(uint8_t *isvalid);
////////////////////////////////////////////////////////////////////////////////////
//External Global access FNs, SERIALDBASE
void copy_serialdbase_cardserial(uint8_t *cardserial);
void copy_serialdbase_carddesc(uint8_t *carddesc);
void copy_serialdbase_validcardserial(uint8_t *validcardserial);
void copy_serialdbase_validcarddesc(uint8_t *validcarddesc);
void copy_serialdbase_isvalid(uint8_t *isvalid);
////////////////////////////////////////////////////////////////////////////////////
//

//
//-------------------------------------------
//FNs defined here............

void init_testdbase_defaults(TESTDBASE_T *pCfg)
{
    TESTDBASE_T *testdbase;
    uint8_t j,k, offset;
    //
    testdbase = (TESTDBASE_T *)pCfg;
    memset(testdbase,0,sizeof(TESTDBASE_T));
    //
    ////////////////////////////////////////////////////////////////
    //'0' - '9'
    for(j = 0;j < 10;j++)
    {
        testdbase->bytes[j] = 0x30 + j;
    }
    //'A' - 'F'
    for(j = 10;j < 16;j++)
    {
        testdbase->bytes[j] = 0x41 + j;
    }
    ////////////////////////////////////////////////////////////////
    //////
}

void init_serialdbase_defaults(SERIALDBASE_T *pCfg)
{
    SERIALDBASE_T *serialdbase;
    //
    serialdbase = (SERIALDBASE_T *)pCfg;
    memset(serialdbase,0,sizeof(SERIALDBASE_T));
    //
    serialdbase->isvalid = VALIDDBASE;
    ////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////
    //////
}


void init_oamcfgdbase_defaults(OAMCFGDBASE_T *data)
{
	OAMCFGDBASE_T *oamcfgdbase;
	uint8_t j,k, offset;
	//
	oamcfgdbase = (OAMCFGDBASE_T *)data;
	memset(oamcfgdbase,0,sizeof(OAMCFGDBASE_T));
	//
	////////////////////////////////////////////////////////////////
	sprintf((char *)oamcfgdbase->title, "TEST-FPGA-CFGB");
	////////////////////////////////////////////////////////////////
	//DEFAULT SETTINGS FOR GLITCH FILTER AND OUTPUT PULSE
	for(j = 0;j < MAX_INCHANS;j++)
	{
		oamcfgdbase->glitchfilter[j] = 8; //5MS
	}
    ////////////////////////////////////////////////////////////////
	for(j = 0;j < MAX_OUTCHANS;j++)
	{
		oamcfgdbase->outputpulse[j] = SET_PULSE200MS; //200MS
	}
	////////////////////////////////////////////////////////////////
    for(k = 0;k < 4;k++)
    {
        if(k == 0)
        {
            offset = 0;
        }
        else if(k == 1)
        {
            offset = 4;
        }
        else if(k == 2)
        {
            offset = 8;
        }
        else //if(k == 3)
        {
            offset = 12;
        }
        for(j = 0;j < 4;j++)
        {
            oamcfgdbase->outputsource[j + offset] = j + 1;
            debug_msg("output [%d] source [%d]\r\n", j + offset + 1, j + 1);
        }
    }
    ////////////////////////////////////////////////////////////////
	//////
    oamcfgdbase->isvalid = VALIDDBASE;
	//////
}

void get_glitchfilterstr(uint8_t val, char *buf, uint8_t szbuf)
{
	//0 => 0.5ms
    //1 => 1.0ms
	//1-20 = 1ms to 20ms
	memset(buf,0,szbuf);
	switch(val)
	{
	    case 0:
        sprintf(buf,"0.5ms");
        break;
        case 1:
        sprintf(buf,"1.0ms");
        break;
        case 2:
        sprintf(buf,"1.5ms");
        break;
        case 3:
        sprintf(buf,"2.0ms");
        break;
        case 4:
        sprintf(buf,"2.5ms");
        break;
        case 5:
        sprintf(buf,"3.0ms");
        break;
        case 6:
        sprintf(buf,"3.5ms");
        break;
        case 7:
        sprintf(buf,"4.0ms");
        break;
        case 8:
        sprintf(buf,"5.0ms");
        break;
        default:
        sprintf(buf,"??");
        break;
	}
	/////////////////
	return;
	/////////////////
}

char *get_outputpulsestr(uint8_t val)
{
	switch(val)
	{
		case 0:
		return "5ms";
		case 1:
		return "10ms";
		case 2:
		return "20ms";
		case 3:
		return "50ms";
		case 4:
		return "100ms";
		case 5:
		return "150ms";
		case 6:
		return "200ms";
		case 7:
		return "250ms";
		case 8:
		return "300ms";
		case 9:
		return "350ms";
		case 10:
		return "400ms";
		case 11:
		return "450ms";
		case 12:
		return "500ms";
		case 13:
		return "600ms";
		case 14:
		return "750ms";
		case 15:
		return "1000ms";
		default:
		return "??";
	}
}

void copy_oamcfgdbase_global(uint8_t *data)
{
    memset(data,0,sizeof(OAMCFGDBASE_T));
    memcpy(data,(uint8_t *)&g_CFG_DBASE,sizeof(OAMCFGDBASE_T));
}


//External Global access FNs
void copy_oamcfgdbase_title(uint8_t *title)
{
    memset(title,0,MAX_TITLE_SIZE + 1);
    memcpy(title,(uint8_t *)&g_CFG_DBASE.title[0],MAX_TITLE_SIZE);
}

void copy_oamcfgdbase_glitchfilter(uint8_t *glitchfilter)
{
    memset(glitchfilter,0,MAX_INCHANS);
    memcpy(glitchfilter,(uint8_t *)&g_CFG_DBASE.glitchfilter[0],MAX_INCHANS);
}

void copy_oamcfgdbase_outputpulse(uint8_t *outputpulse)
{
    memset(outputpulse,0,MAX_OUTCHANS);
    memcpy(outputpulse,(uint8_t *)&g_CFG_DBASE.outputpulse[0],MAX_OUTCHANS);
}

void copy_oamcfgdbase_outputsource(uint8_t *outputsource)
{
    memset(outputsource,0,MAX_OUTCHANS);
    memcpy(outputsource,(uint8_t *)&g_CFG_DBASE.outputsource[0],MAX_OUTCHANS);
}

void copy_oamcfgdbase_isvalid(uint8_t *isvalid)
{
    memset(isvalid,0,4);
    memcpy(isvalid,(uint8_t *)&g_CFG_DBASE.isvalid,4);
}


////////////////////////////////////////////////////////////////////////////////////
//External Global access FNs, SERIALDBASE
void copy_serialdbase_cardserial(uint8_t *cardserial)
{
    memset(cardserial,0,24);
    memcpy(cardserial,(uint8_t *)&g_SERIAL_DBASE.cardserial[0],MAX_SERIAL_SIZE);
}

void copy_serialdbase_carddesc(uint8_t *carddesc)
{
    memset(carddesc,0,24);
    memcpy(carddesc,(uint8_t *)&g_SERIAL_DBASE.carddesc[0],MAX_SERIAL_SIZE);
}

void copy_serialdbase_validcardserial(uint8_t *validcardserial)
{
    memset(validcardserial,0,4);
    memcpy(validcardserial,(uint8_t *)&g_SERIAL_DBASE.validcardserial,4);
}

void copy_serialdbase_validcarddesc(uint8_t *validcarddesc)
{
    memset(validcarddesc,0,4);
    memcpy(validcarddesc,(uint8_t *)&g_SERIAL_DBASE.validcarddesc,4);
}


void copy_serialdbase_isvalid(uint8_t *isvalid)
{
    memset(isvalid,0,4);
    memcpy(isvalid,(uint8_t *)&g_SERIAL_DBASE.isvalid,4);
}


