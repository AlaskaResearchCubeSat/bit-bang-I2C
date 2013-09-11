#include "i2cbb.h"
#include <msp430.h>


//setup bit bang I2C pins
void i2c_bb_setup(void){
  //set pins as inputs
  I2C_BB_DIR&=~(I2C_BB_SDA|I2C_BB_SCL);
  //set output to low
  I2C_BB_OUT&=~(I2C_BB_SDA|I2C_BB_SCL);
}

//wait for 1/2 of I2C clock period
void i2c_bb_hc(void){
  //wait for 0.05ms
  __delay_cycles(800);
}

void i2c_bb_start(void){
  //wait for 1/2 clock first
  i2c_bb_hc();
  //pull SDA low
  I2C_BB_DIR|=I2C_BB_SDA;
  //wait for 1/2 clock for end of start
  i2c_bb_hc();
}

void i2c_bb_stop(void){
  //pull SDA low
  I2C_BB_DIR|=I2C_BB_SDA;
  //wait for 1/2 clock for end of start
  i2c_bb_hc();
  //float SCL
  I2C_BB_DIR&=~I2C_BB_SCL;
  //wait for 1/2 clock
  i2c_bb_hc();
  //float SDA
  I2C_BB_DIR&=~I2C_BB_SDA;
  //wait for 1/2 clock
  i2c_bb_hc();

}

//send value over I2C return 1 if slave ACKed
short i2c_bb_tx_byte(unsigned char val){
  int i;
  //shift out bits
  for(i=0;i<8;i++){      
    //pull SCL low
    I2C_BB_DIR|=I2C_BB_SCL;
    //check bit
    if(val&0x80){
      //float SDA
      I2C_BB_DIR&=~I2C_BB_SDA;
    }else{
      //pull SDA low
      I2C_BB_DIR|=I2C_BB_SDA;
    }
    //shift
    val<<=1;
    //wait for 1/2 clock
    i2c_bb_hc();
    //float SCL
    I2C_BB_DIR&=~I2C_BB_SCL;
    //wait for 1/2 clock
    i2c_bb_hc();
  }
  //check ack bit
  //pull SCL low
  I2C_BB_DIR|=I2C_BB_SCL;
  //float SDA
  I2C_BB_DIR&=~I2C_BB_SDA;
  //wait for 1/2 clock
  i2c_bb_hc();
  //float SCL
  I2C_BB_DIR&=~I2C_BB_SCL;
  //wait for 1/2 clock
  i2c_bb_hc();
  //sample SDA
  val=I2C_BB_IN&I2C_BB_SDA;
  //pull SCL low
  I2C_BB_DIR|=I2C_BB_SCL;
  //return sampled value
  return !val;
}
//send value over I2C return 1 if slave ACKed
unsigned char i2c_bb_rx_byte(unsigned short ack){
  unsigned char val;
  int i;
  //shift out bits
  for(i=0;i<8;i++){      
    //pull SCL low
    I2C_BB_DIR|=I2C_BB_SCL;
    //wait for 1/2 clock
    i2c_bb_hc();
    //float SCL
    I2C_BB_DIR&=~I2C_BB_SCL;
    //wait for 1/2 clock
    i2c_bb_hc();
    //shift value to make room
    val<<=1;
    //sample data
    if(I2C_BB_IN&I2C_BB_SDA){
      val|=1;
    }
  }
  //check ack bit
  //pull SCL low
  I2C_BB_DIR|=I2C_BB_SCL;
  //check if we are ACKing this byte
  if(ack){
    //pull SDA low for ACK
    I2C_BB_DIR|=I2C_BB_SDA;
  }else{
    //float SDA for NACK
    I2C_BB_DIR&=~I2C_BB_SDA;
  }
  //wait for 1/2 clock
  i2c_bb_hc();
  //float SCL
  I2C_BB_DIR&=~I2C_BB_SCL;
  //wait for 1/2 clock
  i2c_bb_hc();
  //pull SCL low
  I2C_BB_DIR|=I2C_BB_SCL;
  //float SDA
  I2C_BB_DIR&=~I2C_BB_SDA;
  //return value
  return val;
}

short i2c_bb_tx(unsigned char addr,const unsigned char *dat,unsigned short len){
  short ack;
  int i;
  //send start
  i2c_bb_start();
  //send address with W bit
  ack=i2c_bb_tx_byte((addr<<1));
  //send data bytes
  for(i=0;i<len && ack;i++){
    //transmit next byte
    ack=i2c_bb_tx_byte(dat[i]);
  }
  //transmit stop
  i2c_bb_stop();
  //return if slave NACKed
  return ack;
}

short i2c_bb_rx(unsigned char addr,unsigned char *dest,unsigned short len){
  int i;
  //send start
  i2c_bb_start();
  //send address with R bit
  if(!i2c_bb_tx_byte((addr<<1)|BIT0)){
    //got NACK return error
    return 0;
  }
  //send data bytes
  for(i=0;i<len;i++){
    //transmit next byte
    dest[i]=i2c_bb_tx_byte(i==len-1);
  }
  //transmit stop
  i2c_bb_stop();
  //return if slave NACKed
  return 1;
}
