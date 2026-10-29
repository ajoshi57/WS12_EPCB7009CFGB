/****************************************************
  FILE NAME     : f04-logicdefcfg.c
  ORIGINATOR    : A Joshi
  LAST EDITED   : 29 OCT 2026
  PROJECT       : WS12_EPCB7009
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/
//
#define DEBUG 1
//

#include "f04-logicdefcfg.h"
//
static LOGICCFG_T g_LOGIC_DEFAULTCFG;
LOGICCFG_T g_LOGIC_WRCFG;
LOGICCFG_T g_LOGIC_RDCFG;
//
extern OAMCFGDBASE_T g_CFG_DBASE;
//

//fn protos defined here...
int read_logiccfg(LOGICCFG_T *logiccfg, int *testfailstep, int debugon);
static int validate_reggf(uint8_t rdata);
static int validate_regpw(uint8_t rdata);
static int validate_regmap(uint8_t rdata);
static int validate_enabalarm(uint8_t rdata);
static int construct_logiccfg(OAMCFGDBASE_T *cfgdbase,LOGICCFG_T *logiccfg, int *testfailstep);
static int write_logiccfg(LOGICCFG_T *logiccfg, int *testfailstep);
static int compare_logiccfg(LOGICCFG_T *wrlogiccfg, LOGICCFG_T *rdlogiccfg, int *testfailstep);
//
int boot_logicconfig(int *failstage, int *testfailstep);
//
int program_gf(uint8_t channo, uint8_t gfvalue, int debugon);
int program_pw(uint8_t channo, uint8_t pwvalue, int debugon);
int program_map(uint8_t channo, uint8_t mapvalue, int debugon);
//
static int reevaluate_enabalarm(OAMCFGDBASE_T *cfgdbase, uint8_t *newenabalarm);
//
int clear_alarmrelay(void);
int statemcpw_start(int debugon);
int statemcgf_start(int debugon);

//fn protos defined here...
/*****************************************************************************
** Function name:       read_logiccfg
**
** Descriptions:        Read Logic CFG Registers
**
** Parameters:          NONE
**
** Returned value:      0 success, -1 fail
**
*****************************************************************************/
int read_logiccfg(LOGICCFG_T *logiccfg, int *testfailstep, int debugon)
{
    int ret = -1;
    int flg,j;
    uint32_t fstep;
    //uint8_t page = 0;
    uint8_t addr, rdata;
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    logiccfg->valid = 0;
    fstep = 0; //starting
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    vTaskDelay(200);
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //Read Cycle Glitch Filter Regs 1-4 #31-#34
    for(j = 0;j < MAX_INCHANS;j++)
    {
        fstep++;
        addr = 31 + j;
        flg = twi1logic_readcycle(0, addr, &rdata);
        if(flg != 0)
        {
            debug_msg("ERROR, 'logic_read_cycle' !\r\n");
            goto endx;
        }
        else
        {
            //Validate GF
            if(validate_reggf(rdata & 0x0F) == 0)
            {
                logiccfg->reggf[j] = rdata & 0x0F;
                if(debugon != 0)
                    debug_msg("SUCCESS READ Valid GF[%d] Addr %02X Data %02X\r\n", j + 1, addr, (rdata & 0x0F) );
            }
            else
            {
                debug_msg("ERROR, Illegal GF[%d] val %d !\r\n", j + 1, (rdata & 0x0F) );
                goto endx;
            }
        }
        vTaskDelay(100);
    }
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //Read Cycle Pulse Width Regs 1-16 #11-#26
    for(j = 0;j < MAX_OUTCHANS;j++)
    {
        fstep++;
        addr = 11 + j;
        flg = twi1logic_readcycle(0, addr, &rdata);
        if(flg != 0)
        {
            debug_msg("ERROR, 'logic_read_cycle' !\r\n");
            goto endx;
        }
        else
        {
            //Validate PW
            if(validate_regpw(rdata & 0x0F) == 0)
            {
                logiccfg->regpw[j] = rdata & 0x0F;
                if(debugon != 0)
                    debug_msg("SUCCESS READ Valid PW[%d] Addr %02X Data %02X\r\n", j + 1, addr, (rdata & 0x0F) );
            }
            else
            {
                debug_msg("ERROR, Illegal PW[%d] val %d !\r\n", j + 1, (rdata & 0x0F) );
                goto endx;
            }
       }
        vTaskDelay(100);
    }
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //Read Cycle Ouput Source Regs 1-16 #51-66
    for(j = 0;j < MAX_OUTCHANS;j++)
    {
        fstep++;
        addr = 51 + j;
        flg = twi1logic_readcycle(0, addr, &rdata);
        if(flg != 0)
        {
            debug_msg("ERROR, 'logic_read_cycle' !\r\n");
            goto endx;
        }
        else
        {
            //Validate
            if(validate_regmap(rdata & 0x0F) == 0)
            {
                logiccfg->regmap[j] = rdata & 0x0F;
                if(debugon != 0)
                    debug_msg("SUCCESS READ Valid MAP[%d] Addr %02X Data %02X\r\n", j + 1, addr, (rdata & 0x0F) );
            }
            else
            {
                debug_msg("ERROR, Illegal MAP[%d] val %d !\r\n", j + 1, (rdata & 0x0F) );
                goto endx;
            }
        }
        vTaskDelay(100);
    }
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //Read Cycle enabalarm
    fstep++;
    addr = 41;
    flg = twi1logic_readcycle(0, addr, &rdata);
    if(flg != 0)
    {
        debug_msg("ERROR, 'logic_read_cycle' !\r\n");
        goto endx;
    }
    else
    {
        //Validate
        if(validate_enabalarm(rdata & 0x0F) == 0)
        {
            logiccfg->enabalarm = rdata & 0x0F;
            if(debugon != 0)
                debug_msg("SUCCESS READ Valid enabalarm Addr %02XH Data %02XH\r\n", addr, (rdata & 0x0F) );
        }
        else
        {
            debug_msg("ERROR, Illegal enabalarm val %d !\r\n", (rdata & 0x0F) );
            goto endx;
        }
    }
    vTaskDelay(100);
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    logiccfg->valid = 1;
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    /////////
    fstep = 0;
    ret = 0;
    //
    ////////////////////////
    endx:
    ////////////////////////
    *testfailstep = fstep;
    return ret;
    ////////////////////////
}

