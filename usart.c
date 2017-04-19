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
//#include "usart.h"

void init_PC_serial(unsigned int ubrr){
	/*Set baud rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	/*Enable receiver and transmitter, and RX interrupt */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);// | (1<<RXCIE0);
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
	return UDR0;}/*void copy_command(){	//This atomic block prevents an interrupt from interrupting partway through a copy	ATOMIC_BLOCK(ATOMIC_FORCEON) {		//copy contents of data_in into command_in		memcpy(command_in, data_in, 8);				//clear data_in		memset(data_in[0],0,8);	}}*//*INTERRUPT SERVICE ROUTINES*///https://www.adnbr.co.uk/articles/parsing-simple-usart-commands/*ISR(USART0_RX_vect){	//put data from USART register into buffer	data_in[data_count] = UDR0;		//detect end of line	if(data_in[data_count] == '\n'){		command_ready = 1;		data_count = 0;	} else {		data_count++; //should add some logic to detect commands that are too long (over 8 characters).	}}*/