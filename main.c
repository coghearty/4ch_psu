/*
* 4CH_PowerSupply.c
*
* Created: 4/02/2017 12:19:54 PM
* Author : NormalUser
*/

#define F_CPU 1000000    // 1MHz AVR clock frequency in Hz, used by util/delay.h
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/interrupt.h>

#include "hardware.h"
#include "general_IO.h"
#include "usart.h"
#include "i2c_comms.h"
#include "adc.h"
#include "timers.h"
#include "power_control.h"

#define DELAY_TIME 1000

/*LED wakeup mode states*/
#define RGBW_OFF			0
#define RGBW_ON				1
#define RGBW_AUTO			2

/*States within auto mode*/
#define RGBW_AUTO_IDLE		0
#define RGBW_AUTO_RAMP		1
#define RGBW_AUTO_FINISH	2
#define RGBW_AUTO_TIMEOUT	3

//void usart_process_command();

static void init_ports(void){
	PORTA = 0x00;
	DDRA = EN_PWR | CH1_BOOST | CH1_EN | CH2_BOOST | CH2_EN | CH3_BOOST;	//set direction register
	PORTA = CH1_ILIM | CH2_ILIM;	//set internal pull-up resistors for inputs
	
	DDRB = CH2_ISET | CH3_ISET | CH4_ISET;
	PORTB = 0x00;	//set all outputs to zero
	
	PORTC = 0x00;
	DDRC |= WIFI_EN | LED_GRN | LED_RED | CH4_EN | CH4_BOOST | CH3_EN;
	PORTC = LED_GRN | LED_RED;		//set initial output state high
	PORTC |= CH4_ILIM | CH3_ILIM;	//set input pull-up resistors
	
	DDRD &= ~SW_AUTO & ~SW_OFF & ~SW_ON & ~PB_SNOOZE;	//set direction to inputs
	PORTD = SW_AUTO | SW_OFF | SW_ON | PB_SNOOZE;	//set input pull-up resistors
	
	DDRE |= CH1_ISET;
	PORTE &= ~(1<<CH1_ISET);	//set output to zero
	
	DDRF = 0x00;	//set all as inputs
}

static void set_initial_IO_states(void){
	/*start with all LEDs off*/
	CH1_led(0);
	CH2_led(0);
	CH3_led(0);
	CH4_led(0);
	/*ensure all boost modes are off*/
	CH1_boost(0);
	CH2_boost(0);
	CH3_boost(0);
	CH4_boost(0);
	/*ensure main power is disabled*/
	EN_power(0);
	/*ensure wifi is off*/
	EN_wifi(0);
}