static int validate_reggf(uint8_t rdata)
{
    int ret = 0;
    //
    if(rdata > 8)
    {
        ret = -1;
    }
    return ret;
}

static int validate_regpw(uint8_t rdata)
{
    int ret = 0;
    //
    if(rdata > 15)
    {
        ret = -1;
    }
    return ret;
}

static int validate_regmap(uint8_t rdata)
{
    int ret = 0;
    //
    if(rdata > 4)
    {
        ret = -1;
    }
    return ret;
}

static int validate_enabalarm(uint8_t rdata)
{
    int ret = 0;
    //
    if(rdata > 0x0F)
    {
        ret = -1;
    }
    return ret;
}


static int construct_logiccfg(OAMCFGDBASE_T *cfgdbase,LOGICCFG_T *logiccfg, int *testfailstep)
{
    int ret = -1;
    int flg,j;
    uint32_t fstep;
    //uint8_t page = 0;
    uint8_t addr, rdata;
    uint8_t found1 = 0;
    uint8_t found2 = 0;
    uint8_t found3 = 0;
    uint8_t found4 = 0;
    uint8_t enabalarm = 0;
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    logiccfg->valid = 0;
    fstep = 0; //starting
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    vTaskDelay(200);
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    if(cfgdbase->isvalid == 0)
    {
        debug_msg("ERROR, 'Dbase Illegal' !\r\n");
        goto endx;
    }
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //SET Glitch Filter Regs 1-4 #31-#34
    for(j = 0;j < MAX_INCHANS;j++)
    {
        fstep++;
        if(validate_reggf(cfgdbase->glitchfilter[j] & 0x0F) == 0)
        {
            logiccfg->reggf[j] = cfgdbase->glitchfilter[j] & 0x0F;
#if DEBUG
            debug_msg("SUCCESS CONSTRUCT GF[%d] Data %02X\r\n", j + 1, logiccfg->reggf[j] );
#endif
        }
        else
        {
            debug_msg("ERROR, Illegal GF[%d] val %d !\r\n", j + 1, (cfgdbase->glitchfilter[j] & 0x0F) );
            goto endx;
        }
    }
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //SET Pulse Width Regs 1-16 #11-#26
    for(j = 0;j < MAX_OUTCHANS;j++)
    {
        fstep++;
        if(validate_regpw(cfgdbase->outputpulse[j] & 0x0F) == 0)
        {
            logiccfg->regpw[j] = cfgdbase->outputpulse[j] & 0x0F;
#if DEBUG
            debug_msg("SUCCESS CONSTRUCT PW[%d] Data %02X\r\n", j + 1, logiccfg->regpw[j] );
#endif
        }
        else
        {
            debug_msg("ERROR, Illegal PW[%d] val %d !\r\n", j + 1, (cfgdbase->outputpulse[j] & 0x0F) );
            goto endx;
        }
    }
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    for(j = 0;j < MAX_OUTCHANS;j++)
    {
        fstep++;
        if(validate_regmap(cfgdbase->outputsource[j] & 0x0F) == 0)
        {
            logiccfg->regmap[j] = cfgdbase->outputsource[j] & 0x0F;
#if DEBUG
            debug_msg("SUCCESS CONSTRUCT MAP[%d] Data %02X\r\n", j + 1, logiccfg->regmap[j] );
#endif
            if(logiccfg->regmap[j] == 1)
            {
                found1 = 1;
                enabalarm |= 0x01;
            }
            else if(logiccfg->regmap[j] == 2)
            {
                found2 = 1;
                enabalarm |= 0x02;
            }
            else if(logiccfg->regmap[j] == 3)
            {
                found3 = 1;
                enabalarm |= 0x04;
            }
            else if(logiccfg->regmap[j] == 4)
            {
                found4 = 1;
                enabalarm |= 0x08;
            }
        }
        else
        {
            debug_msg("ERROR, Illegal MAP[%d] val %d !\r\n", j + 1, (cfgdbase->outputsource[j] & 0x0F) );
            goto endx;
        }
    }
#if DEBUG
            debug_msg("ENABALARM byte #41 %02X\r\n", enabalarm );
#endif
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //ENABALARM byte as per found
    logiccfg->enabalarm = enabalarm;
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    logiccfg->valid = 1;
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    /////////
    fstep = 0;
    ret = 0;
    //
    ////////////////////////
    endx:
    ////////////////////////
    *testfailstep = fstep;
    return ret;
    ////////////////////////
}


