/****************************************************
  FILE NAME     : f01-twi1mast.c
  ORIGINATOR    : A Joshi
  LAST EDITED   : 05 JUN 2026
  PROJECT       : WS12_EPCB7011
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/

#include "f01-twi1mast.h"

//
#define DEBUG 0
//
//This file defines 1 SOFTI2C for EEPROM (I2C0)
//at SCL=PB2, SDA=PB3
//


uint8_t g_TWI1adr;
uint8_t g_TWI1data;
uint8_t g_TWI1Ackflag;


//MCU DEPENDENT FUNCTIONS
void TWI1_init(void);
void TWI1_qp_delay(void);
void TWI1_hp_delay(void);
void TWI1_sda_forcelo(void);
void TWI1_sda_releasehi(void);
void TWI1_scl_forcelo(void);
void TWI1_scl_releasehi(void);
void TWI1_WaitSclHigh(void);
uint8_t TWI1_GetSdaLevel(void);
void TWI1_delay(void);
//
void TWI1_start(void);
void TWI1_rep_start(void);
void TWI1_do_transfer(void);
void TWI1_stop(void);
void TWI1_delay(void);
//
//Other functions
void TWI1write(void);
void TWI1_get_ack(void);
void TWI1read(void);
void TWI1_put_ack(void);


//MCU DEPENDENT FUNCTIONS
void TWI1_init(void)
{
    TWI1_sda_releasehi();
    TWI1_scl_releasehi();

}

void TWI1_qp_delay(void)
{
    drive_ncslogic(0);drive_ncslogic(0);drive_ncslogic(1);
 }

void TWI1_hp_delay(void)
{
    drive_ncslogic(0);drive_ncslogic(0);drive_ncslogic(0);
    //
    drive_ncslogic(1);drive_ncslogic(1);drive_ncslogic(1);

}

void TWI1_sda_forcelo(void)
{
    //sda = LOW level
    MAP_GPIOPinTypeGPIOOutput(GPIO_PORTG_BASE,GPIO_PIN_1);
    GPIOPinWrite(GPIO_PORTG_BASE,GPIO_PIN_1,0x0);
}

void TWI1_sda_releasehi(void)
{
    GPIOPinWrite(GPIO_PORTG_BASE,GPIO_PIN_1,GPIO_PIN_1);
    MAP_GPIOPinTypeGPIOInput(GPIO_PORTG_BASE, GPIO_PIN_1);
}

void TWI1_scl_forcelo(void)
{
    //scl = LOW level
    MAP_GPIOPinTypeGPIOOutput(GPIO_PORTG_BASE,GPIO_PIN_0);
    GPIOPinWrite(GPIO_PORTG_BASE,GPIO_PIN_0,0x0);
}

void TWI1_scl_releasehi(void)
{
    GPIOPinWrite(GPIO_PORTG_BASE,GPIO_PIN_0,GPIO_PIN_0);
    MAP_GPIOPinTypeGPIOInput(GPIO_PORTG_BASE, GPIO_PIN_0);
}

void TWI1_WaitSclHigh(void)
{
    int32_t i32Val;
    uint8_t done=0;
    uint8_t errcnt = 10;//100ms
    while(done==0)
    {
        //scl bit-2
        i32Val=GPIOPinRead(GPIO_PORTG_BASE,GPIO_PIN_0);
        if(i32Val != 0)
        {
            done=1;
#if DEBUG
            debug_msg("SCL1 ACK OK !\r\n");
#endif
        }
        errcnt--;
        if(errcnt == 0)
        {
            debug_msg("SCL1 ACK Failed\r\n");
            done = 1;
        }
        vTaskDelay(10); //10ms
    }
    //
}


uint8_t TWI1_GetSdaLevel(void)
{
    uint8_t ret;
    int32_t i32Val;
    //
    //sda bit-3
    i32Val=GPIOPinRead(GPIO_PORTG_BASE,GPIO_PIN_1);
    if(i32Val == 0)
        ret = 0;
    else
        ret = 1;
    return ret;
}


//This is about 12.5 ms delay in this project
void TWI1_delay(void)
{
   int j;
   for(j=0;j<15000;j++)
   {
       drive_ncslogic(0); drive_ncslogic(0); drive_ncslogic(0); drive_ncslogic(0);
       drive_ncslogic(1);
   }

}

//
//Function initiates the TWI start &
//follows by sending the device address+R/W bit (8-bits in total)
//TWIadr - Contains byte to be transmitted.
//Gets slave acknowledge response.
void TWI1_start(void)
{
    g_TWI1data = g_TWI1adr;     // copy address to transmitt register
    TWI1_sda_forcelo();    //sda line low while scl line high
    TWI1_qp_delay();       //quarter period delay
    TWI1write();

}

void TWI1_rep_start(void)
{
    TWI1_scl_forcelo();          //scl line low
    TWI1_sda_releasehi();    //sda line high (release)
    TWI1_hp_delay();           //half period delay
    TWI1_scl_releasehi();    //scl line high (release)
    TWI1_hp_delay();           //half period delay
    TWI1_start();
}


void TWI1write(void)
{
  unsigned char j,Bit;
  for(j=0;j<8;j++)
  {
      TWI1_scl_forcelo();        //scl line low
      Bit= g_TWI1data & 0x80;  //msb bit
      if(Bit==0)
        TWI1_sda_forcelo();      //sda line low
      else
        TWI1_sda_releasehi();    //sda line high (release)
    TWI1_hp_delay();           //half period delay
    TWI1_scl_releasehi();    //scl line high (release)
    TWI1_hp_delay();           //half period delay
    g_TWI1data = g_TWI1data << 1;
  }
  TWI1_get_ack();
}

void TWI1_get_ack(void)
{
    TWI1_scl_forcelo();          //scl line low
    TWI1_sda_releasehi();      //sda line high (release)
    TWI1_hp_delay();           //half period delay
    TWI1_scl_releasehi();      //scl line high (release)
    TWI1_WaitSclHigh();            //is Scl high ? ...wait
    g_TWI1Ackflag = TWI1_GetSdaLevel();   //Sda line gives ack flag
    TWI1_hp_delay();           //half period delay
}


void TWI1_do_transfer(void)
{
  if((g_TWI1adr & 0x01)==0)
      TWI1write();
  else
      TWI1read();

}

void TWI1read(void)
{
  unsigned char j,bit;
  //
  g_TWI1data = 0x00;//init to 0
  for(j=0;j<8;j++)
  {
      TWI1_scl_forcelo();        //scl line low
      TWI1_hp_delay();           //half period delay
      TWI1_scl_releasehi();      //scl line high (release)
      TWI1_hp_delay();           //half period delay
      g_TWI1data = g_TWI1data << 1;
      bit= TWI1_GetSdaLevel();        //Sda line gives rx data bit
      if(bit==0)
        g_TWI1data &= ~0x01;
      else
        g_TWI1data |= 0x01;
  }
  TWI1_put_ack();
  //
}

void TWI1_put_ack(void)
{
    TWI1_scl_forcelo();        //scl line low
    if(g_TWI1Ackflag == 0)
      TWI1_sda_forcelo();      //sda line low
    else
      TWI1_sda_releasehi();    //sda line high (release)
    TWI1_hp_delay();           //half period delay
    TWI1_scl_releasehi();      //scl line high (release)
    TWI1_WaitSclHigh();        //is Scl high ? ...wait
    TWI1_hp_delay();           //half period delay
}


void TWI1_stop(void)
{
    TWI1_scl_forcelo();          //scl line low
    TWI1_sda_forcelo();      //sda line low
    TWI1_hp_delay();           //half period delay
    TWI1_scl_releasehi();    //scl line high (release)
    TWI1_qp_delay();           //half period delay
    TWI1_sda_releasehi();    //sda line high (release)
    TWI1_hp_delay();           //half period delay
}


