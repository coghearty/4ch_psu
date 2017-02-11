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

void init_PC_serial(unsigned int ubrr){
	/*Set baud rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	/*Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
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
	return UDR0;}