static int write_logiccfg(LOGICCFG_T *logiccfg, int *testfailstep)
{
    int ret = -1;
    int flg,j;
    uint32_t fstep = 0;
    //uint8_t page = 0;
    uint8_t addr, wdata;
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    if(logiccfg->valid == 0)
    {
        debug_msg("ERROR, 'Reg Values Illegal' !\r\n");
        goto endx;
    }
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    vTaskDelay(200);
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //Write Cycle Glitch Filter Regs 1-4 #31-#34
    for(j = 0;j < MAX_INCHANS;j++)
    {
        fstep++;
        addr = 31 + j;
        wdata = logiccfg->reggf[j] & 0x0F;
        flg = twi1logic_writecycle(0, addr, wdata);
        if(flg != 0)
        {
            debug_msg("ERROR, 'logic_write_cycle' !\r\n");
            goto endx;
        }
        else
        {
#if DEBUG
            debug_msg("SUCCESS Write GF[%d] Addr %02X Data %02X\r\n", j + 1, addr, wdata);
#endif
        }
        vTaskDelay(100);
    }
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //Write Cycle Pulse Width Regs 1-16 #11-#26
    for(j = 0;j < MAX_OUTCHANS;j++)
    {
        fstep++;
        addr = 11 + j;
        wdata = logiccfg->regpw[j] & 0x0F;
        flg = twi1logic_writecycle(0, addr, wdata);
        if(flg != 0)
        {
            debug_msg("ERROR, 'logic_write_cycle' !\r\n");
            goto endx;
        }
        else
        {
#if DEBUG
            debug_msg("SUCCESS Write PW[%d] Addr %02X Data %02X\r\n", j + 1, addr, wdata);
#endif
        }
        vTaskDelay(100);
    }
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //Write Cycle Output Source Regs 1-16 #51-66
    for(j = 0;j < MAX_OUTCHANS;j++)
    {
        fstep++;
        addr = 51 + j;
        wdata = logiccfg->regmap[j] & 0x0F;
        flg = twi1logic_writecycle(0, addr, wdata);
        if(flg != 0)
        {
            debug_msg("ERROR, 'logic_write_cycle' !\r\n");
            goto endx;
        }
        else
        {
#if DEBUG
            debug_msg("SUCCESS Write MAP[%d] Addr %02X Data %02X\r\n", j + 1, addr, wdata);
#endif
        }
        vTaskDelay(100);
    }
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //Write Cycle enabalarm
    fstep++;
    addr = 41;
    wdata = logiccfg->enabalarm & 0x0F;
    flg = twi1logic_writecycle(0, addr, wdata);
    if(flg != 0)
    {
        debug_msg("ERROR, 'logic_write_cycle' !\r\n");
        goto endx;
    }
    else
    {
#if DEBUG
        debug_msg("SUCCESS Write enabalarm Addr %02X Data %02X\r\n", addr, wdata);
#endif
    }
    vTaskDelay(100);


    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    logiccfg->valid = 1;
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    /////////
    fstep = 0;
    ret = 0;
    //
    ////////////////////////
    endx:
    ////////////////////////
    *testfailstep = fstep;
    return ret;
    ////////////////////////
}



