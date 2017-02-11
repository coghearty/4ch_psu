/*
 * i2c_comms.h
 *
 * Created: 11/02/2017 2:08:09 PM
 *  Author: NormalUser
 */ 


#ifndef I2C_COMMS_H_
#define I2C_COMMS_H_

/*First 2 bits of DAC config are always 0
 *Last 2 bits set the mode of operation
 * 0  0		Normal Operation
 * 0  1		2.5k Ohm to Ground
 * 1  0		100k Ohm to Ground
 * 1  1		High Impedance
*/
#define DAC_CONFIG		0x00

#define CH1_DAC_ADDR	0x4E
#define CH2_DAC_ADDR	0x09
#define CH3_DAC_ADDR	0x4D
#define CH4_DAC_ADDR	0x0A

#define DAC_MIN_VAL				0x00	//0
#define DAC_MAX_VAL				0xFFC	//4092
#define DAC_MIN_MILLIVOLTS		0
#define DAC_MAX_MILLIVOLTS		2048

#define CHANNEL_1		0
#define CHANNEL_2		1
#define CHANNEL_3		2
#define CHANNEL_4		3

void init_i2c(void);

int i2c_DAC101C08x_write(unsigned int dev_addr, uint8_t config, uint16_t data);
int i2c_DAC101C08x_read(unsigned int dev_addr, uint16_t *data);
int set_DAC_mV(unsigned char channel, unsigned int millivolts);
int read_DAC_mV(unsigned char channel, unsigned int *millivolts);

#endif /* I2C_COMMS_H_ */