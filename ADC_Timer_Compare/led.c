#include <avr/io.h>
#include "led.h"

void LED_init() {
	DDRD |= (1<<6);
}

uint8_t simple_ramp() {
	static uint8_t direction = 1;
	static uint8_t count = 0;
	(direction) ? count++ : count--;
	if(count == 255 || count == 0){
		direction = !direction;
	}
	return count;
}
