/*
 * f02-dbase-access.c
 *
 *  Created on: 21-Apr-2026
 *      Author: Arvind Joshi
 */

#include "f02-dbase-access.h"

////////////////////////////////////////////////////////////////
TESTDBASE_T   g_TESTREF;
TESTDBASE_T   g_TEST_DBASE;
SERIALDBASE_T g_SERIAL_DBASE;
OAMCFGDBASE_T g_CFG_DBASE;

//
////////////////////////////////////////////////////////////////
#define EEPROM_TESTSTART_WORD 0
#define EEPROM_TESTSTART_ADDR (EEPROM_TESTSTART_WORD * 4)
////////////////////////////////////////////////////////////////
#define EEPROM_SERIALSTART_WORD 4
#define EEPROM_SERIALSTART_ADDR (EEPROM_SERIALSTART_WORD * 4)
////////////////////////////////////////////////////////////////
#define EEPROM_CFGSTART_WORD    19  //4 + 15                           // Starting word offset
#define EEPROM_CFGSTART_ADDR   (EEPROM_CFGSTART_WORD * 4)              // Byte address
/////////////////////////////////////////////////////////////////////
//FNs protos defined here...
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//TEST Access
int EEPROM_TEST_clear(void);
int EEPROM_TEST_default(void);
int EEPROM_WriteTEST(void);
int EEPROM_ReadTEST(void);
void init_test_ref(void);
int check_test_defaults(int *failstep);

//=====================================================================
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//Config Access
int EEPROM_OAMConfig_clear(void);
int EEPROM_OAMConfig_default(void);
int EEPROM_OAMConfig_title(uint8_t *title, uint8_t len);
int EEPROM_OAMConfig_glitchfilter_factor(uint8_t ch, uint8_t factor);
int EEPROM_OAMConfig_outputpulse(uint8_t ch, uint8_t outputpulse);
int EEPROM_OAMConfig_outputsource(uint8_t ch, uint8_t outputsource);
int EEPROM_WriteOAMConfig(void);
int EEPROM_ReadOAMConfig(void);
//=====================================================================
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//FACTORY Access
int EEPROM_Serial_clear(void);
int EEPROM_Serial_default(void);
int EEPROM_Serial_serno(uint8_t *serno, uint8_t len);
int EEPROM_Serial_desc(uint8_t *desc, uint8_t len);
int EEPROM_WriteSerial(void);
int EEPROM_ReadSerial(void);
//=====================================================================
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int boot_testdbase_eeprom(int *result, uint8_t *failstep);
int boot_serialdbase_eeprom(int *result, uint8_t *failstep);
int boot_oamcfgdbase_eeprom(int *result, uint8_t *failstep);

//
//
static int validate_serialdbase(int *failstep);
static int validate_oamcfgdbase(int *failstep);
static int validate_glitchfilter(uint8_t factor);
static int validate_outputpulse(uint8_t outputpulse);
static int validate_outputsource(uint8_t outputsource);

/////////////////////////////////////////////////////////////////////
//FNs defined here...

int EEPROM_TEST_clear(void)
{
    int ret;
    memset(&g_TEST_DBASE, 0x00, sizeof(g_TEST_DBASE));

    /* 'isvalid' 0 after memset – not 0xA5A5,
     * so dbase is treated as invalid / empty. */
    ret = EEPROM_WriteTEST();
    return ret;
}

int EEPROM_TEST_default(void)
{
    int ret;
    //
    TESTDBASE_T *pCfg;
    pCfg = (TESTDBASE_T *)&g_TEST_DBASE;
    if (pCfg == NULL)
    {
        goto endx;
    }
    init_testdbase_defaults((TESTDBASE_T *)pCfg);
    /* isvalid is set to 0xA5A5 */
    ret = EEPROM_WriteTEST();
    endx:
    return ret;
}


