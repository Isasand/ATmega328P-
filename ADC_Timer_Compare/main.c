#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include "led.h"
#include "serial.h"
#include "timer.h"
#include "adc.h"
#include "statemachine.h"

void init(void);

ISR (TIMER2_COMPA_vect){
	runstates[state]();
	handle_state();
}

ISR (ADC_vect){
	res = ADCH;
}

void main (void) {
	init();
	sei();
       	while(1){

	}
}

void init(void){
	res = 0;
	state = st_init;
	LED_init();
	uart_init();
	timer_init();
	adc_init();
}