static int compare_logiccfg(LOGICCFG_T *wrlogiccfg, LOGICCFG_T *rdlogiccfg, int *testfailstep)
{
    int ret = -1;
    int j;
    uint32_t fstep;
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    fstep = 0; //starting
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    vTaskDelay(200);
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    if(wrlogiccfg->valid == 0 || rdlogiccfg->valid == 0)
    {
        debug_msg("ERROR, 'Config Illegal' !\r\n");
        goto endx;
    }
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //Compare R/W Glitch Filter Regs 1-4 #31-#34
    for(j = 0;j < MAX_INCHANS;j++)
    {
        fstep++;
        if(wrlogiccfg->reggf[j] != rdlogiccfg->reggf[j])
        {
            debug_msg("ERROR, MisMatch GF[%d] WR %02X RD %02X!\r\n", j + 1, wrlogiccfg->reggf[j], rdlogiccfg->reggf[j] );
            goto endx;
        }
        else
        {
#if DEBUG
            debug_msg("SUCCESS MATCH GF[%d]\r\n", j + 1);
#endif
        }
    }
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //Compare R/W Pulse Width Regs 1-16 #11-#26
    for(j = 0;j < MAX_OUTCHANS;j++)
    {
        fstep++;
        if(wrlogiccfg->regpw[j] != rdlogiccfg->regpw[j])
        {
            debug_msg("ERROR, MisMatch PW[%d] WR %02X RD %02X!\r\n", j + 1, wrlogiccfg->regpw[j], rdlogiccfg->regpw[j] );
            goto endx;
        }
        else
        {
#if DEBUG
            debug_msg("SUCCESS MATCH PW[%d]\r\n", j + 1);
#endif
        }
    }
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //Compare R/W Output Source Regs 1-16 #51-66
    for(j = 0;j < MAX_OUTCHANS;j++)
    {
        fstep++;
        if(wrlogiccfg->regmap[j] != rdlogiccfg->regmap[j])
        {
            debug_msg("ERROR, MisMatch MAP[%d] wr %02X rd %02X!\r\n", j + 1, wrlogiccfg->regmap[j], rdlogiccfg->regmap[j] );
            goto endx;
        }
        else
        {
#if DEBUG
            debug_msg("SUCCESS MATCH MAP[%d]\r\n", j + 1);
#endif
        }
    }
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //Compare enabalarm Reg #41
    if(wrlogiccfg->enabalarm != rdlogiccfg->enabalarm)
    {
        debug_msg("ERROR, MisMatch enabalarm wr %02XH rd %02XH !\r\n", j + 1, wrlogiccfg->enabalarm, rdlogiccfg->enabalarm );
        goto endx;
    }
    else
    {
#if DEBUG
        debug_msg("SUCCESS enabalarm\r\n");
#endif
    }


    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


    /////////
    fstep = 0;
    ret = 0;
    //
    ////////////////////////
    endx:
    ////////////////////////
    *testfailstep = fstep;
    return ret;
    ////////////////////////
    //
}


