#pragma once

#include "esp_err.h"
#include <stddef.h>
#include <stdint.h>
#include <time.h>


#ifndef MYTIME_H
#define MYTIME_H

#ifdef __cplusplus
extern "C"
{
#endif




/**
* TODO
* CREATE YOUR FUNCTION HEADERS HERE
*/

#define I2C_MASTER_SCL 22
#define I2C_MASTER_SDA 21
#define I2C_PCF8563_ADDR 0x51
#define I2C_MASTER_PORT I2C_NUM_0
#define I2C_MASTER_FREQ_HZ 100000


#define PCF8563_TIME_REG_SIZE 7 // Number of registers responsible for timekeeping
#define PCF8563_TIME_ADDR     0x02 // Starting address for time registers
#define PCF8563_CR1_ADDR      0x00

#define PCF8563_SEC_MASK    (0x01 << 7)
#define PCF8563_MIN_MASK    (0x01 << 7)
#define PCF8563_HR_MASK     (0x03 << 6)
#define PCF8563_DAY_MASK    (0x03 << 6)
#define PCF8563_WDAY_MASK   (0x1F << 3)
#define PCF8563_MON_MASK    (0x07 << 5)

#define PCF8563_CR1_STOP    (0x01 << 5)


// Public-facing functions
void RTC_init();
void RTC_get_time(struct tm *timeInfo);
void RTC_set_time(struct tm *timeInfo);





// Low-level register writes
static esp_err_t RTC_write(uint8_t reg_addr, uint8_t *buf, size_t buf_len);
static esp_err_t RTC_read(uint8_t reg_addr, uint8_t *buf, size_t buf_len);


// Helper functions
static void apply_time_masks(uint8_t *raw_buf);
static int get_DOY(uint8_t month, uint8_t day, uint8_t year);
uint8_t decToBcd(uint8_t val);
uint8_t bcdToDec(uint8_t val);

#ifdef __cplusplus
}
#endif

#endif