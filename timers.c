/*
* timers.c
*
* Created: 1/04/2017 3:58:34 PM
*  Author: NormalUser
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include "timers.h"

static void init_timer0(void){
	TCNT0 = 0x00;	//reset the timer to zero
	TCCR0 |= (1<<CS02) | (1<<CS01) | (1<<CS00); //set prescaler of 1024
	TIMSK |= (1<<TOIE0);	//enable overflow interrupt
}

/*TIMER1 is configured as a PWM output*/
static void init_timer1(void){
	//TCCR1A |= (1<<COM1A1) | (1<<COM1A0) |(1<<WGM11);	//set on match when up-counting, clear on down-counting
	TCCR1A |= (1<<COM1A1) | (1<<WGM10);
	TCCR1B |= (1<<CS10) | (1<<WGM12);
	//TCCR1B |= (1<<CS10) | (1<<WGM13) | (1<<WGM12);	//set pre-scaling to 1, and set to phase correct PWM
	//ICR1 = 20000;
	OCR1A = 128;
	
}

/*TIMER3 is configured as a basic 1 second timer*/
static void init_timer3(void){
	TCCR3A = 0x00;
	TCCR3B |= (1<<CS31) | (1<<CS30);	//set pre-scaler to 64 (at 1MHz, 64us per tick)
	OCR3A = 15625;	//set the compare register. This many ticks corresponds to 1000ms.
	//TIMSK |= (1<<OCIE1A);	//enable compare A interrupt
	ETIMSK |= (1<<OCIE3A);
	TCNT3 = 0x00;	//reset count to zero

}

void init_timers(void){
	//init_timer0();
	init_timer1();
	init_timer3();
}

void set_PWM_Timer1(uint8_t duty_cycle){
	if(duty_cycle > MAX_PWM)
		duty_cycle = MAX_PWM;
	
	OCR1A = duty_cycle;
}

/*-------- INTERRUPT SERVICE ROUTINES --------*/
ISR (TIMER0_OVF_vect){
	TCNT0 = 0x00;
	timer0_triggered = 1;
}

ISR (TIMER3_COMPA_vect){
	TCNT3 = 0x00;
	timer3_triggered = 1;
}
