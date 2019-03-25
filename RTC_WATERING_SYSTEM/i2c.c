#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#include <stdio.h>
#include <string.h>

#include "i2c.h"

void i2c_init(void) {
	TWBR = I2C_BITRATE;
}

void i2c_meaningful_status(uint8_t status) {
	switch (status) {
		case 0x08: // START transmitted, proceed to load SLA+W/R
			printf_P(PSTR("START\n"));
			break;
		case 0x10: // repeated START transmitted, proceed to load SLA+W/R
			printf_P(PSTR("RESTART\n"));
			break;
		case 0x38: // NAK or DATA ARBITRATION LOST
			printf_P(PSTR("NOARB/NAK\n"));
			break;
		// MASTER TRANSMIT
		case 0x18: // SLA+W transmitted, ACK received
			printf_P(PSTR("MT SLA+W, ACK\n"));
			break;
		case 0x20: // SLA+W transmitted, NAK received
			printf_P(PSTR("MT SLA+W, NAK\n"));
				break;
		case 0x28: // DATA transmitted, ACK received
			printf_P(PSTR("MT DATA+W, ACK\n"));
			break;
		case 0x30: // DATA transmitted, NAK received
			printf_P(PSTR("MT DATA+W, NAK\n"));
			break;
		// MASTER RECEIVE
		case 0x40: // SLA+R transmitted, ACK received
			printf_P(PSTR("MR SLA+R, ACK\n"));
			break;
		case 0x48: // SLA+R transmitted, NAK received
			printf_P(PSTR("MR SLA+R, NAK\n"));
			break;
		case 0x50: // DATA received, ACK sent
			printf_P(PSTR("MR DATA+R, ACK\n"));
			break;
		case 0x58: // DATA received, NAK sent
			printf_P(PSTR("MR DATA+R, NAK\n"));
			break;
		default:
			printf_P(PSTR("N/A %02X\n"), status);
			break;
	}
}

inline void i2c_wait_on_TWINT(void) {
	while (!(TWCR & (1 << TWINT)))
		;
}

inline void i2c_start() {
	TWCR = (1 << TWSTA | 1 << TWEN | 1 << TWINT);
	i2c_wait_on_TWINT();
}

inline void i2c_stop() {
	TWCR = (1 << TWSTO | 1 << TWEN | 1 << TWINT);
	_delay_us(50);
}

inline uint8_t i2c_get_status(void) {
	return TWSR & 0xF8;
}

inline void i2c_emit_addr(uint8_t address, uint8_t rw) {
	TWDR = address | rw;
	TWCR = (1 << TWEN | 1 << TWINT);
	i2c_wait_on_TWINT();
}

inline void i2c_emit_byte(uint8_t data) {
	TWDR = data;
	TWCR = (1 << TWEN | 1 << TWINT);
	i2c_wait_on_TWINT();
}

inline uint8_t i2c_read_ACK() {
	TWCR = (1 << TWEN | 1 << TWEA | 1 << TWINT);
	i2c_wait_on_TWINT();

	return TWDR;
}

inline uint8_t i2c_read_NAK() {
	TWCR = (1 << TWEN | 1 << TWINT);
	i2c_wait_on_TWINT();

	return TWDR;
}

inline void rtc_wait_until_write_complete() {
	do {
		i2c_start();
		i2c_emit_addr(RTC_ADDR, I2C_W);
	} while (i2c_get_status() != MT_SLA_W_ACK);
}

uint8_t rtc_read_byte(uint8_t addr) {
	uint8_t data;

	i2c_start();
	i2c_emit_addr(RTC_ADDR, I2C_W);
	i2c_emit_byte(addr);

	i2c_start();
	i2c_emit_addr(RTC_ADDR, I2C_R);
	data = i2c_read_NAK();
	i2c_stop();

	return data;
}

void rtc_write_byte(uint8_t addr, uint8_t data) {
	i2c_start();
	i2c_emit_addr(RTC_ADDR, I2C_W);
	i2c_emit_byte(addr);
	i2c_emit_byte(data);
	i2c_stop();

	rtc_wait_until_write_complete();
}

void rtc_write_page(uint8_t *data, uint8_t start_addr, size_t len) {
	uint8_t *ptr = data;

	i2c_start();
	i2c_emit_addr(RTC_ADDR, I2C_W);
	i2c_emit_byte(start_addr);

	for (int i = 0; i < len; i++) {
		i2c_emit_byte(*(ptr++));
	}
	i2c_stop();

	rtc_wait_until_write_complete();
}

void rtc_sequential_read(uint8_t *buf, uint8_t start_addr, size_t len) {
	uint8_t ix = 0;
	uint8_t *ptr = buf;

	i2c_start();
	i2c_emit_addr(RTC_ADDR, I2C_W);
	i2c_emit_byte(start_addr);

	i2c_start();
	i2c_emit_addr(RTC_ADDR, I2C_R);

	do {
		*(ptr++) = i2c_read_ACK();
	} while (ix++ < len - 1);

	*ptr = i2c_read_NAK();

	i2c_stop();
}

void print_rtc(void){
	uint8_t buf[255];
	rtc_sequential_read(buf, 0, 18);
	for(uint8_t i = 0; i < 18; i++){
		printf_P(PSTR("%d: %d\n"), i, buf[i]);
	}
	printf_P(PSTR("\n"));
}

void rtc_set_interval(interval selected_interval){
	static uint8_t count = 1;
	unsigned char flags[20];
	switch(selected_interval){
		case EVERY_SECOND:
			rtc_write_byte(CTRR, A1IE_INTCN);
			flags[0] = A1M1;
			flags[1] = A1M2;
			flags[2] = A1M2;
			flags[3] = A1M3;
			rtc_write_page(flags,0x7, 4);
			break;
		case EVERY_MINUTE:
			rtc_write_byte(CTRR, A2IE_INTCN);
			flags[0] = A2M2;
			flags[1] = A2M3;
			flags[2] = A2M4;
			rtc_write_page(flags, 0xB, 3);
			break;
		case ONCE_DAILY:
			rtc_write_byte(CTRR, A2IE_INTCN);
			rtc_write_byte(0xD, A2M4);
			rtc_write_byte(0xB, WAKEUP_MINUTE);
			rtc_write_byte(0xC, WAKEUP_HOUR);
			break;
		case TWICE_DAILY:
			rtc_write_byte(CTRR, A2IE_INTCN);
			rtc_write_byte(0xD, A2M4);
			rtc_write_byte(0xB, WAKEUP_MINUTE);
			if(count == 1){
				rtc_write_byte(0xC, (WAKEUP_HOUR+5));
				count = 2;
			}
			else{
				rtc_write_byte(0xC, WAKEUP_HOUR);
				count = 1;
			}
			break;
	}
}

void rtc_reset_clock(void){
	rtc_write_byte(HTIMR, 0);
        rtc_write_byte(MTIMR, 0);
        rtc_write_byte(STIMR, 0);
}

void rtc_set_time(uint8_t hours, uint8_t minutes, uint8_t seconds){
	rtc_write_byte(HTIMR, dec_to_bcd(hours));
	rtc_write_byte(MTIMR, dec_to_bcd(minutes));
	rtc_write_byte(STIMR, dec_to_bcd(seconds));
}

//data in some registers are stored in BCD form
uint8_t dec_to_bcd(uint8_t dec){
        return ((dec/10*16) + (dec%10));
}

//data from some registers will be returned in BCD form
uint8_t bcd_to_dec(uint8_t bcd){
	return ((bcd/16*10)+(bcd%16));
}
