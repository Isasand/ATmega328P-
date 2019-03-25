#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdio.h>

#include "gpio.h"

void gpio_init() {
	//configure PORTB0 as output for the relay
	//and PORTB1 as output for the LED
	DDRB = (1<<PORTB0 | 1<<PORTB1);
}

void start_pump(void){
	PORTB |= (1<<PORTB0);
}

void stop_pump(void){
	PORTB &= ~(1<<PORTB0);
}

void led_on(void){
	PORTB |= (1<<PORTB1);
}

void led_off(void){
	PORTB &= ~(1<<PORTB1);
}

void led_toggle(void){
	PORTB ^= (1<<PORTB1);
}
