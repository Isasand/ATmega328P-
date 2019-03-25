#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>

#include "adc.h"
#include "gpio.h"
#include "i2c.h"
#include "serial.h"
#include "timer.h"
#include "util.h"

#define INTERVAL EVERY_MINUTE

volatile uint8_t rtc_interrupt = 0;

void init(void);
void sleep(void);
void loop(void);
void handle_rtc_interrupt(void);

//functions to test/ demo functionality
void blinky_interval(void);
void wakeup_demo(void);
void demoloop(void);

ISR (ADC_vect){
	adc_value = get_adc_value();
}

//external interrupt
ISR (PCINT2_vect){
	rtc_interrupt = 1;
}

void main (void) {
	sei();
	init();
	rtc_set_interval(INTERVAL);
	while (1) {
		loop();
	}
}


void loop(void){
	printf_P(PSTR("Going to sleep...\n"));
	_delay_ms(200);
	sleep();
	if(rtc_interrupt){
		handle_rtc_interrupt();
		rtc_interrupt = 0;
	}
}

void handle_rtc_interrupt(void){
	for(uint8_t i = 0; i<=2; i++){
		adc_start();
		_delay_ms(500);
		if(adc_value > 200){
			start_pump();
			_delay_ms(6000);
			stop_pump();
			_delay_ms(6000);
		}
	}
	rtc_set_interval(INTERVAL);
	rtc_write_byte(STAR, 0x88);
}

void sleep(void){
        //set pin 3 as external interrupt
        PCMSK2 |= (1<<PCINT19);
        //set pin change interrupt flag 2
        PCIFR |= (1<<PCIF2);
        //enable pin change interrupt 2
        PCICR |= (1<<PCIE2);
        //set sleep mode to power save
        set_sleep_mode(SLEEP_MODE_PWR_DOWN);
        sleep_enable();
        sleep_cpu();
        sleep_disable();
}

void init(void){
        uart_init();
        adc_init();
        gpio_init();
        i2c_init();
}

//simulate waking up at a specific time by setting the time to one minute before desired time
void wakeup_demo(void){
	rtc_set_interval(ONCE_DAILY);
	rtc_set_time(WAKEUP_HOUR, WAKEUP_MINUTE -1, 0);
}

//before calling demoloop, call wakeup_demo once
void demoloop(void){
	if(!(PIND &(1<<PCINT19))){
		led_toggle();
                rtc_set_interval(ONCE_DAILY);
		rtc_write_byte(STAR, 0x88);
        }
}

//to use this, call rtc_set_interval(INTERVAL) once
void blinky_interval(void){
	if(!(PIND &(1<<PCINT19))){
		led_toggle();
		rtc_set_interval(INTERVAL);
		rtc_write_byte(STAR, 0x88);
	}
}
