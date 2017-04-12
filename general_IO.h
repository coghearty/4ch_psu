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
void toggle_green_led(void);
void red_led(uint8_t on);
void toggle_red_led(void);

/*------ CHANNEL FUNCTIONS ------*/
void CH1_boost(uint8_t on);
void CH2_boost(uint8_t on);
void CH3_boost(uint8_t on);
void CH4_boost(uint8_t on);
void EN_power(uint8_t on);
void EN_wifi(uint8_t on);

/*------ ENCODER FUNCTIONS ------*/
//#define ENC_PORT PORTB
//#define ENC_DDR  DDRB
#define ENC_A_MSK 0x01
#define ENC_B_MSK 0x02
#define ENC_CCW 0
#define ENC_CW 1
#define ENC_FAIL 2
#define ENCODER_STATUS_FAULT 0x01
#define MAX_ENCODER_POS	0xFF

volatile uint8_t encoder1_direction;
volatile uint8_t encoder1_count;
volatile uint8_t encoder1_updated;

volatile uint8_t encoder2_direction;
volatile uint8_t encoder2_count;
volatile uint8_t encoder2_updated;

void init_encoders(void);
void get_enc1_mV(uint16_t *enc1_mV);

#endif /* GENERAL_IO_H_ */