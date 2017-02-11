/*
 * PCB_leds.c
 *
 * Created: 4/02/2017 5:36:01 PM
 *  Author: NormalUser
 */ 
#include "general_IO.h"
#include "hardware.h"
#include <stdint.h>
#include <avr/io.h>

/*------ LED FUNCTIONS ------*/
void CH1_led(uint8_t on){
	if(!on)
	PORTA &= ~CH1_EN;
	else
	PORTA |= CH1_EN;
}

void CH2_led(uint8_t on){
	if(!on)
	PORTA &= ~CH2_EN;
	else
	PORTA |= CH2_EN;
}

void CH3_led(uint8_t on){
	if(!on)
	PORTC &= ~CH3_EN;
	else
	PORTC |= CH3_EN;
}

void CH4_led(uint8_t on){
	if(!on)
	PORTC &= ~CH4_EN;
	else
	PORTC |= CH4_EN;
}

void green_led(uint8_t on){
	if(!on)
	PORTC |= LED_GRN;
	else
	PORTC &= ~LED_GRN;
}

void red_led(uint8_t on){
	if(!on)
	PORTC |= LED_RED;
	else
	PORTC &= ~LED_RED;
}

/*------ CHANNEL FUNCTIONS ------*/
void CH1_boost(uint8_t on){
	if(!on)
	PORTA &= ~CH1_BOOST;
	else
	PORTA |= CH1_BOOST;
}

void CH2_boost(uint8_t on){
	if(!on)
	PORTA &= ~CH2_BOOST;
	else
	PORTA |= CH2_BOOST;
}

void CH3_boost(uint8_t on){
	if(!on)
	PORTA &= ~CH3_BOOST;
	else
	PORTA |= CH3_BOOST;
}

void CH4_boost(uint8_t on){
	if(!on)
	PORTC &= ~CH4_BOOST;
	else
	PORTC |= CH4_BOOST;
}

void EN_power(uint8_t on){
	if(!on)
	PORTA &= ~EN_PWR;
	else
	PORTA |= EN_PWR;
}

void EN_wifi(uint8_t on){
	if(!on)
	PORTC &= ~WIFI_EN;
	else
	PORTC |= WIFI_EN;
}