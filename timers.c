/*
* timers.c
*
* Created: 1/04/2017 3:58:34 PM
*  Author: NormalUser
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "timers.h"

volatile int seconds_count;
volatile int minutes_count;
volatile int hours_count;
volatile int dayofweek_count;

static void init_timer0(void){
	/*TCNT0 = 0x00;	//reset the timer to zero
	TCCR0 |= (1<<CS02) | (1<<CS01) | (1<<CS00); //set prescaler of 1024
	TIMSK |= (1<<TOIE0);	//enable overflow interrupt
	*/
	
	/*Configure timer0 to use external 32.768kHz crystal*/
	seconds_count = 40;
	minutes_count = 0;
	hours_count = 0;
	dayofweek_count = 0;
	
	/*clear OCIE0 and TOIE0 to disable interrupts before changing clock source*/
	TIMSK = 0x00;
	/*change clock source to external crystal*/
	ASSR |= (1<<AS0);
	
	/*set prescale to 128*/
	TCCR0 = (1<<CS02) | (1<<CS00);
	/*reset clock count to zero*/
	TCNT0 = 0x00;
	//OCR0 output compare register not used.
	
	while((TCN0UB==1) || (TCR0UB==1)){
		//wait until TCNT0 and TCCR0 have been updated from the temporary register.
	}
	
	/*Clear the Timer/Counter0 interrupt flags*/
	TIFR = (1<<OCF0) | (1<<TOV0);
	/*Enable interrupts*/
	TIMSK = (1<<TOIE0);	//enable overflow interrupt
}

/*TIMER1 is configured as a PWM output for CH2, CH3 & CH4 I-set*/
static void init_timer1(void){
	/*Set compare output mode for channels A,B,C to "Clear on compare match."*/
	/*Set mode to Fast PWM 10-bit (WGM10 and WGM12)*/
	TCCR1A = (1<<COM1A1) | (1<<COM1B1) | (1<<COM1C1) | (1<<WGM10);
	//TCCR1A = (1<<COM1A1) | (1<<COM1B1) | (1<<COM1C1) | (1<<WGM10) | (1<<WGM11);
	TCCR1B |= (1<<CS10) | (1<<WGM12);	//set to no prescale

	OCR1A = 0;
	OCR1B = 0;
	OCR1C = 0;
	
}

/*TIMER3 is configured as a PWM output for CH1 I-set*/
static void init_timer3(void){
	/*Set compare output mode for channel A to "Clear on compare match."*/
	/*Set mode to PWM 8-bit (WGM10 and WGM12)*/
	TCCR3A = (1<<COM3A1) | (1<<WGM30);
	//set mode to Fast PWM 10bit (WGM30, WGM31 & WGM32)
	//TCCR3A = (1<<COM3A1) | (1<<WGM31) | (1<<WGM30);
	TCCR3B |= (1<<CS30) | (1<<WGM32);	//set to no prescale

	OCR3A = 0;

}

/*TIMER3 is configured as a basic 1 second timer*/
/*static void init_timer3(void){
TCCR3A = 0x00;
TCCR3B |= (1<<CS31) | (1<<CS30);	//set pre-scaler to 64 (at 1MHz, 64us per tick)
//OCR3A = 15625;	//set the compare register. This many ticks corresponds to 1000ms.
OCR3A = 15300;
//TIMSK |= (1<<OCIE1A);	//enable compare A interrupt
ETIMSK |= (1<<OCIE3A);
TCNT3 = 0x00;	//reset count to zero
seconds_count = 0;
minutes_count = 0;
hours_count = 0;
}*/

void init_timers(void){
	init_timer0();	//RTC timekeeper
	init_timer1();	//PWM for CH2,CH3,CH4 Iset
	init_timer3();	//PWM for CH1 Iset
}

void set_PWM_CH1(uint8_t duty_cycle){
	if(duty_cycle > MAX_PWM_8BIT)
	duty_cycle = MAX_PWM_8BIT;

	OCR3A = duty_cycle;
}

void set_PWM_CH2(uint8_t duty_cycle){
	if(duty_cycle > MAX_PWM_8BIT)
	duty_cycle = MAX_PWM_8BIT;
	
	OCR1A = duty_cycle;
}

void set_PWM_CH3(uint8_t duty_cycle){
	if(duty_cycle > MAX_PWM_8BIT)
	duty_cycle = MAX_PWM_8BIT;
	
	OCR1B = duty_cycle;
}

void set_PWM_CH4(uint8_t duty_cycle){
	if(duty_cycle > MAX_PWM_8BIT)
	duty_cycle = MAX_PWM_8BIT;

	OCR1C = duty_cycle;
}

void print_current_time(){
	printf("%u :", dayofweek_count);
	printf(" %u :", hours_count);
	printf(" %u :", minutes_count);
	printf(" %u\n", seconds_count);
}


/*-------- INTERRUPT SERVICE ROUTINES --------*/
ISR (TIMER0_OVF_vect){
	timer0_triggered = 1;
	seconds_count++;
	if(seconds_count == 60){
		minutes_count++;
		seconds_count = 0;
		minute_elapsed = 1;
	}
	if(minutes_count == 60){
		hours_count++;
		minutes_count = 0;
	}
	if(hours_count == 24){
		dayofweek_count++;
		hours_count = 0;
	}
	if(dayofweek_count == 7){
		dayofweek_count = 0;
	}
}
/*
ISR (TIMER3_COMPA_vect){
TCNT3 = 0x00;
timer3_triggered = 1;
seconds_count++;
if(seconds_count == 60){
minutes_count++;
seconds_count = 0;
minute_elapsed = 1;
}
if(minutes_count == 60){
hours_count++;
minutes_count = 0;
}
if(hours_count == 24){
dayofweek_count++;
hours_count = 0;
}
if(dayofweek_count == 7){
dayofweek_count = 0;
}
}*/
