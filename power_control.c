/*
* power_control.c
*
* Created: 12/04/2017 11:42:25 AM
*  Author: NormalUser
*/

#include <stdint.h>
#include "power_control.h"

void set_PWM_mA(unsigned char channel, uint16_t milliAmps){
	if(milliAmps > MAX_PWM_MILLIAMPS)
	milliAmps = MAX_PWM_MILLIAMPS;
	
	switch(channel) {
		case CHANNEL_1:
		set_PWM_CH1((uint8_t) round((milliAmps*cal_CH1_mA_x) + cal_CH1_mA_y));
		break;
		case CHANNEL_2:
		set_PWM_CH2((uint8_t) round(milliAmps*cal_CH2_mA_x) + cal_CH2_mA_y);
		break;
		case CHANNEL_3:
		set_PWM_CH3((uint8_t) round(milliAmps*cal_CH3_mA_x) + cal_CH3_mA_y);
		break;
		case CHANNEL_4:
		set_PWM_CH4((uint8_t) round(milliAmps*cal_CH4_mA_x) + cal_CH4_mA_y);
		break;
	}
}

void set_all_power_zero(){
	set_DAC_mV(CHANNEL_1,0);
	set_DAC_mV(CHANNEL_2,0);
	set_DAC_mV(CHANNEL_3,0);
	set_DAC_mV(CHANNEL_4,0);
	
	set_PWM_mA(CHANNEL_1,0);
	set_PWM_mA(CHANNEL_2,0);
	set_PWM_mA(CHANNEL_3,0);
	set_PWM_mA(CHANNEL_4,0);
}