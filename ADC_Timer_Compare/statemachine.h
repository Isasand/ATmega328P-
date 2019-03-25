#ifndef _STATEMACHINE_H
#define _STATEMACHINE_H
#define st_init 0
#define STATES_MAX 4
#define DEBOUNCE_MAX 15

volatile uint8_t res;
volatile uint8_t state;

void st_simpleramp(void);
void st_potentiometer(void);
void st_blinky(void);
void st_shutdown(void);

void (*runstates[STATES_MAX])(void);
void handle_state(void);
#endif
