/*
 * i2c_comms.h
 *
 * Created: 11/02/2017 2:08:09 PM
 *  Author: NormalUser
 */ 


#ifndef I2C_COMMS_H_
#define I2C_COMMS_H_

void init_i2c(void);

int i2c_writebyte(unsigned int dev_addr,char data1, char data2);

#endif /* I2C_COMMS_H_ */