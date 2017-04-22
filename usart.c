/*
* usart.c
*
* Created: 11/02/2017 12:55:15 PM
*  Author: NormalUser
*/

/* Some of the uart code taken from:
* Copyright 2011 Mika Tuupola
*
* Licensed under the MIT license:
*   http://www.opensource.org/licenses/mit-license.php
*/
#include <avr/io.h>
#include <stdio.h>
#include <util/atomic.h>
#include <avr/interrupt.h>
#include <string.h>

#include "usart.h"
#include "timers.h"

FILE usart_output = FDEV_SETUP_STREAM(USART_putchar, NULL, _FDEV_SETUP_WRITE);
FILE usart_input = FDEV_SETUP_STREAM(NULL, USART_getchar, _FDEV_SETUP_READ);

void init_PC_serial(unsigned int ubrr){
	/*Set baud rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	/*Enable receiver and transmitter, and RX interrupt */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0) | (1<<RXCIE0);
	/* Set frame format: 8data, 1stop bit */
	// UCSR0C = (3<<UCSZ00);	UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);
}

void USART_putchar(unsigned char c, FILE *stream )
{
	if (c == '\n') {
		USART_putchar('\r', stream);
	}
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) )
	;
	/* Put data into buffer, sends the data */
	UDR0 = c;
}void USART_getchar(FILE *stream){	while( !(UCSR0A & (1<<RXC0)))	;
	return UDR0;}void usart_putc(char send){	//Wait for empty transmit buffer
	while ( !( UCSR0A & (1<<UDRE0)) )
	;		UDR0 = send;}void usart_copy_command(){	if((data_in[0] == '\n') || (data_in[0] == '\r')){		//no command was entered, only the enter button pressed		//printf("No command entered\n");		usart_clear_command();		}else{		//This atomic block prevents an interrupt from interrupting partway through a copy		ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {			//copy contents of data_in into command_in			memcpy(command_in, data_in, 8);						//clear data_in			memset(data_in[0],0,8);		}	}}void usart_clear_command(){	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){		//clear data_in		memset(data_in[0],0,8);	}}/*INTERRUPT SERVICE ROUTINES*///https://www.adnbr.co.uk/articles/parsing-simple-usart-commandsISR(USART0_RX_vect){	//put data from USART register into buffer	data_in[usart_data_count] = UDR0;		//echo character back to terminal	usart_putc(data_in[usart_data_count]);		//detect end of line. Need \r because putty doesn't send \n	if((data_in[usart_data_count] == '\r') || data_in[usart_data_count] == '\n'){		usart_command_ready = 1;		usart_data_count = 0;		} else {		usart_data_count++;		if(usart_data_count>8){			usart_command_overflow = 1;		}		//printf("Data count: %u\n",usart_data_count);	}}/*COMMAND PROCESSING*/void usart_process_command(){	switch(command_in[0]){		case 't':			if(command_in[1] == '?'){				print_current_time();			}else if(command_in[1] == '='){				//update time				minutes_count = 32;				puts("dummy2");			}			break;		default:			puts("Command not recognised\n");			break;	}}