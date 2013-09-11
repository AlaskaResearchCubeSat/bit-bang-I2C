#ifndef __I2CBB_H
#define __I2CBB_H

//port to use for I2C pins
#define I2C_BB_PORT P5
//pins to use on I2C port
#define I2C_BB_SDA  (BIT1)
#define I2C_BB_SCL  (BIT2)

//tricky macros
//needed to get around macro expansion
#define out(r)      _out(r)
#define _out(r)     (r##OUT)
#define in(r)      _in(r)
#define _in(r)     (r##IN)
#define dir(r)      _dir(r)
#define _dir(r)     (r##DIR)

//I2C registers
#define I2C_BB_IN   in(I2C_BB_PORT)
#define I2C_BB_OUT  out(I2C_BB_PORT)
#define I2C_BB_DIR  dir(I2C_BB_PORT)

//Function prototypes
void i2c_bb_setup(void);
short i2c_bb_tx(unsigned char addr,const unsigned char *dat,unsigned short len);
short i2c_bb_rx(unsigned char addr,unsigned char *dest,unsigned short len);



#endif
  