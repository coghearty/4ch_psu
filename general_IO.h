/*
 * general_IO.h
 *
 * Created: 4/02/2017 5:36:33 PM
 *  Author: NormalUser
 */ 


#ifndef GENERAL_IO_H_
#define GENERAL_IO_H_

#include <stdint.h>
/*------ LED FUNCTIONS ------*/
void CH1_led(uint8_t on);

void CH2_led(uint8_t on);

void CH3_led(uint8_t on);

void CH4_led(uint8_t on);

void green_led(uint8_t on);

void red_led(uint8_t on);


/*------ CHANNEL FUNCTIONS ------*/
void CH1_boost(uint8_t on);

void CH2_boost(uint8_t on);

void CH3_boost(uint8_t on);

void CH4_boost(uint8_t on);

void EN_power(uint8_t on);

void EN_wifi(uint8_t on);

#endif /* GENERAL_IO_H_ */