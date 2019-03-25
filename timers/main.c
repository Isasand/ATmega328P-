#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include <stdio.h>

#include "led.h"
#include "serial.h"
#include "timer.h"

void main (void) {
	LED_init();
	timer_init();
        while (1) {
		if(TIFR2 &(1<<OCF2A)){
			TIFR2 = (1<<OCF2A);
			OCR0A = simple_ramp();
		}
		if(TIFR0 &(1<<OCF0A)){
			TIFR0 = (1<<OCF0A);
		}
        }
}
