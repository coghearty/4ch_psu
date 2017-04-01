/*
* 4CH_PowerSupply.c
*
* Created: 4/02/2017 12:19:54 PM
* Author : NormalUser
*/

#define F_CPU 1000000    // 1MHz AVR clock frequency in Hz, used by util/delay.h
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/interrupt.h>

#define DELAY_TIME 1000

#include "hardware.h"
#include "general_IO.h"
#include "usart.h"
#include "i2c_comms.h"
#include "adc.h"
#include "timers.h"

static void init_ports(void){
	PORTA = 0x00;
	DDRA = EN_PWR | CH1_BOOST | CH1_EN | CH2_BOOST | CH2_EN | CH3_BOOST;	//set direction register
	PORTA = CH1_ILIM | CH2_ILIM;	//set internal pull-up resistors for inputs
	
	DDRB = CH1_ISET | CH2_ISET | CH3_ISET | CH4_ISET;
	PORTB = 0x00;
	
	PORTC = 0x00;
	DDRC |= WIFI_EN | LED_GRN | LED_RED | CH4_EN | CH4_BOOST | CH3_EN;
	PORTC = LED_GRN | LED_RED;		//set initial output state high
	PORTC |= CH4_ILIM | CH3_ILIM;	//set input pull-up resistors
	
	DDRD &= ~SW_AUTO & ~SW_OFF & ~SW_ON & ~PB_SNOOZE;	//set direction to inputs
	PORTD = SW_AUTO | SW_OFF | SW_ON | PB_SNOOZE;	//set input pull-up resistors
	

}

static void set_initial_IO_states(void){
	/*start with all LEDs off*/
	CH1_led(0);
	CH2_led(0);
	CH3_led(0);
	CH4_led(0);
	/*ensure all boost modes are off*/
	CH1_boost(0);
	CH2_boost(0);
	CH3_boost(0);
	CH4_boost(0);
	/*ensure main power is disabled*/
	EN_power(0);
	/*ensure wifi is off*/
	EN_wifi(0);
}



int main(void)
{
	unsigned char ResetSrc = MCUCSR;   // save reset source
	MCUCSR = 0x00;  // cleared for next reset detection

	init_ports();
	set_initial_IO_states();
	
	init_PC_serial(MYUBRR);
	stdout = &usart_output;
	stdin  = &usart_input;
	char input;
	/*print out the last reset cause*/
	if (ResetSrc & PORF)
	puts("PWR RESET\n\r");
	if (ResetSrc & EXTRF)
	puts("EXT RESET\n\r");
	if (ResetSrc & BORF)
	puts("BOD RESET\n\r");
	if (ResetSrc & WDRF)
	puts("WDT RESET\n\r");
	
	
	init_i2c();
	
	init_adc();
	enable_adc();
	
	init_timers();
	init_encoders();
	
	int rv;
	timer0_triggered = 0;
	timer1_triggered = 0;

	
	red_led(1);
	_delay_ms(500);
	red_led(0);
	_delay_ms(500);
	green_led(1);
	_delay_ms(500);
	green_led(0);
	_delay_ms(500);
	
	//PORTB |= CH1_ISET;
	PORTB |= CH2_ISET;

	uint16_t v_data = 2048; //4094; //0x7FF;
	uint16_t rcv_data = 0x0000;
	uint8_t adc_L = 0;
	uint8_t adc_H = 0;
	uint16_t adc_raw = 0;
	
	//EN_power(1);
	

	sei();	//enable global interrupts
	while (1)
	{
		
		/*if(PIND & PB_SNOOZE)
		CH4_led(0);
		else
		CH4_led(1);
		
		if(PIND & SW_AUTO)
		CH1_led(0);
		else
		CH1_led(1);
		
		if(PIND & SW_OFF)
		CH2_led(0);
		else
		CH2_led(1);
		
		if(PIND & SW_ON)
		CH3_led(0);
		else
		CH3_led(1);*/

		
		



		
		//printf("i2c write return: %i\n", rv);
		

		//USART_putchar(sendData);
		
		// input = getchar();
		//printf("You wrote %c\n", input);
		//_delay_ms(2000);
		

		if(timer0_triggered){
			toggle_green_led();
			timer0_triggered = 0;
		}
		
		/*1 second timer heartbeat*/
		if(timer1_triggered){
			toggle_red_led();
			timer1_triggered = 0;
			puts("Hello World!");
			rv = read_DAC_mV(CHANNEL_1,&CH1_DAC_read_mV);
			//printf("i2c read return: %i\n", rv);
			
			rv = set_DAC_mV(CHANNEL_1,500);
			rv = read_DAC_mV(CHANNEL_2,&CH2_DAC_read_mV);
			
			rv = set_DAC_mV(CHANNEL_2,100);
			rv = read_DAC_mV(CHANNEL_3,&CH3_DAC_read_mV);
			
			rv = set_DAC_mV(CHANNEL_3,500);
			rv = read_DAC_mV(CHANNEL_4,&CH4_DAC_read_mV);

			rv = set_DAC_mV(CHANNEL_4,500);
			/*printf("CH1 read data: %u\n", CH1_DAC_read_mV);
			printf("CH2 read data: %u\n", CH2_DAC_read_mV);
			printf("CH3 read data: %u\n", CH3_DAC_read_mV);
			printf("CH4 read data: %u\n", CH4_DAC_read_mV);*/
			
			adc_L = ADCL;
			adc_H = ADCH;
			adc_raw = (adc_H << 8) | (adc_L & 0xFF);
			/*printf("CH0 raw adc_H: %u\n", adc_H);
			printf("CH0 raw adc_L: %u\n", adc_L);*/
			printf("CH0 raw adc: %u\n", adc_raw);
			printf("ENC1 count: %u\n", encoder1_count);
		}
		

	}
	
	return 0;
}