/*****************************************************************************
** Function name:       boot_logicconfig
** Descriptions:        BOOT Logic Config
**                      STAGE-1: Construct Reg CONFIG Values from DBASE
**                               These should be valid
**                      STAGE-2: WRITE Reg CONFIG Values to Logic
**                               This should be successful
**                      STAGE-3: Read Back Reg CONFIG Values from Logic
**                               These should be valid
**                      STAGE-4: Compare R/W CONFIG Values to/from Logic
**                               These should be valid & PASSED
**
**
** Parameters:          NONE
**
** Returned value:      0 success, -1 fail
**                      int *failstage 1-4 StageNo Failed
**                      int *testfailstep FailStep in case of Stage Failure
**
*****************************************************************************/
int boot_logicconfig(int *failstage, int *testfailstep)
{
    int flg;
    int ret = -1;
    int fstep;
    //====================================================================
#if 0
    //Read Default Config
    flg = read_logiccfg(&g_LOGIC_DEFAULTCFG, &fstep, 1);
    if(flg != 0)
    {
        *failstage = 0;
        *testfailstep = fstep;
        goto endx;
    }
#endif
    //====================================================================
    //CONSTRUCT Regs Config from DBASE
    flg = construct_logiccfg(&g_CFG_DBASE ,&g_LOGIC_WRCFG, &fstep);
    if(flg != 0)
    {
        *failstage = 1;
        *testfailstep = fstep;
        goto endx;
    }
    //====================================================================
    //WRITE Regs Config
    flg = write_logiccfg(&g_LOGIC_WRCFG, &fstep);
    if(flg != 0)
    {
        *failstage = 2;
        *testfailstep = fstep;
        goto endx;
    }
    //====================================================================
    //READ Regs Config
    flg = read_logiccfg(&g_LOGIC_RDCFG, &fstep, 1);
    if(flg != 0)
    {
        *failstage = 3;
        *testfailstep = fstep;
        goto endx;
    }
    //====================================================================
    //COMPARE Regs Config
    flg = compare_logiccfg(&g_LOGIC_WRCFG, &g_LOGIC_RDCFG, &fstep);
    if(flg != 0)
    {
        *failstage = 4;
        *testfailstep = fstep;
        goto endx;
    }
    //====================================================================
    ret = 0;
    *failstage = 0;
    *testfailstep = 0;
    //====================================================================
    endx:
    //===========
    return ret;
    //===========
    //
}


