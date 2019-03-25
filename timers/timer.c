#include <avr/io.h>

#include "timer.h"

void timer_init() {
	TCCR0B |= (1<<CS00) | (1<<CS01);
	TCCR0A |= (1<<WGM00) | (1<<COM0A1);
	TCNT0 = 0;
	OCR0A = 20;

	TCCR2A |= (1<<WGM21);
	TCCR2B |= (1<<CS22) | (1<<CS21) | (1<<CS20);
	TCNT2 = 0;
	OCR2A = 124;
	direction = 1;
	count = 0;
}

uint8_t simple_ramp(){
	(direction) ? count++ : count--;
	if(count == 255 || count == 0){
		direction = !direction;
	}
	return count;
}
