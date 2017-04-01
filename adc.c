/*
 * adc.c
 *
 * Created: 15/02/2017 2:55:36 PM
 *  Author: NormalUser
 */ 
#include "adc.h"

void init_adc(void){
	/*adc prescaler is the division of the CPU frequency to get an ADC frequency between 50 - 200 kHz*/
	/*Assuming CPU frequency of 1 MHz, use a prescaler of 8 to get 125 kHz ADC frequency*/
	ADCSRA |= (1 << ADPS1) | (1 << ADPS0); //set prescale to 8
	
	/*set the analog reference voltage to be from the Aref pin*/
	ADMUX &= ~(1 << REFS0)  & ~(1 << REFS1);
	
	/*Set the analog input to be read (start with ADC3) */
	ADMUX |= (1 << MUX1); // | (1 << MUX0);
	
	/*Set the ADC to free-running mode (auto-trigger) */
	ADCSRA |= (1 << ADATE);
	
	/*for now, left adjust output data so we only need to read 1 register (now only 8 bits) */
	ADMUX |= (1 << ADLAR);
	
	
}

void enable_adc(void){
	/*enable the ADC*/
	ADCSRA |= (1 << ADEN);
	
	/*start ADC conversions*/
	ADCSRA |= (1 << ADSC);
	
}

void disable_adc(void){
	/*disable the adc*/
	ADCSRA &= ~(1 << ADEN);
	
}