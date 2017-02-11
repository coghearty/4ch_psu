/*
* i2c_comms.c
*
* Created: 11/02/2017 2:07:54 PM
*  Author: NormalUser
*/
#include <avr/io.h>
#include <compat/twi.h>

#define MAX_TRIES 50
//#define EEPROM_ID    0xA0        // I2C 24AA128 EEPROM Device Identifier
//#define CH1_DAC_ADDR  0x9C        // I2C 24AA128 EEPROM Device Address
#define I2C_START 0
#define I2C_DATA  1
#define I2C_STOP  2

unsigned char i2c_transmit(unsigned char type) {
	switch(type) {
		case I2C_START:    // Send Start Condition
		TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
		break;
		case I2C_DATA:     // Send Data
		TWCR = (1 << TWINT) | (1 << TWEN);
		break;
		case I2C_STOP:     // Send Stop Condition
		TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
		return 0;
	}
	// Wait for TWINT flag set in TWCR Register
	while (!(TWCR & (1 << TWINT)));
	// Return TWI Status Register, mask the prescaler bits (TWPS1,TWPS0)
	return (TWSR & 0xF8);
}

int i2c_DAC101C08x_write(unsigned int dev_addr, uint8_t config, uint16_t data) {
	// Take the top 4 bits of config and the top 4 valid bits (data is actually a 12 bit number) and or them together
	uint8_t top_msg = (config & 0xF0) | (0x0F & (data >> 8));
	// Take the bottom octet of data
	uint8_t lower_msg = (data & 0x00FF);

	unsigned char n = 0;
	unsigned char twi_status;
	char r_val = 2;
	i2c_retry:
	if (n++ >= MAX_TRIES) return r_val;
	// Transmit Start Condition
	twi_status=i2c_transmit(I2C_START);

	// Check the TWI Status
	if (twi_status == TW_MT_ARB_LOST) goto i2c_retry;
	r_val = 3;
	if ((twi_status != TW_START) && (twi_status != TW_REP_START)) goto i2c_quit;
	r_val = 4;
	// Send slave address (SLA_W)
	//TWDR = (dev_id & 0xF0) | ((dev_addr & 0x07) << 1) | TW_WRITE;
	TWDR = (dev_addr << 1) | TW_WRITE;
	//TWDR = 0b10011100;
	// Transmit I2C Data
	twi_status=i2c_transmit(I2C_DATA);
	// Check the TWSR status
	if ((twi_status == TW_MT_SLA_NACK) || (twi_status == TW_MT_ARB_LOST)) goto i2c_retry;
	r_val = 5;
	if (twi_status != TW_MT_SLA_ACK) goto i2c_quit;
	r_val = 6;
	TWDR = top_msg;
	// Transmit I2C Data
	twi_status=i2c_transmit(I2C_DATA);
	// Check the TWSR status
	if (twi_status != TW_MT_DATA_ACK) goto i2c_quit;
	r_val = 7;
	// Put data into data register and start transmission
	TWDR = lower_msg;
	// Transmit I2C Data
	twi_status=i2c_transmit(I2C_DATA);
	// Check the TWSR status
	if (twi_status != TW_MT_DATA_ACK) goto i2c_quit;
	r_val = 8;
	// TWI Transmit Ok
	r_val=1;
	i2c_quit:
	// Transmit I2C Data
	twi_status=i2c_transmit(I2C_STOP);
	return r_val;
}

int i2c_readbyte(unsigned int i2c_address, unsigned int dev_id,
unsigned int dev_addr, char *data)
{
	unsigned char n = 0;
	unsigned char twi_status;
	char r_val = -1;
	i2c_retry:
	if (n++ >= MAX_TRIES) return r_val;
	// Transmit Start Condition
	twi_status=i2c_transmit(I2C_START);

	// Check the TWSR status
	if (twi_status == TW_MT_ARB_LOST) goto i2c_retry;
	if ((twi_status != TW_START) && (twi_status != TW_REP_START)) goto i2c_quit;
	// Send slave address (SLA_W) 0xa0
	TWDR = (dev_id & 0xF0) | ((dev_addr << 1) & 0x0E) | TW_WRITE;
	// Transmit I2C Data
	twi_status=i2c_transmit(I2C_DATA);
	// Check the TWSR status
	if ((twi_status == TW_MT_SLA_NACK) || (twi_status == TW_MT_ARB_LOST)) goto i2c_retry;
	if (twi_status != TW_MT_SLA_ACK) goto i2c_quit;
	// Send the High 8-bit of I2C Address
	TWDR = i2c_address >> 8;
	// Transmit I2C Data
	twi_status=i2c_transmit(I2C_DATA);
	// Check the TWSR status
	if (twi_status != TW_MT_DATA_ACK) goto i2c_quit;
	// Send the Low 8-bit of I2C Address
	TWDR = i2c_address;
	// Transmit I2C Data
	twi_status=i2c_transmit(I2C_DATA);
	// Check the TWSR status
	if (twi_status != TW_MT_DATA_ACK) goto i2c_quit;

	// Send start Condition
	twi_status=i2c_transmit(I2C_START);
	// Check the TWSR status
	if (twi_status == TW_MT_ARB_LOST) goto i2c_retry;
	if ((twi_status != TW_START) && (twi_status != TW_REP_START)) goto i2c_quit;
	// Send slave address (SLA_R)
	TWDR = (dev_id & 0xF0) | ((dev_addr << 1) & 0x0E) | TW_READ;
	// Transmit I2C Data
	twi_status=i2c_transmit(I2C_DATA);

	// Check the TWSR status
	if ((twi_status == TW_MR_SLA_NACK) || (twi_status == TW_MR_ARB_LOST)) goto i2c_retry;
	if (twi_status != TW_MR_SLA_ACK) goto i2c_quit;
	// Read I2C Data
	twi_status=i2c_transmit(I2C_DATA);
	if (twi_status != TW_MR_DATA_NACK) goto i2c_quit;
	// Get the Data
	*data=TWDR;
	r_val=1;
	i2c_quit:
	// Send Stop Condition
	twi_status=i2c_transmit(I2C_STOP);
	return r_val;
}

void init_i2c(void){
  /* Initial TWI Peripheral */
  TWSR = 0x00;   // Select Prescaler of 1
  // SCL frequency = 11059200 / (16 + 2 * 48 * 1) = 98.743 khz
  TWBR = 0x30;   // 48 Decimal
}