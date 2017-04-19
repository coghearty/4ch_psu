/*
 * usart.h
 *
 * Created: 11/02/2017 12:55:33 PM
 *  Author: NormalUser
 */ 


#ifndef USART_H_
#define USART_H_

#define BAUD 4800
#define MYUBRR F_CPU/16/BAUD-1

/*Input buffer for sent commands*/
volatile unsigned char data_in[8];
unsigned char command_in[8];

volatile unsigned char data_count;
volatile unsigned char command_ready;

void init_PC_serial(unsigned int ubrr);
void USART_putchar(unsigned char c, FILE *stream );
void USART_getchar(FILE *stream);
void copy_command();

FILE usart_output = FDEV_SETUP_STREAM(USART_putchar, NULL, _FDEV_SETUP_WRITE);
FILE usart_input = FDEV_SETUP_STREAM(NULL, USART_getchar, _FDEV_SETUP_READ);

#endif /* USART_H_ */