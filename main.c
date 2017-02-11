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

#define DELAY_TIME 1000

#include "hardware.h"
#include "general_IO.h"
#include "usart.h"
#include "i2c_comms.h"



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
 	
 	DDRD |= SW_AUTO | SW_OFF | SW_ON | PB_SNOOZE;
 	PORTD = SW_AUTO | SW_OFF | SW_ON | PB_SNOOZE;	//set input pull-up resistors
	 
	DDRE |= SW_ENC1 | SW_ENC2 | ENC1B | ENC1A | ENC2B | ENC2A;
	PORTE |= SW_ENC1 | SW_ENC2;
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
	init_i2c();
	int rv;
#define CH1_DAC_ID    0x90        // I2C 24AA128 EEPROM Device Identifier
#define CH1_DAC_ADDR  0x06        // I2C 24AA128 EEPROM Device Address
#define CH1_DA_ADDR		0x4E

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
	
	red_led(1);
	_delay_ms(500);
	red_led(0);
	_delay_ms(500);	
	green_led(1);
	_delay_ms(500);
	green_led(0);
	_delay_ms(500);

char buffer[2]= {0b00000000,0b11110000};
	
    while (1) 
    {

		CH1_led(1);
		_delay_ms(DELAY_TIME);
		CH2_led(1);
		//EN_power(1);
		_delay_ms(DELAY_TIME);
		CH3_led(1);
		_delay_ms(DELAY_TIME);
		CH4_led(1);
		_delay_ms(DELAY_TIME);	
		green_led(1);
		_delay_ms(DELAY_TIME);
		red_led(1);
		_delay_ms(DELAY_TIME);
		CH1_led(0);
		CH2_led(0);
		CH3_led(0);
		CH4_led(0);
		green_led(0);
		red_led(0);
puts("Hello world!");
		rv = i2c_writebyte(CH1_DA_ADDR,buffer[0],buffer[1]);
printf("i2c write return: %i\n", rv);		
//USART_putchar(sendData);
		
		       // input = getchar();
		        //printf("You wrote %c\n", input);
		_delay_ms(2000);
		

    }
	
	return 0;
}