int program_gf(uint8_t channo, uint8_t gfvalue, int debugon)
{
    int ret = -1;
    int c1,c2,j,flg;
    uint8_t addr,wdata,rdata,exdata;
    //
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //Validate ChanNo
    if(channo != 99 && (channo <1 || channo > MAX_INCHANS))
    {
        debug_msg("Error GF Reg Access, Illegal Channo [%d] !\r\n",channo);
        goto endx;
    }
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //Validate GF Value
    if(validate_reggf(gfvalue) != 0)
    {
        debug_msg("Error GF Reg Access, Illegal GFValue [%d] !\r\n",gfvalue);
        goto endx;
    }
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //CanNo or Range
    if(channo == 99)
    {
        c1 = 1;
        c2 = MAX_INCHANS;
    }
    else
    {
        c1 = channo;
        c2 = channo;
    }
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //Write
    for(j = c1;j < (c2+1);j++)
    {
        addr = 31 + j - 1;
        wdata = gfvalue & 0x0F;
        flg = twi1logic_writecycle(0, addr, wdata);
        if(flg != 0)
        {
            debug_msg("ERROR, 'logic_write_cycle' !\r\n");
            goto endx;
        }
        else
        {
            if(debugon)
            {
                debug_msg("SUCCESS Write GF[%d] Addr %02X Data %02X\r\n", j, addr, wdata);
            }
        }
        vTaskDelay(100);
    }
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //READ
    for(j = c1;j < (c2+1);j++)
    {
        addr = 31 + j - 1;
        exdata = gfvalue & 0x0F;
        flg = twi1logic_readcycle(0, addr, &rdata);
        if(flg != 0)
        {
            debug_msg("ERROR, 'logic_write_cycle' !\r\n");
            goto endx;
        }
        else
        {
            if(debugon)
            {
                debug_msg("SUCCESS Read GF[%d] Addr %02X Data %02X\r\n", j, addr, rdata);
            }
        }
        vTaskDelay(100);
        if((rdata & 0x0F) != exdata)
        {
            debug_msg("ERROR, Compare GF[%d] !\r\n", j);
        }
    }
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //
    ret = 0;
    //
    ////////////////
    endx:
    return ret;
    ////////////////
    //
}

int program_pw(uint8_t channo, uint8_t pwvalue, int debugon)
{
    int ret = -1;
    int c1,c2,j,flg;
    uint8_t addr,wdata,rdata,exdata;
    //
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //Validate ChanNo
    if(channo != 99 && (channo <1 || channo > MAX_OUTCHANS))
    {
        debug_msg("Error PW Reg Access, Illegal Channo [%d] !\r\n",channo);
        goto endx;
    }
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //Validate GF Value
    if(validate_regpw(pwvalue) != 0)
    {
        debug_msg("Error PW Reg Access, Illegal PWValue [%d] !\r\n",pwvalue);
        goto endx;
    }
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //CanNo or Range
    if(channo == 99)
    {
        c1 = 1;
        c2 = MAX_OUTCHANS;
    }
    else
    {
        c1 = channo;
        c2 = channo;
    }
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //Write
    for(j = c1;j < (c2+1);j++)
    {
        addr = 11 + j - 1;
        wdata = pwvalue & 0x0F;
        flg = twi1logic_writecycle(0, addr, wdata);
        if(flg != 0)
        {
            debug_msg("ERROR, 'logic_write_cycle' !\r\n");
            goto endx;
        }
        else
        {
            if(debugon)
            {
                debug_msg("SUCCESS Write PW[%d] Addr %02X Data %02X\r\n", j, addr, wdata);
            }
        }
        vTaskDelay(100);
    }
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //READ
    for(j = c1;j < (c2+1);j++)
    {
        addr = 11 + j - 1;
        exdata = pwvalue & 0x0F;
        flg = twi1logic_readcycle(0, addr, &rdata);
        if(flg != 0)
        {
            debug_msg("ERROR, 'logic_write_cycle' !\r\n");
            goto endx;
        }
        else
        {
            if(debugon)
            {
                debug_msg("SUCCESS Read PW[%d] Addr %02X Data %02X\r\n", j, addr, rdata);
            }
        }
        vTaskDelay(100);
        if((rdata & 0x0F) != exdata)
        {
            debug_msg("ERROR, Compare PW[%d] !\r\n", j);
        }
    }
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //
    ret = 0;
    //
    ////////////////
    endx:
    return ret;
    ////////////////
    //
}

