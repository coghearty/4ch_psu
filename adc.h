/*
 * adc.h
 *
 * Created: 15/02/2017 2:55:48 PM
 *  Author: NormalUser
 */ 


#ifndef ADC_H_
#define ADC_H_

#include <avr/io.h>

void init_adc(void);
void enable_adc(void);
void disable_adc(void);
uint16_t read_ADC(uint8_t channel);


#endif /* ADC_H_ */