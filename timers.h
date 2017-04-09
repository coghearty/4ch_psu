/*
 * timers.h
 *
 * Created: 1/04/2017 3:58:53 PM
 *  Author: NormalUser
 */ 


#ifndef TIMERS_H_
#define TIMERS_H_

void init_timers(void);
void set_PWM_CH1(uint8_t duty_cycle);
void set_PWM_CH2(uint8_t duty_cycle);
void set_PWM_CH3(uint8_t duty_cycle);
void set_PWM_CH4(uint8_t duty_cycle);

volatile uint8_t timer0_triggered;
volatile uint8_t timer3_triggered;

volatile int seconds_count;
volatile int minutes_count;
volatile int hours_count;
volatile int dayofweek_count;
volatile uint8_t minute_elapsed;

#define MAX_PWM	0xFF

#endif /* TIMERS_H_ */