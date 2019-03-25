#include <avr/io.h>
#include <util/delay.h>
#include "serial.h"
#include "led.h"

void main (void) {
	uart_init();
	while(1){
		control_led();
		_delay_ms(500);
	}
}

