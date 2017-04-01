/*
 * timers.h
 *
 * Created: 1/04/2017 3:58:53 PM
 *  Author: NormalUser
 */ 


#ifndef TIMERS_H_
#define TIMERS_H_

void init_timers(void);

volatile int timer0_triggered;
volatile int timer1_triggered;

#endif /* TIMERS_H_ */