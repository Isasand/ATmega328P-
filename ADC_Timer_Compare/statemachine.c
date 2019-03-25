#include <avr/io.h>
#include <avr/interrupt.h>
#include "statemachine.h"
#include "led.h"
#include <util/delay.h>

void st_simpleramp(void){
	OCR0A = simple_ramp();
}

void st_potentiometer(void){
	ADCSRA |= (1<<ADSC);
	OCR0A = res;
}

void st_blinky(void){
	static int count = 0;
	if(count==100){
		(OCR0A == 0) ? (OCR0A = 255) : (OCR0A = 0);
		count = 0;
	}
	count++;
}

void st_shutdown(void){
	OCR0A = 0;
}

void (*runstates[STATES_MAX])(void) = {
        st_simpleramp,
        st_potentiometer,
        st_blinky,
        st_shutdown
};

void handle_state(void){
        static uint8_t btn_state = 0;
        static uint8_t debounce = 0;
        if(PIND&(1<<2)){
                if(debounce++>=DEBOUNCE_MAX){
                        debounce = DEBOUNCE_MAX;
                        btn_state = 1;
                }
        }
        if(!(PIND&(1<<2))&& btn_state == 1){
                if(debounce-- == 0){
                        debounce = 0;
                        btn_state = 0;
                        (state == STATES_MAX-1) ? state = st_init : state++;
                }
        }
}
