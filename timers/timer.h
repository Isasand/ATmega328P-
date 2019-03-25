#ifndef _TIMER_H_
#define _TIMER_H_

uint8_t count;
_Bool direction;

void timer_init(void);

void timer2_init(void);

uint8_t simple_ramp(void);
#endif // _TIMER_H_
