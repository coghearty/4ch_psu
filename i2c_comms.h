/*
 * i2c_comms.h
 *
 * Created: 11/02/2017 2:08:09 PM
 *  Author: NormalUser
 */ 


#ifndef I2C_COMMS_H_
#define I2C_COMMS_H_

#define DAC_CONFIG		0x00
#define CH1_DAC_ADDR	0x4E


void init_i2c(void);

//int i2c_writebyte(unsigned int dev_addr,char data1, char data2);
int i2c_DAC101C08x_write(unsigned int dev_addr, uint8_t config, uint16_t data);
#endif /* I2C_COMMS_H_ */