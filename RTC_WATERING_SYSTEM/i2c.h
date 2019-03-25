#ifndef _I2C_H_
#define _I2C_H_

#define I2C_FREQ 100000
#define I2C_BITRATE ((F_CPU / I2C_FREQ) - 16) / 2 + 1

#define EEPROM_ADDR 0xA0

#define MT_SLA_W_ACK 0x18

// I2C Read/Write flags
#define I2C_R	1
#define I2C_W	0

/*
 * RTC DEFINES
 */

#define RTC_ADDR 0xD0

//control register
#define CTRR 0xE
//status register
#define STAR 0xF
//timekeeping registers
#define HTIMR 0x2
#define MTIMR 0x1
#define STIMR 0x0

//alarm interrupt enables and interrupt control bit
#define A1IE_INTCN 0x5
#define A2IE_INTCN 0x6

//RTC Alarm register mask bits
#define A2M2 0x80
#define A2M3 0x80
#define A2M3 0x80
#define A2M4 0x80

#define A1M1 0x80
#define A1M2 0x80
#define A1M3 0x80
#define A1M4 0x80

//RTC alarm flags
#define A1F 0x1
#define A2F 0x2

#define WAKEUP_HOUR 10
#define WAKEUP_MINUTE 30

enum interval{
	EVERY_SECOND,
	EVERY_MINUTE,
	ONCE_DAILY,
	TWICE_DAILY
};

typedef enum interval interval;

void i2c_init(void);

void i2c_start(void);
void i2c_stop(void);

uint8_t i2c_get_status(void);
void i2c_meaningful_status(uint8_t status);

void i2c_emit_addr(uint8_t address, uint8_t rw);
void i2c_emit_byte(uint8_t data);

void rtc_wait_until_write_complete(void);

uint8_t rtc_read_byte(uint8_t addr);
void rtc_write_byte(uint8_t addr, uint8_t data);

void rtc_write_page(uint8_t *data, uint8_t start_addr, size_t len);
void rtc_sequential_read(uint8_t *buf, uint8_t start_addr, size_t len);

void print_rtc(void);
void rtc_set_interval(interval selected_interval);
void rtc_reset_clock(void);
void rtc_set_time(uint8_t hours, uint8_t minutes, uint8_t seconds);
uint8_t dec_to_bcd(uint8_t dec);
uint8_t bcd_to_dec(uint8_t bcd);
#endif // _I2C_H_