int program_map(uint8_t channo, uint8_t mapvalue, int debugon)
{
    int ret = -1;
    int c1,c2,j,flg;
    uint8_t addr,wdata,rdata,exdata;
    //
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //Validate ChanNo
    if(channo != 99 && (channo <1 || channo > MAX_OUTCHANS) )
    {
        debug_msg("Error MAP Reg Access, Illegal Channo [%d] !\r\n",channo);
        goto endx;
    }
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //Validate GF Value
    if(validate_regmap(mapvalue) != 0)
    {
        debug_msg("Error MAP Reg Access, Illegal MAPValue [%d] !\r\n",mapvalue);
        goto endx;
    }
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //CanNo or Range
    if(channo == 99)
    {
        c1 = 1;
        c2 = MAX_OUTCHANS;
    }
    else
    {
        c1 = channo;
        c2 = channo;
    }
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //Write
    for(j = c1;j < (c2+1);j++)
    {
        addr = 51 + j - 1;
        wdata = mapvalue & 0x0F;
        flg = twi1logic_writecycle(0, addr, wdata);
        if(flg != 0)
        {
            debug_msg("ERROR, 'logic_write_cycle' !\r\n");
            goto endx;
        }
        else
        {
            if(debugon)
            {
                debug_msg("SUCCESS Write MAP[%d] Addr %02X Data %02X\r\n", j, addr, wdata);
            }
        }
        vTaskDelay(100);
    }
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //READ
    for(j = c1;j < (c2+1);j++)
    {
        addr = 51 + j - 1;
        exdata = mapvalue & 0x0F;
        flg = twi1logic_readcycle(0, addr, &rdata);
        if(flg != 0)
        {
            debug_msg("ERROR, 'logic_write_cycle' !\r\n");
            goto endx;
        }
        else
        {
            if(debugon)
            {
                debug_msg("SUCCESS Read MAP[%d] Addr %02X Data %02X\r\n", j, addr, rdata);
            }
        }
        vTaskDelay(100);
        if((rdata & 0x0F) != exdata)
        {
            debug_msg("ERROR, Compare MAP[%d] !\r\n", j);
        }
    }
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    uint8_t newenabalarm;
    flg = reevaluate_enabalarm(&g_CFG_DBASE, &newenabalarm);
    if(flg != 0)
    {
        goto endx;
    }
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //Write Cycle enabalarm
    addr = 41;
    wdata = newenabalarm;
    flg = twi1logic_writecycle(0, addr, wdata);
    if(flg != 0)
    {
        debug_msg("ERROR, 'logic_write_cycle' !\r\n");
        goto endx;
    }
    else
    {
#if DEBUG
        debug_msg("SUCCESS Write enabalarm Addr %02X Data %02X\r\n", addr, wdata);
#endif
    }
    vTaskDelay(100);

    ////////////////
    ret = 0;
    //
    ////////////////
    endx:
    return ret;
    ////////////////
    //
}

static int reevaluate_enabalarm(OAMCFGDBASE_T *cfgdbase, uint8_t *newenabalarm)
{
    uint8_t outputsource, tenabalarm;
    tenabalarm = 0;
    int j, ret = 0;
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
     for(j = 0;j < MAX_OUTCHANS;j++)
     {
         if(validate_regmap(cfgdbase->outputsource[j] & 0x0F) == 0)
         {
             outputsource = cfgdbase->outputsource[j] & 0x0F;
 #if DEBUG
             debug_msg("SUCCESS CONSTRUCT MAP[%d] Data %02X\r\n", j + 1, outputsource );
 #endif
             if(outputsource == 1)
             {
                 tenabalarm |= 0x01;
             }
             else if(outputsource == 2)
             {
                 tenabalarm |= 0x02;
             }
             else if(outputsource == 3)
             {
                 tenabalarm |= 0x04;
             }
             else if(outputsource == 4)
             {
                 tenabalarm |= 0x08;
             }
         }
         else
         {
             debug_msg("ERROR, Illegal MAP[%d] val %d !\r\n", j + 1, (cfgdbase->outputsource[j] & 0x0F) );
             ret = -1;
             goto endx;
         }
     }
     /////////////////////////////
     endx:
     *newenabalarm = tenabalarm;
     return ret;
     /////////////////////////////
     //
}

