/*
 * power_control.h
 *
 * Created: 12/04/2017 11:42:42 AM
 *  Author: NormalUser
 */ 


#ifndef POWER_CONTROL_H_
#define POWER_CONTROL_H_



#define CHANNEL_1		0
#define CHANNEL_2		1
#define CHANNEL_3		2
#define CHANNEL_4		3

#define MAX_PWM_MILLIAMPS	1000

/*Calibration Values*/
#define cal_CH1_mA_x		0.25
#define cal_CH1_mA_y		0
#define cal_CH2_mA_x		0.25
#define cal_CH2_mA_y		0
#define cal_CH3_mA_x		0.25
#define cal_CH3_mA_y		0
#define cal_CH4_mA_x		0.25
#define cal_CH4_mA_y		0

/*Individual LED maximums*/
/*Note these are all 100mV less than abs max for safety*/
#define CH1_R_MAX_MV	2500
#define CH2_G_MAX_MV	3800
#define CH3_B_MAX_MV	3600
#define CH4_W_MAX_MV	3600
#define RGBW_MAX_MA		350

/*Individual LED nominal values*/
#define CH1_R_NOM_MV	2200
#define CH2_G_NOM_MV	3300
#define CH3_B_NOM_MV	3100
#define CH4_W_NOM_MV	3100

/*Individual top mA values to give desired spectrum*/
#define CH1_R_NOM_MA	350
#define CH2_G_NOM_MA	350
#define CH3_B_NOM_MA	350
#define CH4_W_NOM_MA	350

void set_PWM_mA(unsigned char channel, uint16_t milliAmps);
void set_all_power_zero();


#endif /* POWER_CONTROL_H_ */