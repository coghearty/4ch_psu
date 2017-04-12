/*
* PCB_leds.c
*
* Created: 4/02/2017 5:36:01 PM
*  Author: NormalUser
*/
//#include "general_IO.h"
#include "hardware.h"
#include "general_IO.h"
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>


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

void toggle_green_led(void){
	PORTC ^= LED_GRN;
}

void red_led(uint8_t on){
	if(!on)
	PORTC |= LED_RED;
	else
	PORTC &= ~LED_RED;
}

void toggle_red_led(void){
	PORTC ^= LED_RED;
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


void init_encoders(void){
	/*Set encoder signals as inputs and enable internal pull-ups*/
	DDRE &= ~SW_ENC1 & ~ENC1B & ~ENC1A & ~ENC2B & ~ENC2A;
	PORTE |= SW_ENC1;
	
	/*Configure first encoder*/
	EIMSK |= (1<<INT5) | (1<<INT4);       // Enable pin change interrupts 4&5 on port E.
	EICRB |= (1<< ISC41) | (1<<ISC51) | (1<<ISC50);	//INT4 falling edge trigger, INT5 rising edge.
	encoder1_count = 0;
	
	/*Configure second encoder*/
	EIMSK |= (1<<INT7) | (1<<INT6);       // Enable pin change interrupts 6&7 on port E.
	EICRB |= (1<< ISC61) | (1<<ISC71) | (1<<ISC70);	//INT6 falling edge trigger, INT7 rising edge.
	encoder2_count = 0;
}

void get_enc1_mV(uint16_t *enc1_mV){
	*enc1_mV = encoder1_count * 10;
}

/*--------------------------------------------*/
/*-------- INTERRUPT SERVICE ROUTINES --------*/

/*ENC1A*/
ISR(INT5_vect){
	encoder1_updated = 1;
	
	if (PINE & ENC1B){
		if (PINE & ENC1A){
			encoder1_direction = ENC_CW;
			if(encoder1_count < MAX_ENCODER_POS)
				++encoder1_count;
		}
		else{
			encoder1_direction = ENC_CCW;
			if(encoder1_count > 0)
				--encoder1_count;
		}
	}
	else{
		if (PINE & ENC1A){
			encoder1_direction = ENC_CCW;
			if(encoder1_count > 0)
				--encoder1_count;
		}
		else{
			encoder1_direction = ENC_CW;
			if(encoder1_count < MAX_ENCODER_POS)
				++encoder1_count;
		}
	}
}

/*ENC1B*/
ISR(INT4_vect){
	encoder1_updated = 1;
	
	if(PINE & ENC1B){
		if(PINE & ENC1A){
			encoder1_direction = ENC_CCW;
			if(encoder1_count > 0)
				--encoder1_count;
		}
		else{
			encoder1_direction = ENC_CW;
			if(encoder1_count < MAX_ENCODER_POS)
				++encoder1_count;
		}
	}
	else{
		if(PINE & ENC1A){
			encoder1_direction = ENC_CW;
			if(encoder1_count < MAX_ENCODER_POS)
				++encoder1_count;
		}
		else{
			encoder1_direction = ENC_CCW;
			if(encoder1_count > 0)
				--encoder1_count;
		}
	}
}


/*ENC2A*/
ISR(INT7_vect){
	encoder2_updated = 1;
	
	if (PINE & ENC2B){
		if (PINE & ENC2A){
			encoder2_direction = ENC_CW;
			if(encoder2_count < MAX_ENCODER_POS)
				++encoder2_count;
		}
		else{
			encoder2_direction = ENC_CCW;
			if(encoder2_count > 0)
				--encoder2_count;
		}
	}
	else{
		if (PINE & ENC2A){
			encoder2_direction = ENC_CCW;
			if(encoder2_count > 0)
				--encoder2_count;
		}
		else{
			encoder2_direction = ENC_CW;
			if(encoder2_count < MAX_ENCODER_POS)
				++encoder2_count;
		}
	}
}

/*ENC2B*/
ISR(INT6_vect){
	encoder2_updated = 1;
	
	if(PINE & ENC2B){
		if(PINE & ENC2A){
			encoder2_direction = ENC_CCW;
			if(encoder2_count > 0)
			--encoder2_count;
		}
		else{
			encoder2_direction = ENC_CW;
			if(encoder2_count < MAX_ENCODER_POS)
			++encoder2_count;
		}
	}
	else{
		if(PINE & ENC2A){
			encoder2_direction = ENC_CW;
			if(encoder2_count < MAX_ENCODER_POS)
			++encoder2_count;
		}
		else{
			encoder2_direction = ENC_CCW;
			if(encoder2_count > 0)
			--encoder2_count;
		}
	}
}