int clear_alarmrelay(void)
{
    int ret = -1;
    int c1,c2,j,flg;
    uint8_t addr,wdata,rdata,exdata;
    //
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //Write Cycle nclearalarm
    addr = 42;
    wdata = 0x0;
    flg = twi1logic_writecycle(0, addr, wdata);
    if(flg != 0)
    {
        debug_msg("ERROR, 'logic_write_cycle' !\r\n");
        goto endx;
    }
    else
    {
#if DEBUG
        debug_msg("SUCCESS Write nclearalarm Addr %02X Data %02X\r\n", addr, wdata);
#endif
    }
    vTaskDelay(100);
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //Write Cycle nclearalarm
    addr = 42;
    wdata = 0x01;
    flg = twi1logic_writecycle(0, addr, wdata);
    if(flg != 0)
    {
        debug_msg("ERROR, 'logic_write_cycle' !\r\n");
        goto endx;
    }
    else
    {
#if DEBUG
        debug_msg("SUCCESS Write nclearalarm Addr %02X Data %02X\r\n", addr, wdata);
#endif
    }
    vTaskDelay(100);
    ret = 0;
    endx:
    return ret;
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //
}

int statemcpw_start(int debugon)
{
    int ret = -1;
    int c1,c2,j,flg;
    uint8_t addr,wdata,rdata,exdata;
    //
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //Write Cycle mcstart
    addr = 44;
    wdata = 0x01;
    flg = twi1logic_writecycle(0, addr, wdata);
    if(flg != 0)
    {
        debug_msg("ERROR, 'logic_write_cycle' !\r\n");
        goto endx;
    }
    else
    {
#if DEBUG
        debug_msg("SUCCESS Write MCSTARTPW Addr %02X Data %02X\r\n", addr, wdata);
#endif
    }
    vTaskDelay(100);
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //Read Cycle mcstart
    exdata = 0x01;
    flg = twi1logic_readcycle(0, addr, &rdata);
    if(flg != 0)
    {
        debug_msg("ERROR, 'logic_read_cycle' !\r\n");
        goto endx;
    }
    else
    {
        if(debugon)
        {
            debug_msg("SUCCESS Read MCSTARTPW Addr %02X Data %02X\r\n", addr, rdata);
        }
    }
    vTaskDelay(100);
    if((rdata & 0x01) != exdata)
    {
        debug_msg("ERROR, Compare MCSTARTPW !\r\n");
        goto endx;
    }
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    ret = 0;
    /////////////
    endx:
    return ret;
    //
}

int statemcgf_start(int debugon)
{
    int ret = -1;
    int c1,c2,j,flg;
    uint8_t addr,wdata,rdata,exdata;
    //
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //Write Cycle mcstart
    addr = 43;
    wdata = 0x01;
    flg = twi1logic_writecycle(0, addr, wdata);
    if(flg != 0)
    {
        debug_msg("ERROR, 'logic_write_cycle' !\r\n");
        goto endx;
    }
    else
    {
#if DEBUG
        debug_msg("SUCCESS Write MCSTARTGF Addr %02X Data %02X\r\n", addr, wdata);
#endif
    }
    vTaskDelay(100);
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //Read Cycle mcstart
    exdata = 0x01;
    flg = twi1logic_readcycle(0, addr, &rdata);
    if(flg != 0)
    {
        debug_msg("ERROR, 'logic_read_cycle' !\r\n");
        goto endx;
    }
    else
    {
        if(debugon)
        {
            debug_msg("SUCCESS Read MCSTARTGF Addr %02X Data %02X\r\n", addr, rdata);
        }
    }
    vTaskDelay(100);
    if((rdata & 0x01) != exdata)
    {
        debug_msg("ERROR, Compare MCSTARTGF !\r\n");
        goto endx;
    }
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    ret = 0;
    /////////////
    endx:
    return ret;
    //
}