int EEPROM_WriteTEST(void)
{
    int ret = -1;
    uint32_t ui32Result = 0;
    TESTDBASE_T *pCfg;
    pCfg = (TESTDBASE_T *)&g_TEST_DBASE;
    if (pCfg == NULL)
    {
        goto endx;
    }
    ui32Result = EEPROMProgram(
        (uint32_t *)pCfg,       // Source buffer (must be word-aligned)
        EEPROM_TESTSTART_ADDR,   // Destination EEPROM byte address (4 * word offset)
        sizeof(TESTDBASE_T)   // Number of bytes to write (64)
    );
    /////////////////////////////
    if(ui32Result == 0)
    {
        ret = 0;
    }
    /////////////////////////////
    endx:
    return ret;  // EEPROMProgram returns 0 on success
}

int EEPROM_ReadTEST(void)
{
    int ret = -1;
    TESTDBASE_T *pCfg;
    pCfg = (TESTDBASE_T *)&g_TEST_DBASE;
    if (pCfg == NULL)
    {
        goto endx;
    }

    EEPROMRead(
        (uint32_t *)pCfg,       // Destination buffer (must be word-aligned)
        EEPROM_TESTSTART_ADDR,   // Source EEPROM byte address (4 * word offset)
        sizeof(TESTDBASE_T)   // Number of bytes to read (64)
    );
    endx:
    ret = 0;
    return ret;
}