int main(void)
{
	unsigned char ResetSrc = MCUCSR;   // save reset source
	MCUCSR = 0x00;  // cleared for next reset detection

	init_ports();
	set_initial_IO_states();
	
	init_PC_serial(MYUBRR);
	stdout = &usart_output;
	stdin  = &usart_input;
	char input;
	/*print out the last reset cause*/
	if (ResetSrc & PORF)
	puts("PWR RESET\n");
	if (ResetSrc & EXTRF)
	puts("EXT RESET\n");
	if (ResetSrc & BORF)
	puts("BOD RESET\n");
	if (ResetSrc & WDRF)
	puts("WDT RESET\n");
	
	
	init_i2c();
	
	init_adc();
	//enable_adc();
	
	init_timers();
	init_encoders();
	
	int rv;
	timer0_triggered = 0;


	
	red_led(1);
	_delay_ms(500);
	red_led(0);
	_delay_ms(500);
	green_led(1);
	_delay_ms(500);
	green_led(0);
	_delay_ms(500);


	uint16_t adc_raw = 0;
	uint16_t enc1_mV = 0;
	
	uint8_t rgbw_state = RGBW_OFF;
	uint8_t rgbw_state_old = 100;
	uint8_t rgbw_auto_state = RGBW_AUTO_IDLE;
	uint8_t rgbw_auto_state_old = 100;

	uint16_t wakeup_duration_s = 1800;
	uint16_t wakeup_t_elapsed_s = 0;
	uint16_t flashing_timeout_s = 65;
	uint16_t flashing_elapsed_s = 0;
	/*float mA_per_second_ramp_R = 0;
	float mA_per_second_ramp_G = 0;
	float mA_per_second_ramp_B = 0;
	float mA_per_second_ramp_W = 0;*/
	uint16_t mV_ramp_sp_R = 0;
	uint16_t mV_ramp_sp_G = 0;
	uint16_t mV_ramp_sp_B = 0;
	uint16_t mV_ramp_sp_W = 0;
	uint16_t mA_ramp_sp_R = 0;
	uint16_t mA_ramp_sp_G = 0;
	uint16_t mA_ramp_sp_B = 0;
	uint16_t mA_ramp_sp_W = 0;
	uint8_t voltage_ramp_mode = 1;
	uint8_t voltage_ramp_freq = 10;

	unsigned int CH1_DAC_read_mV = 0;
	
	uint8_t CH1_I_limiting = 0;
	uint8_t CH4_I_limiting = 0;
	
	hours_count = 17;
	minutes_count = 28;
	wakeup_h = 5;
	wakeup_m = 15;

	sei();	//enable global interrupts
	while (1)
	{

		if((PIND & SW_AUTO) && (PIND & SW_OFF)){
			rgbw_state = RGBW_ON;
		}
		
		if((PIND & SW_AUTO) && (PIND & SW_ON)){
			rgbw_state = RGBW_OFF;
		}

		if((PIND & SW_ON) && (PIND & SW_OFF)){
			rgbw_state = RGBW_AUTO;
		}
		
		switch(rgbw_state){
			case RGBW_OFF:
			if(rgbw_state != rgbw_state_old){
				//we only want to do this once on change of state
				rgbw_state_old = rgbw_state;
				set_all_power_zero();
				EN_power(0);
				disable_adc();
				printf("STATE changed to OFF state\n");
			}
			break;
			
			case RGBW_ON:
			if(rgbw_state != rgbw_state_old){
				//we only do this once on change of state
				rgbw_state_old = rgbw_state;
				encoder1_count = 0;
				encoder2_count = 0;
				set_all_power_zero();
				
				enable_adc();
				EN_power(1);
				printf("STATE changed to ON state\n");
			}
			
			/*We do the following every loop*/
			if(encoder1_updated){
				encoder1_updated = 0;
				get_enc1_mV(&enc1_mV);
				rv = set_DAC_mV(CHANNEL_1,enc1_mV);
				rv = set_DAC_mV(CHANNEL_2,enc1_mV);
				//rv = set_DAC_mV(CHANNEL_3,enc1_mV);
				rv = set_DAC_mV(CHANNEL_4,enc1_mV);
				printf("CH2 mV data: %u\n", enc1_mV);
			}
			
			if(encoder2_updated){
				encoder2_updated = 0;
				set_PWM_mA(CHANNEL_1, encoder2_count*4);
				set_PWM_mA(CHANNEL_2, encoder2_count*4);
				set_PWM_mA(CHANNEL_4, encoder2_count*4);
				//set_PWM_CH1(encoder2_count);
				//set_PWM_CH2(encoder2_count);
				//set_PWM_CH3(encoder2_count);
				//set_PWM_CH4(encoder2_count);
				printf("enc2: %u\n", encoder2_count);
			}
			break;
			
			case RGBW_AUTO:
			if(rgbw_state != rgbw_state_old){
				//only do at first change of state
				rgbw_state_old = rgbw_state;
				rgbw_auto_state = RGBW_AUTO_IDLE;
				printf("STATE changed to Auto state\n");
			}
			break;
		}
		
		if(rgbw_state == RGBW_AUTO){
			switch(rgbw_auto_state){
				case RGBW_AUTO_IDLE:
				//We are in Auto mode, but waiting for the right time to turn on LEDs
				if(rgbw_auto_state != rgbw_auto_state_old){
					rgbw_auto_state_old = rgbw_auto_state;
					set_all_power_zero();
					EN_power(0);
					disable_adc();
					printf("STATE changed to Auto-Idle state\n");
				}
				
				//add day of week check here
				if(hours_count == wakeup_h){
					if(minutes_count == wakeup_m){
						rgbw_auto_state = RGBW_AUTO_RAMP;
					}
				}
				break;
				
				case RGBW_AUTO_RAMP:
				//Now we ramp up the LED brightness over the set time period
				if(rgbw_auto_state != rgbw_auto_state_old){
					rgbw_auto_state_old = rgbw_auto_state;
					set_all_power_zero();
					EN_power(1);
					enable_adc();
					wakeup_t_elapsed_s = 0;
					
					//Set all LED voltages to bare minimum
					mV_ramp_sp_R = CH1_R_MIN_MV;
					mV_ramp_sp_G = CH2_G_MIN_MV;
					mV_ramp_sp_B = CH3_B_MIN_MV;
					mV_ramp_sp_W = CH4_W_MIN_MV;
					set_DAC_mV(CHANNEL_1,mV_ramp_sp_R);
					set_DAC_mV(CHANNEL_2,mV_ramp_sp_G);
					set_DAC_mV(CHANNEL_3,0);	//disable blue for now
					set_DAC_mV(CHANNEL_4,mV_ramp_sp_W);
					
					//Set all current to just above the minimum of 30mA
					mA_ramp_sp_R = 72; //52
					mA_ramp_sp_G = 28;
					mA_ramp_sp_B = 0;
					mA_ramp_sp_W = 44;
					set_PWM_mA(CHANNEL_1,mA_ramp_sp_R);
					set_PWM_mA(CHANNEL_2,mA_ramp_sp_G);
					set_PWM_mA(CHANNEL_4,mA_ramp_sp_W);
					
					//set mode to voltage ramping
					voltage_ramp_mode = 1;
					voltage_ramp_freq = 10;
					CH1_I_limiting = 0;
					CH4_I_limiting = 0;
					
					/*mA_per_second_ramp_R = CH1_R_NOM_MA/wakeup_duration_s; //actually need a variable for each colour here.
					mA_per_second_ramp_G = CH2_G_NOM_MA/wakeup_duration_s;
					mA_per_second_ramp_B = CH3_B_NOM_MA/wakeup_duration_s;
					mA_per_second_ramp_W = CH4_W_NOM_MA/wakeup_duration_s;*/
					printf("STATE changed to Auto-ramp state\n");
				}
				
				if(wakeup_t_elapsed_s >= wakeup_duration_s){
					rgbw_auto_state = RGBW_AUTO_FINISH;
				}
				break;
				
				case RGBW_AUTO_FINISH:
				/*In this case we want to start flashing the LED*/
				if(rgbw_auto_state != rgbw_auto_state_old){
					rgbw_auto_state_old = rgbw_auto_state;
					printf("STATE changed to auto-finish\n");
				}
				
				if(flashing_elapsed_s >= flashing_timeout_s){
					/*LEDs have been on for too long. User is probably not home*/
					rgbw_auto_state = RGBW_AUTO_IDLE;
				}
				
			}
		}
		
		



		if(usart_command_ready){
			//toggle_red_led();
			usart_command_ready = 0;
			if(usart_command_overflow){
				usart_clear_command();
				printf("Command too long. Must be max 8 chars\n");
				usart_command_overflow = 0;
				}else{
				usart_copy_command();
				printf("Command entered: %s\n",command_in);
				usart_process_command();
			}
			
		}

		

		if(minute_elapsed){
			//toggle_red_led();
			minute_elapsed = 0;
		}
		
		/*1 second timer heartbeat*/
		if(timer0_triggered){
			timer0_triggered = 0;

			for(uint8_t i=0; i<8; i++){
				adc_raw = read_ADC(i);
				//printf("adc: %u\n",adc_raw);
			}

			//if(rgbw_state == RGBW_ON){
				toggle_green_led();
			//}
			print_current_time();			

			
			if(rgbw_state == RGBW_AUTO){
				if(rgbw_auto_state == RGBW_AUTO_RAMP){
					wakeup_t_elapsed_s++;
					

					//if both channels are already in current limit mode, there is no point in ramping slowly.
					if((CH1_I_limiting>3) && (CH4_I_limiting==3)){
						voltage_ramp_freq = 1;
						}else{
						
						if(!(PINA & CH1_ILIM)){
							CH1_I_limiting++;
						}
						
						if(!(PINC & CH4_ILIM)){
							CH4_I_limiting++;
						}
					}

					if(voltage_ramp_mode == 1){
						if((wakeup_t_elapsed_s % voltage_ramp_freq) == 0){
							printf("elapsed wakeup time: %u\n", wakeup_t_elapsed_s);
							if(mV_ramp_sp_R < CH1_R_NOM_MV){
								mV_ramp_sp_R = mV_ramp_sp_R + DAC_MIN_MV_STEP;
								set_DAC_mV(CHANNEL_1,mV_ramp_sp_R);
							}
							if(mV_ramp_sp_G < CH2_G_NOM_MV){
								mV_ramp_sp_G = mV_ramp_sp_G + DAC_MIN_MV_STEP;
								set_DAC_mV(CHANNEL_2,mV_ramp_sp_G);
								}else{
								voltage_ramp_mode = 0;
							}
							if(mV_ramp_sp_W < CH4_W_NOM_MV){
								mV_ramp_sp_W = mV_ramp_sp_W + DAC_MIN_MV_STEP;
								set_DAC_mV(CHANNEL_4,mV_ramp_sp_W);
								}else{
								//voltage_ramp_mode = 0;
							}
						}
						}else{
						if((wakeup_t_elapsed_s % 5) == 0){
							if(mA_ramp_sp_R < CH1_R_NOM_MA){
								mA_ramp_sp_R = mA_ramp_sp_R + PWM_MIN_MA_STEP;
								set_PWM_mA(CHANNEL_1,mA_ramp_sp_R);
								printf("mA setpoint R: %u\n",mA_ramp_sp_R);
							}
							if(mA_ramp_sp_G < CH2_G_NOM_MA){
								mA_ramp_sp_G = mA_ramp_sp_G + PWM_MIN_MA_STEP;
								set_PWM_mA(CHANNEL_2, mA_ramp_sp_G);
								printf("mA setpoint G: %u\n",mA_ramp_sp_G);
							}
							if(mA_ramp_sp_W < CH4_W_NOM_MA){
								mA_ramp_sp_W = mA_ramp_sp_W + PWM_MIN_MA_STEP;
								set_PWM_mA(CHANNEL_4, mA_ramp_sp_W);
								printf("mA setpoint W: %u\n",mA_ramp_sp_W);
							}
						}
					}

				}
				if(rgbw_auto_state == RGBW_AUTO_FINISH){
					flashing_elapsed_s++;
				}
			}
		}
		

	}
	
	return 0;
}

/*void usart_process_command(){	/*if(command_in[0]=='T'){		if(command_in[1]=='?'){			puts("dummy\n");		}	}else{		puts("command unknown\n");	}*/		/*switch(command_in[0]){		case 'G':			if(command_in[1] == '?'){					puts("dummy\n");			}else if(command_in[1] == '='){				puts("dummy2");			}			break;		default:			puts("Command not recognised\n");			break;	}}*/