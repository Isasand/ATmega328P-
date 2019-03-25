#ifndef _GPIO_H_
#define _GPIO_H_

#define DEBOUNCE_MAX 15

void gpio_init(void);
void start_pump(void);
void stop_pump(void);
void led_on(void);
void led_off(void);
void led_toggle(void);

#endif // _GPIO_H_

