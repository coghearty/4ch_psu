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

volatile unsigned char usart_data_count;
volatile unsigned char usart_command_ready;
volatile unsigned char usart_command_overflow;

void init_PC_serial(unsigned int ubrr);
void USART_putchar(unsigned char c, FILE *stream );
void USART_getchar(FILE *stream);
void usart_copy_command();
void usart_clear_command();
void usart_process_command();

FILE usart_output;
FILE usart_input;

#endif /* USART_H_ */