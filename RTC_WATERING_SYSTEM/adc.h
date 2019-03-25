#ifndef _ADC_H_
#define _ADC_H_

static volatile uint8_t adc_value;
void adc_init(void);
void set_adc_value(uint8_t val);
uint8_t get_adc_value(void);
void adc_start(void);

#endif // _ADC_H_
