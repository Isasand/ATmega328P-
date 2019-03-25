#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"

void timer_init() {
	TCCR0B |= (1<<CS00) | (1<<CS01);
	TCCR0A |= (1<<WGM01) | (1<<WGM00)| (1<<COM0A1);

	TCCR2A |= (1<<WGM21) | (1<<COM2A1);
	TCCR2B |= (1<<CS20) | (1<<CS21) | (1<<CS22);
	TIMSK2 |= (1<<OCIE2A);
	OCR2A = 77;
}
