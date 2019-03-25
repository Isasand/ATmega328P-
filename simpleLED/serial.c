#include <avr/io.h>
#include <util/delay.h>
#include "serial.h"
#include "led.h"
#include <string.h>

#define FOSC 16000000UL
#define BAUD 38400
#define MYUBRR FOSC/16/BAUD-1

void uart_init(void){
	unsigned int ubrr = MYUBRR;
	UBRR0H = (unsigned char) (ubrr >> 8);
	UBRR0L = (unsigned char) ubrr;
	UCSR0C = (3 << UCSZ00);
	UCSR0B = (1 << RXEN0) | (1 << TXEN0) ;
}

void uart_putchar(char chr){
	while( !(UCSR0A & (1<<UDRE0) ) );
	if(chr == '\n'){
		UDR0 = '\r';
		while( !(UCSR0A & (1<<UDRE0 )) );
	}
	UDR0 = chr;
}

void uart_putstr(const char *str){
	while(*str){
		uart_putchar(*str++);
	}
}

char uart_getchar(void){
	while ( !(UCSR0A & (1<<RXC0))){};

	return UDR0;
}

void uart_echo(void){
	uart_putchar(uart_getchar());
}

void control_led(void){

	char c[4]; //declare a char array to store incoming string
	uint8_t i = 0; //counter

	while(1){
		//get recieved data from buffer
		c[i] = uart_getchar();
		//break the loop on newline
		if(c[i] == '\n'){
			break;
		}
		//increment counter
		i++;
	}

	//null terminate string
	c[++i] = '\0';
	//set direction on port pin 1
	DDRB = 0x01;
	//use strcmp to compare data
	if(strcmp(c, "OFF\r\n") == 0){
		//set port b to low
		PORTB = 0x0;
	}
	else if(strcmp(c, "ON\r\n") == 0){
		//set bit 1 (pin 8) on port B to high
		PORTB = 0x01;
	}
}

