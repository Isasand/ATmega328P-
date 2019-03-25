#include <avr/io.h>
#include <avr/interrupt.h>
#include "adc.h"

void adc_init(void){
	ADCSRA |= (1<<ADPS1) | (1<<ADPS0);
	ADCSRA |= (1<<ADEN) | (1<<ADIE);
        ADMUX |= (1<<REFS0) | (1<<ADLAR);
}
