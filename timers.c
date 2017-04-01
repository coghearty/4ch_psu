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

/*TIMER1 is configured as a basic 1 second timer*/
static void init_timer1(void){
	TCCR1A = 0x00;
	TCCR1B |= (1<<CS01) | (1<<CS00);	//set pre-scaler to 64 (at 1MHz, 64us per tick)
	OCR1A = 15625;	//set the compare register. This many ticks corresponds to 1000ms.
	TIMSK |= (1<<OCIE1A);	//enable compare A interrupt
	TCNT1 = 0x00;	//reset count to zero

}

void init_timers(void){
	//init_timer0();
	init_timer1();
}

/*-------- INTERRUPT SERVICE ROUTINES --------*/
ISR (TIMER0_OVF_vect){
	TCNT0 = 0x00;
	timer0_triggered = 1;
}

ISR (TIMER1_COMPA_vect){
	TCNT1 = 0x00;
	timer1_triggered = 1;
}
