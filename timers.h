/*
 * timers.h
 *
 * Created: 1/04/2017 3:58:53 PM
 *  Author: NormalUser
 */ 


#ifndef TIMERS_H_
#define TIMERS_H_

void init_timers(void);
void set_PWM_Timer1(uint8_t duty_cycle);

volatile int timer0_triggered;
volatile int timer3_triggered;

#define MAX_PWM	0xFF

#endif /* TIMERS_H_ */