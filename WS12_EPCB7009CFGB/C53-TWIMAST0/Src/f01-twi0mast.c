/****************************************************
  FILE NAME     : f01-twi0mast.c
  ORIGINATOR    : A Joshi
  LAST EDITED   : 19 MAY 2026
  PROJECT       : WS12_EPCB7011
  Target        : TM4C129DNCPDT
  Crystal       : 25.0000Mhz
  PLL Oper      : 480MHz
 ****************************************************/
/**/

#include "f01-twi0mast.h"

//
#define DEBUG 0
//
//This file defines 1 SOFTI2C for EEPROM (I2C0)
//at SCL=PB2, SDA=PB3
//


uint8_t g_TWI0adr;
uint8_t g_TWI0data;
uint8_t g_TWI0Ackflag;


//MCU DEPENDENT FUNCTIONS
void TWI0_init(void);
void TWI0_qp_delay(void);
void TWI0_hp_delay(void);
void TWI0_sda_forcelo(void);
void TWI0_sda_releasehi(void);
void TWI0_scl_forcelo(void);
void TWI0_scl_releasehi(void);
void TWI0_WaitSclHigh(void);
uint8_t TWI0_GetSdaLevel(void);
void TWI0_delay(void);
//
void TWI0_start(void);
void TWI0_rep_start(void);
void TWI0_do_transfer(void);
void TWI0_stop(void);
void TWI0_delay(void);
//
//Other functions
void TWI0write(void);
void TWI0_get_ack(void);
void TWI0read(void);
void TWI0_put_ack(void);



extern void iwdg_strobpulse(void);

//MCU DEPENDENT FUNCTIONS
void TWI0_init(void)
{
    TWI0_sda_releasehi();
    TWI0_scl_releasehi();

}

void TWI0_qp_delay(void)
{
    drive_ncslogic(0);drive_ncslogic(0);drive_ncslogic(0);drive_ncslogic(0);
    drive_ncslogic(1);drive_ncslogic(1);drive_ncslogic(1);drive_ncslogic(1);
}

void TWI0_hp_delay(void)
{
    drive_ncslogic(0);drive_ncslogic(0);drive_ncslogic(0);drive_ncslogic(0);
    drive_ncslogic(0);drive_ncslogic(0);drive_ncslogic(0);drive_ncslogic(0);
    //
    drive_ncslogic(1);drive_ncslogic(1);drive_ncslogic(1);drive_ncslogic(1);
    drive_ncslogic(1);drive_ncslogic(1);drive_ncslogic(1);drive_ncslogic(1);

}

void TWI0_sda_forcelo(void)
{
    //sda = LOW level
    MAP_GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE,GPIO_PIN_3);
    GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_3,0x0);
}

void TWI0_sda_releasehi(void)
{
    GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_3,GPIO_PIN_3);
    MAP_GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_3);
}

void TWI0_scl_forcelo(void)
{
    //scl = LOW level
    MAP_GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE,GPIO_PIN_2);
    GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_2,0x0);
}

void TWI0_scl_releasehi(void)
{
    GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_2,GPIO_PIN_2);
    MAP_GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_2);
}

void TWI0_WaitSclHigh(void)
{
    int32_t i32Val;
    uint8_t done=0;
    uint8_t errcnt = 10;//100ms
    while(done==0)
    {
        //scl bit-2
        i32Val=GPIOPinRead(GPIO_PORTB_BASE,GPIO_PIN_2);
        if(i32Val != 0)
        {
            done=1;
#if DEBUG
            debug_msg("SCL ACK OK !\r\n");
#endif
        }
        errcnt--;
        if(errcnt == 0)
        {
            debug_msg("SCL ACK Failed\r\n");
            done = 1;
        }
        vTaskDelay(10); //10ms
    }
    //
}


uint8_t TWI0_GetSdaLevel(void)
{
    uint8_t ret;
    int32_t i32Val;
    //
    //sda bit-3
    i32Val=GPIOPinRead(GPIO_PORTB_BASE,GPIO_PIN_3);
    if(i32Val == 0)
        ret = 0;
    else
        ret = 1;
    return ret;
}


//This is about 12.5 ms delay in this project
void TWI0_delay(void)
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
void TWI0_start(void)
{
    g_TWI0data = g_TWI0adr;     // copy address to transmitt register
    TWI0_sda_forcelo();    //sda line low while scl line high
    TWI0_qp_delay();       //quarter period delay
    TWI0write();

}

void TWI0_rep_start(void)
{
    TWI0_scl_forcelo();          //scl line low
    TWI0_sda_releasehi();    //sda line high (release)
    TWI0_hp_delay();           //half period delay
    TWI0_scl_releasehi();    //scl line high (release)
    TWI0_hp_delay();           //half period delay
    TWI0_start();
}


void TWI0write(void)
{
  unsigned char j,Bit;
  for(j=0;j<8;j++)
  {
      TWI0_scl_forcelo();        //scl line low
      Bit= g_TWI0data & 0x80;  //msb bit
      if(Bit==0)
        TWI0_sda_forcelo();      //sda line low
      else
         TWI0_sda_releasehi();    //sda line high (release)
    TWI0_hp_delay();           //half period delay
    TWI0_scl_releasehi();    //scl line high (release)
    TWI0_hp_delay();           //half period delay
    g_TWI0data = g_TWI0data << 1;
  }
  TWI0_get_ack();
}

void TWI0_get_ack(void)
{
    TWI0_scl_forcelo();          //scl line low
    TWI0_sda_releasehi();      //sda line high (release)
    TWI0_hp_delay();           //half period delay
    TWI0_scl_releasehi();      //scl line high (release)
    TWI0_WaitSclHigh();            //is Scl high ? ...wait
    g_TWI0Ackflag = TWI0_GetSdaLevel();   //Sda line gives ack flag
    TWI0_hp_delay();           //half period delay
}


void TWI0_do_transfer(void)
{
  if((g_TWI0adr & 0x01)==0)
      TWI0write();
  else
      TWI0read();

}

void TWI0read(void)
{
  unsigned char j,bit;
  //
  g_TWI0data = 0x00;//init to 0
  for(j=0;j<8;j++)
  {
      TWI0_scl_forcelo();        //scl line low
      TWI0_hp_delay();           //half period delay
      TWI0_scl_releasehi();      //scl line high (release)
      TWI0_hp_delay();           //half period delay
      g_TWI0data = g_TWI0data << 1;
      bit= TWI0_GetSdaLevel();        //Sda line gives rx data bit
      if(bit==0)
        g_TWI0data &= ~0x01;
      else
        g_TWI0data |= 0x01;
  }
  TWI0_put_ack();
  //
}

void TWI0_put_ack(void)
{
    TWI0_scl_forcelo();        //scl line low
    if(g_TWI0Ackflag == 0)
      TWI0_sda_forcelo();      //sda line low
    else
      TWI0_sda_releasehi();    //sda line high (release)
    TWI0_hp_delay();           //half period delay
    TWI0_scl_releasehi();      //scl line high (release)
    TWI0_WaitSclHigh();        //is Scl high ? ...wait
    TWI0_hp_delay();           //half period delay
}


void TWI0_stop(void)
{
    TWI0_scl_forcelo();          //scl line low
    TWI0_sda_forcelo();      //sda line low
    TWI0_hp_delay();           //half period delay
    TWI0_scl_releasehi();    //scl line high (release)
    TWI0_qp_delay();           //half period delay
    TWI0_sda_releasehi();    //sda line high (release)
    TWI0_hp_delay();           //half period delay
}