void init_test_ref(void)
{
    TESTDBASE_T *testdbase;
    uint8_t j;
    //
    testdbase = (TESTDBASE_T *)&g_TESTREF;
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

int check_test_defaults(int *failstep)
{
    int ret = 0;
    int j;
    int fstep = 0;
    //
    TESTDBASE_T *ref;
    ref = (TESTDBASE_T *)&g_TESTREF;
    if (ref == NULL)
    {
        ret = -1;
        fstep = 89;
        goto endx;
    }
    TESTDBASE_T *pCfg;
    pCfg = (TESTDBASE_T *)&g_TEST_DBASE;
    if (pCfg == NULL)
    {
        ret = -1;
        fstep = 99;
        goto endx;
    }
   //////////////////////////////////////////////////////////
    for(j = 0;j < 16;j++)
    {
        if(pCfg->bytes[j] != ref->bytes[j])
        {
            ret = -1;
            if(fstep == 0)
            {
                fstep = j + 1;
            }
        }
    }
    //////////////////////////////////////////////////////////
    /////////////////
    endx:
    *failstep = fstep;
    return ret;
    /////////////////
}


/**
 * @brief  Zero the global structure and persist the cleared state to EEPROM.
 *
 * After this call the validity word is 0x00000000 (not 0xA5A5), which
 * marks field as empty.
 */
int EEPROM_OAMConfig_clear(void)
{
    int ret;
    memset(&g_CFG_DBASE, 0x00, sizeof(g_CFG_DBASE));

    /* 'isvalid' 0 after memset – not 0xA5A5,
     * so dbase is treated as invalid / empty. */
    ret = EEPROM_WriteOAMConfig();
    return ret;
}

/**
 * @brief  DEFAULT Settings in GLOBAL as well as EEPROM .
 *
 * After this call the validity word is 0xA5A5, which marks field as VALID.
 */
int EEPROM_OAMConfig_default(void)
{
    int ret;
    //
    OAMCFGDBASE_T *pCfg;
    pCfg = (OAMCFGDBASE_T *)&g_CFG_DBASE;
    if (pCfg == NULL)
    {
        ret = -1;
        goto endx;
    }
    init_oamcfgdbase_defaults((OAMCFGDBASE_T *)pCfg);
    /* isvalid is set to 0xA5A5 */
    ret = EEPROM_WriteOAMConfig();
    endx:
    return ret;
}

/**
 * @brief  TITLE Setting in GLOBAL as well as EEPROM .
 *
 *
 */
int EEPROM_OAMConfig_title(uint8_t *title, uint8_t len)
{
    int ret = -1;
    //
    OAMCFGDBASE_T *pCfg;

    pCfg = (OAMCFGDBASE_T *)&g_CFG_DBASE;
    if (pCfg == NULL)
    {
        goto endx;
    }
    memcpy((void *)pCfg->title, (void *)title,len);
    /* Write Setting */
    ret = EEPROM_WriteOAMConfig();
    endx:
    return ret;
}

/**
 * @brief  GLITCH FILTER Factor Setting in GLOBAL as well as EEPROM .
 *
 *
 */
int EEPROM_OAMConfig_glitchfilter_factor(uint8_t ch, uint8_t factor)
{
    int ret = -1;
    int flg;
    int j;
    //
    OAMCFGDBASE_T *pCfg;
    //
    if(ch < MIN_INCHANS)
    {
        debug_msg("Illegal Input Chan No !\r\n");
        goto endx;
    }
    if(ch > MAX_INCHANS && ch != 99)
    {
        debug_msg("Illegal Input Chan No !\r\n");
        goto endx;
    }
    flg = validate_glitchfilter(factor);
    if(flg != 0)
    {
        debug_msg("Illegal Glitch Filter factor !\r\n");
        ret = -1;
        goto endx;
    }
    //
    pCfg = (OAMCFGDBASE_T *)&g_CFG_DBASE;
    if (pCfg == NULL)
    {
        goto endx;
    }
    if(ch == 99)
    {
        for(j = 0;j < MAX_INCHANS;j++)
        {
            pCfg->glitchfilter[j] = factor;
        }
    }
    else
    {
        pCfg->glitchfilter[ch - 1] = factor;
    }
    /* Write Setting */
    flg = EEPROM_WriteOAMConfig();
    if(flg == 0)
    {
        ret = 0;
    }
    endx:
    return ret;
}

/**
 * @brief  OUTPUT PULSE Setting in GLOBAL as well as EEPROM .
 *
 *
 */
int EEPROM_OAMConfig_outputpulse(uint8_t ch, uint8_t outputpulse)
{
    int ret = -1;
    int flg;
    int j;
    //
    OAMCFGDBASE_T *pCfg;
    //
    if(ch < MIN_OUTCHANS)
    {
        debug_msg("Illegal Output Chan No !\r\n");
        ret = -1;
        goto endx;
    }
    if(ch > MAX_OUTCHANS && ch != 99)
    {
        debug_msg("Illegal Output Chan No !\r\n");
        ret = -1;
        goto endx;
    }
    flg = validate_outputpulse(outputpulse);
    if(flg != 0)
    {
        debug_msg("Illegal OutputPulse !\r\n");
        ret = -1;
        goto endx;

    }
    pCfg = (OAMCFGDBASE_T *)&g_CFG_DBASE;
    if (pCfg == NULL)
    {
        ret = -1;
        debug_msg("Illegal DBASE !\r\n");
        goto endx;
    }
    if(ch == 99)
    {
        for(j = 0;j < MAX_OUTCHANS;j++)
        {
            pCfg->outputpulse[j] = outputpulse;
        }
    }
    else
    {
        pCfg->outputpulse[ch - 1] = outputpulse;
    }
    /* Write Setting */
    flg = EEPROM_WriteOAMConfig();
    if(flg != 0)
    {
        ret = -1;
        debug_msg("Illegal Write EEPROM !\r\n");
        goto endx;
    }
    ////////////////////////
    ret = 0;
    ////////////////////////
    endx:
    return ret;
}

int EEPROM_OAMConfig_outputsource(uint8_t ch, uint8_t outputsource)
{
    int ret = -1;
    int flg;
    int j;
    //
    OAMCFGDBASE_T *pCfg;
    //
    if(ch < MIN_OUTCHANS)
    {
        debug_msg("Illegal Output Chan No !\r\n");
        ret = -1;
        goto endx;
    }
    if(ch > MAX_OUTCHANS && ch != 99)
    {
        debug_msg("Illegal Output Chan No !\r\n");
        ret = -1;
        goto endx;
    }
    flg = validate_outputsource(outputsource);
    if(flg != 0)
    {
        debug_msg("Illegal OutputSource !\r\n");
        ret = -1;
        goto endx;

    }
    pCfg = (OAMCFGDBASE_T *)&g_CFG_DBASE;
    if (pCfg == NULL)
    {
        ret = -1;
        debug_msg("Illegal DBASE !\r\n");
        goto endx;
    }
    if(ch == 99)
    {
        for(j = 0;j < MAX_OUTCHANS;j++)
        {
            pCfg->outputsource[j] = outputsource;
        }
    }
    else
    {
        pCfg->outputsource[ch - 1] = outputsource;
    }
    /* Write Setting */
    flg = EEPROM_WriteOAMConfig();
    if(flg != 0)
    {
        ret = -1;
        debug_msg("Illegal Write EEPROM !\r\n");
        goto endx;
    }
    ////////////////////////
    ret = 0;
    ////////////////////////
    endx:
    return ret;
}


/**
 * @brief  Write OAMCFGDBASE_T structure to EEPROM starting at byte EEPROM_CFGSTART_ADDR.
 * @param  pCfg  Pointer to the structure to write.
 * @return true on success, false on EEPROM error.
 */
int EEPROM_WriteOAMConfig(void)
{
    int ret = -1;
    uint32_t ui32Result = 0;
    OAMCFGDBASE_T *pCfg;
    pCfg = (OAMCFGDBASE_T *)&g_CFG_DBASE;
    if (pCfg == NULL)
    {
        goto endx;
    }
    ui32Result = EEPROMProgram(
        (uint32_t *)pCfg,       // Source buffer (must be word-aligned)
        EEPROM_CFGSTART_ADDR,   // Destination EEPROM byte address (4 * word offset)
        sizeof(OAMCFGDBASE_T)   // Number of bytes to write (64)
    );
    /////////////////////////////
    if(ui32Result == 0)
    {
        ret = 0;
    }
    /////////////////////////////
    endx:
    return ret;  // EEPROMProgram returns 0 on success
}


/**
 * @brief  Read OAMCFGDBASE_T structure from EEPROM starting at word 'EEPROM_CFGSTART_ADDR'.
 * @param  pCfg  Pointer to the structure to populate.
 * @return true on success, false on invalid pointer.
 */
int EEPROM_ReadOAMConfig(void)
{
    int ret = -1;
    OAMCFGDBASE_T *pCfg;
    pCfg = (OAMCFGDBASE_T *)&g_CFG_DBASE;
    if (pCfg == NULL)
    {
        goto endx;
    }

    EEPROMRead(
        (uint32_t *)pCfg,       // Destination buffer (must be word-aligned)
        EEPROM_CFGSTART_ADDR,   // Source EEPROM byte address (4 * word offset)
        sizeof(OAMCFGDBASE_T)   // Number of bytes to read (64)
    );
    endx:
    ret = 0;
    return ret;
}



#if 0
/**
 * @brief  Read the SERIALDBASE block from EEPROM into g_serialdbase.
 */
bool EEPROM_SerialDBase_Read(void)
{
    EEPROMRead((uint32_t *)&g_serialdbase,
               EPSERIAL_DBASE_BYTE_ADDR,
               EPSERIAL_DBASE_SIZE_BYTES);

    /* EEPROMRead() returns void in some TivaWare versions.
     * A simple sanity check on the sentinel values is a reasonable
     * post-read validation strategy. */
    return true;
}
#endif


/**
 * @brief  Zero the global structure and persist the cleared state to EEPROM.
 *
 * After this call the validity word is 0x00000000 (not 0xA5A5), which
 * marks field as empty.
 */
int EEPROM_Serial_clear(void)
{
    int ret;
    memset(&g_SERIAL_DBASE, 0x00, sizeof(g_SERIAL_DBASE));

    /* validcardserial and validcarddesc are 0 after memset – not 0xA5A5,
     * so both fields are treated as invalid / empty. */
    ret = EEPROM_WriteSerial();
    return ret;
}

/**
 * @brief  DEFAULT Settings in GLOBAL as well as EEPROM .
 *
 * After this call the validity word is 0xA5A5, which marks field as VALID.
 */
int EEPROM_Serial_default(void)
{
    int ret = -1;
    //
    SERIALDBASE_T *pCfg;
    pCfg = (SERIALDBASE_T *)&g_SERIAL_DBASE;
    if (pCfg == NULL)
    {
        goto endx;
    }
    init_serialdbase_defaults((SERIALDBASE_T *)pCfg);
    /* isvalid is set to 0xA5A5 */
    ret = EEPROM_WriteSerial();
    endx:
    return ret;
}

int EEPROM_Serial_serno(uint8_t *serno, uint8_t len)
{
    int ret = -1;
    //
    SERIALDBASE_T *pCfg;

    pCfg = (SERIALDBASE_T *)&g_SERIAL_DBASE;
    if (pCfg == NULL)
    {
        goto endx;
    }
    memcpy((void *)pCfg->cardserial, (void *)serno,len);
    pCfg->validcardserial = VALIDSERNO;
    pCfg->isvalid = VALIDDBASE;
    /* Write Setting */
    ret = EEPROM_WriteSerial();
    endx:
    return ret;
}

int EEPROM_Serial_desc(uint8_t *desc, uint8_t len)
{
    int ret = -1;
    //
    SERIALDBASE_T *pCfg;

    pCfg = (SERIALDBASE_T *)&g_SERIAL_DBASE;
    if (pCfg == NULL)
    {
        goto endx;
    }
    memcpy((void *)pCfg->carddesc, (void *)desc,len);
    pCfg->validcarddesc = VALIDDESC;
    pCfg->isvalid = VALIDDBASE;
    /* Write Setting */
    ret = EEPROM_WriteSerial();
    endx:
    return ret;
}

/**
 * @brief  Write SERIALDBASE_T structure to EEPROM starting at byte EEPROM_SERIALSTART_ADDR.
 * @param  pCfg  Pointer to the structure to write.
 * @return true on success, false on EEPROM error.
 */
int EEPROM_WriteSerial(void)
{
    int ret = -1;
    uint32_t ui32Result = 0;
    SERIALDBASE_T *pCfg;
    pCfg = (SERIALDBASE_T *)&g_SERIAL_DBASE;
    if (pCfg == NULL)
    {
        goto endx;
    }
    ui32Result = EEPROMProgram(
        (uint32_t *)pCfg,       // Source buffer (must be word-aligned)
        EEPROM_SERIALSTART_ADDR,   // Destination EEPROM byte address (4 * word offset)
        sizeof(SERIALDBASE_T)   // Number of bytes to write
    );
    /////////////////////////////
    if(ui32Result == 0)
    {
        ret = 0;
    }
    /////////////////////////////
    endx:
    return ret;  // EEPROMProgram returns 0 on success
}


/**
 * @brief  Read SERIALDBASE_T structure from EEPROM starting at word 'EEPROM_SERIALSTART_ADDR'.
 * @param  pCfg  Pointer to the structure to populate.
 * @return true on success, false on invalid pointer.
 */
int EEPROM_ReadSerial(void)
{
    int ret = -1;
    SERIALDBASE_T *pCfg;
    pCfg = (SERIALDBASE_T *)&g_SERIAL_DBASE;
    if (pCfg == NULL)
    {
        goto endx;
    }
    EEPROMRead(
        (uint32_t *)pCfg,       // Destination buffer (must be word-aligned)
        EEPROM_SERIALSTART_ADDR,   // Source EEPROM byte address (4 * word offset)
        sizeof(SERIALDBASE_T)   // Number of bytes to read
    );
    endx:
    ret = 0;
    return ret;
}

int boot_testdbase_eeprom(int *result, uint8_t *failstep)
{
    int flg;
    int ret = -1;
    int fstep;
    //
    flg = EEPROM_ReadTEST();
    if(flg != 0)
    {
        goto endx;
    }
    ///////////////////////////////////////////////
    ret = 0;
    ///////////////////////////////////////////////
    flg = check_test_defaults(&fstep);
    if(flg != 0)
    {
        *result = -1;
        *failstep = fstep;
    }
    else
    {
        *result = 0;
    }
    ///////////////////////////////////////////////
    //
    endx:
    return ret;
}

int boot_serialdbase_eeprom(int *result, uint8_t *failstep)
{
    int flg;
    int ret = -1;
    int fstep;
    //
    flg = EEPROM_ReadSerial();
    if(flg != 0)
    {
        goto endx;
    }
    ///////////////////////////////////////////////
    ret = 0;
    ///////////////////////////////////////////////
    flg = validate_serialdbase(&fstep);
    if(flg != 0)
    {
        *result = -1;
        *failstep = fstep;
    }
    else
    {
        *result = 0;
    }
    ///////////////////////////////////////////////
    //
    endx:
    return ret;
}

int boot_oamcfgdbase_eeprom(int *result, uint8_t *failstep)
{
    int flg;
    int ret = -1;
    int fstep;
    //
    flg = EEPROM_ReadOAMConfig();
    if(flg != 0)
    {
        goto endx;
    }
    ///////////////////////////////////////////////
    ret = 0;
    ///////////////////////////////////////////////
    flg = validate_oamcfgdbase(&fstep);
    if(flg != 0)
    {
        *result = -1;
        *failstep = fstep;
    }
    else
    {
        *result = 0;
    }
    ///////////////////////////////////////////////
    //
    endx:
    return ret;
}


static int validate_serialdbase(int *failstep)
{
    {
        int ret = 0;
        int fstep = 0;
        //
        SERIALDBASE_T *pCfg;
        pCfg = (SERIALDBASE_T *)&g_SERIAL_DBASE;
        if (pCfg == NULL)
        {
            ret = -1;
            fstep = 1;
            goto endx;
        }
        //////////////////////////////////////////////////////////
        if(pCfg->isvalid != VALIDDBASE)
        {
            ret = -1;
            fstep = 2;
            goto endx;
        }
        //////////////////////////////////////////////////////////
        /////////////////
        endx:
        *failstep = fstep;
        return ret;
        /////////////////
    }

}

static int validate_oamcfgdbase(int *failstep)
{
    int ret = 0;
    int fstep = 0;
    int j;
    //
    OAMCFGDBASE_T *pCfg;
    pCfg = (OAMCFGDBASE_T *)&g_CFG_DBASE;
    if (pCfg == NULL)
    {
        ret = -1;
        fstep = 1;
        goto endx;
    }
    //////////////////////////////////////////////////////////
    if(pCfg->isvalid != VALIDDBASE)
    {
        ret = -1;
        fstep = 2;
        goto endx;
    }
    //////////////////////////////////////////////////////////
    for(j=0;j<MAX_INCHANS;j++)
    {
        if(validate_glitchfilter(pCfg->glitchfilter[j]) != 0)
        {
            ret = -1;
            fstep = 3 + j;
            goto endx;
        }
    }
   //////////////////////////////////////////////////////////
    for(j=0;j<MAX_OUTCHANS;j++)
    {
        if(validate_outputpulse(pCfg->outputpulse[j]) != 0)
        {
            ret = -1;
            fstep = 3 + MAX_INCHANS + j;
            goto endx;
        }
    }
    //////////////////////////////////////////////////////////
    for(j=0;j<MAX_OUTCHANS;j++)
    {
        if(validate_outputsource(pCfg->outputsource[j]) != 0)
        {
            ret = -1;
            fstep = 3 + MAX_INCHANS + MAX_OUTCHANS + j;
            goto endx;
        }
    }
    //////////////////////////////////////////////////////////
    /////////////////
    endx:
    *failstep = fstep;
    return ret;
    /////////////////

}

static int validate_glitchfilter(uint8_t factor)
{
    int ret = 0;
    if(factor > 8)
    {
        ret = -1;
    }
    return ret;
}


static int validate_outputpulse(uint8_t outputpulse)
{
    int ret = 0;
    if(outputpulse > 15)
    {
        ret = -1;
    }
    return ret;
}

static int validate_outputsource(uint8_t outputsource)
{
    int ret = 0;
    if(outputsource < MIN_INCHANS ||  outputsource > MAX_INCHANS)
    {
        ret = -1;
    }
    return ret;
}



