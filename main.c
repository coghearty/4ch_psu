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
	
	DDRB = CH2_ISET | CH3_ISET | CH4_ISET;
	PORTB = 0x00;	//set all outputs to zero
	
	PORTC = 0x00;
	DDRC |= WIFI_EN | LED_GRN | LED_RED | CH4_EN | CH4_BOOST | CH3_EN;
	PORTC = LED_GRN | LED_RED;		//set initial output state high
	PORTC |= CH4_ILIM | CH3_ILIM;	//set input pull-up resistors
	
	DDRD &= ~SW_AUTO & ~SW_OFF & ~SW_ON & ~PB_SNOOZE;	//set direction to inputs
	PORTD = SW_AUTO | SW_OFF | SW_ON | PB_SNOOZE;	//set input pull-up resistors
	
	DDRE |= CH1_ISET;
	PORTE &= ~(1<<CH1_ISET);	//set output to zero
	
	DDRF = 0x00;	//set all as inputs
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
	//timer3_triggered = 0;

	
	red_led(1);
	_delay_ms(500);
	red_led(0);
	_delay_ms(500);
	green_led(1);
	_delay_ms(500);
	green_led(0);
	_delay_ms(500);

	uint16_t v_data = 2048; //4094; //0x7FF;
	uint16_t rcv_data = 0x0000;
	uint8_t adc_L = 0;
	uint8_t adc_H = 0;
	uint16_t adc_raw = 0;
	uint8_t adc_channel = 0;
	uint16_t enc1_mV = 0;


	sei();	//enable global interrupts
	while (1)
	{
		
		if(encoder1_updated){
			encoder1_updated = 0;
			get_enc1_mV(&enc1_mV);
			rv = set_DAC_mV(CHANNEL_2,enc1_mV);
			rv = set_DAC_mV(CHANNEL_1,enc1_mV);
			rv = set_DAC_mV(CHANNEL_3,enc1_mV);
			rv = set_DAC_mV(CHANNEL_4,enc1_mV);
			printf("CH2 mV data: %u\n", enc1_mV);
		}
		
		if(encoder2_updated){
			encoder2_updated = 0;
			set_PWM_CH1(encoder2_count);
			set_PWM_CH2(encoder2_count);
			set_PWM_CH3(encoder2_count);
			set_PWM_CH4(encoder2_count);
			//printf("enc2 updated\n");
		}
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
		*/
		if(PIND & SW_ON)
		EN_power(1);
		else
		EN_power(0);

		
		



		
		//printf("i2c write return: %i\n", rv);
		

		//USART_putchar(sendData);
		
		// input = getchar();
		//printf("You wrote %c\n", input);
		//_delay_ms(2000);
		

		/*if(timer0_triggered){
			toggle_red_led();
			timer0_triggered = 0;
		}*/
		
		if(minute_elapsed){
			toggle_red_led();
			minute_elapsed = 0;
		}
		
		/*1 second timer heartbeat*/
		if(timer0_triggered){
			toggle_green_led();
			timer0_triggered = 0;
			//puts("Hello World!");
			//rv = read_DAC_mV(CHANNEL_1,&CH1_DAC_read_mV);
			//printf("i2c read return: %i\n", rv);
			
			//rv = set_DAC_mV(CHANNEL_1,500);
			//rv = read_DAC_mV(CHANNEL_2,&CH2_DAC_read_mV);
			
			//rv = set_DAC_mV(CHANNEL_2,100);
			//rv = read_DAC_mV(CHANNEL_3,&CH3_DAC_read_mV);
			
			//rv = set_DAC_mV(CHANNEL_3,500);
			//rv = read_DAC_mV(CHANNEL_4,&CH4_DAC_read_mV);

			//rv = set_DAC_mV(CHANNEL_4,500);
			/*printf("CH1 read data: %u\n", CH1_DAC_read_mV);
			printf("CH2 read data: %u\n", CH2_DAC_read_mV);
			printf("CH3 read data: %u\n", CH3_DAC_read_mV);
			printf("CH4 read data: %u\n", CH4_DAC_read_mV);*/
			
			//adc_L = ADCL;
			//adc_H = ADCH;
			//adc_raw = (adc_H << 8) | (adc_L & 0xFF);
			for(uint8_t i=0; i<8; i++){
				adc_raw = read_ADC(i);
				printf("adc: %u\n",adc_raw);
			}
			//adc_raw = read_ADC(CH4_VSENSE);
			/*printf("CH0 raw adc_H: %u\n", adc_H);
			printf("CH0 raw adc_L: %u\n", adc_L);*/
			printf("CH0 raw adc: %u\n", adc_raw);
			//printf("ENC1 count: %u\n", encoder1_count);
			//printf("ENC2 count: %u\n", encoder2_count);
			printf("%u :", dayofweek_count);
			printf(" %u :", hours_count);
			printf(" %u :", minutes_count);
			printf(" %u\n", seconds_count);
			//printf("ENC1: %u, ENC2: %u\n", encoder2_count, encoder1_count);
			

		}
		

	}
	
	return 0;
}

