#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>

#include "adc.h"
#include "gpio.h"
#include "i2c.h"
#include "serial.h"
#include "timer.h"


void main (void) {
i2c_init();
        uart_init();
        uint8_t mem_location = 0x00;
        char buf[16];
        sei();

	eeprom_write_page(0x00, "fizzbuzz");
	eeprom_write_page(0x08, "foo--bar");
	eeprom_sequential_read(buf, mem_location, 16);
	for(int i = 0; i < 16; i++){
		printf_P(PSTR("00%02x: "), mem_location++);
		printf_P(PSTR("%02x\n"), buf[i]);
	}

	while (1) {
	}
}
