#include <msp430.h>
#include "i2cbb.h"

void main(void){
  const unsigned char msg[]={1,2,3};
  unsigned char rec[4];
  //setup for bit bang I2C
  i2c_bb_setup();
  //setup P7 for LED's
  P7OUT=0;
  P7DIR=0xFF;
  P7REN=0;
  //loop write and read I2C
  for(;;){
    i2c_bb_tx(0x24,msg,sizeof(msg));
    //pause a bit between transactions
    __delay_cycles(800000);
    //flash LED
    P7OUT^=BIT0;
    //recive data
    i2c_bb_rx(0x24,rec,sizeof(rec));
    //pause a bit between transactions
    __delay_cycles(800000);
    //flash LED
    P7OUT^=BIT0;
  }
}
