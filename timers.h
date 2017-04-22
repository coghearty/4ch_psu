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
extern void print_current_time();


volatile uint8_t timer0_triggered;

extern volatile int seconds_count;
extern volatile int minutes_count;
extern volatile int hours_count;
extern volatile int dayofweek_count;
volatile uint8_t minute_elapsed;

#define MAX_PWM_8BIT	0xFF
#define MAX_PWM_10BIT	0x3FF


#endif /* TIMERS_H_ */