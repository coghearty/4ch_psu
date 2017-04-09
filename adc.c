/*
 * adc.c
 *
 * Created: 15/02/2017 2:55:36 PM
 *  Author: NormalUser
 */ 
#include "adc.h"
#include "hardware.h"

void init_adc(void){
	/*adc prescaler is the division of the CPU frequency to get an ADC frequency between 50 - 200 kHz*/
	/*Assuming CPU frequency of 1 MHz, use a prescaler of 8 to get 125 kHz ADC frequency*/
	ADCSRA |= (1 << ADPS1) | (1 << ADPS0); //set prescale to 8
	
	/*set the analog reference voltage to be from the Aref pin*/
	ADMUX &= ~(1 << REFS0)  & ~(1 << REFS1);
	/*Clear any channel selections, and clear REFS0 & 1 so we use the analog reference from the Aref pin*/
	/*Also Clear ADLAR  so the data is right adjusted and we get the full 10-bits*/
	ADMUX = 0x00;
	/*Set the analog input to be read (start with ADC7) */
	//ADMUX |= (1 << MUX2) | (1 << MUX1) | (1 << MUX0);
	//ADMUX |= CH4_VSENSE;
	/*Set the ADC to free-running mode (auto-trigger) */
	//ADCSRA |= (1 << ADATE);
		
}

void enable_adc(void){
	/*enable the ADC*/
	ADCSRA |= (1 << ADEN);
	
	/*start ADC conversions*/
	//ADCSRA |= (1 << ADSC);
	
}

void disable_adc(void){
	/*disable the adc*/
	ADCSRA &= ~(1 << ADEN);
	
}

uint16_t read_ADC(uint8_t channel){
	uint8_t adc_L;
	uint8_t adc_H;
	ADMUX = 0x00;			//clear any previous selected channels
	ADMUX = channel;		//set the desired channel
	ADCSRA |= (1 << ADSC);	//start an ADC conversion
	while(ADSC == 1);		//wait for conversion to finish
	
	adc_L = ADCL;
	adc_H = ADCH;
	return((adc_H << 8) | (adc_L & 0xFF));
}