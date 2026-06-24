#include "mytime.h"
#include "driver/i2c.h"
#include "esp_err.h"
#include <string.h>




/**
 * TODO
 * WRITE YOUR FUNCTION IMPLEMENTATIONS HERE
 */

void RTC_init(){

    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
        .scl_pullup_en = true,
        .sda_pullup_en = true,
        .scl_io_num = I2C_MASTER_SCL,
        .sda_io_num = I2C_MASTER_SDA
    };

    ESP_ERROR_CHECK(i2c_param_config(I2C_MASTER_PORT, &conf));
   
}



void RTC_get_time(struct tm *timeInfo){
    // Store BCD time in a buffer
    uint8_t read_buf[PCF8563_TIME_REG_SIZE];
    uint8_t buf_len = PCF8563_TIME_REG_SIZE;
    ESP_ERROR_CHECK(RTC_read(PCF8563_TIME_ADDR, read_buf, PCF8563_TIME_REG_SIZE));

    // Mask out each register
    apply_time_masks(read_buf);

    for(int i = 0; i < buf_len; i++){
        read_buf[i] = bcdToDec(read_buf[i]);
    }

    int day_365 = get_DOY(read_buf[5] - 1, read_buf[3], read_buf[6]);

    timeInfo->tm_sec  = read_buf[0];
    timeInfo->tm_min  = read_buf[1];
    timeInfo->tm_hour = read_buf[2];
    timeInfo->tm_mday = read_buf[3];
    timeInfo->tm_wday = read_buf[4];
    timeInfo->tm_mon  = read_buf[5] - 1;       // correct indexing
    timeInfo->tm_year = read_buf[6] + 100;     // 2000 to 1900 time reference

    timeInfo->tm_isdst = 1;
    timeInfo->tm_yday  = day_365;
}
void RTC_set_time(struct tm *timeInfo){
    uint8_t reg;
    // Stop RTC
    RTC_read(PCF8563_CR1_ADDR, &reg, 1);
    reg |= PCF8563_CR1_STOP;
    RTC_write(PCF8563_CR1_ADDR, &reg, 1);

    // Store Dec time in a buffer
    uint8_t write_buf[PCF8563_TIME_REG_SIZE] = {
        timeInfo->tm_sec,
        timeInfo->tm_min,
        timeInfo->tm_hour,
        timeInfo->tm_mday,
        timeInfo->tm_wday,
        timeInfo->tm_mon + 1,
        timeInfo->tm_year - 100 // 1900 to 2000 time reference
    };
    uint8_t buf_len = PCF8563_TIME_REG_SIZE;

    for(int i = 0; i < buf_len; i++){
        write_buf[i] = decToBcd(write_buf[i]);
    }

    ESP_ERROR_CHECK(RTC_write(PCF8563_TIME_ADDR, write_buf, buf_len));
    
    // Resume RTC
    RTC_read(PCF8563_CR1_ADDR, &reg, 1);
    reg &= ~PCF8563_CR1_STOP;
    RTC_write(PCF8563_CR1_ADDR, &reg, 1);
}

/**
 * Write multiple bytes to the real-time clock (RTC)
 * 
 * Written using I2C, data is pulled from buf
 * 
 * @param reg_addr Address of the register to read
 * @param buf      Data buffer containing write data
 * @param buf_len  Variable indicating buffer length
 * 
 * @return 
 *     - ESP_OK Success
 *     - ESP_ERR_INVALID_ARG Parameter error
 *     - ESP_FAIL Sending command error, slave hasn't ACK the transfer.
 *     - ESP_ERR_INVALID_STATE I2C driver not installed or not in master mode.
 *     - ESP_ERR_TIMEOUT Operation timeout because the bus is busy.
 */
static esp_err_t RTC_write(uint8_t reg_addr, uint8_t *buf, size_t buf_len){
    esp_err_t ret;
    uint8_t tx_buf[buf_len + 1];

    tx_buf[0] = reg_addr;
    memcpy(&tx_buf[1], buf, buf_len);

    ret = i2c_master_write_to_device(
        I2C_MASTER_PORT,
        I2C_PCF8563_ADDR,
        tx_buf,
        buf_len + 1,
        pdMS_TO_TICKS(1000)
    );

    return ret;
    
}
/**
 * Read multiple bytes from the real-time clock (RTC)
 * 
 * Read using I2C, data gets stored in buf and its size in buf_len
 * 
 * @param reg_addr Address of the register to read
 * @param buf      Data buffer to store read data
 * @param buf_len  Variable to store buffer length
 * 
 * @return 
*   - ESP_OK Success
*   - ESP_ERR_INVALID_ARG Parameter error
*   - ESP_FAIL Sending command error, slave hasn't ACK the transfer.
*   - ESP_ERR_INVALID_STATE I2C driver not installed or not in master mode.
*   - ESP_ERR_TIMEOUT Operation timeout because the bus is busy.
 */
static esp_err_t RTC_read(uint8_t reg_addr, uint8_t *buf, size_t buf_len){
    esp_err_t ret;

    ret = i2c_master_write_to_device(
        I2C_MASTER_PORT,
        I2C_PCF8563_ADDR,
        &reg_addr,
        1,
        pdMS_TO_TICKS(1000)
    );

    if(ret != ESP_OK){
       return ret;
    }

    ret = i2c_master_read_from_device(
        I2C_MASTER_PORT,
        I2C_PCF8563_ADDR,
        buf,
        buf_len,
        pdMS_TO_TICKS(1000)
    );

    return ret;
}



/**
 * Bitmasks to strip flag/unused bits from each PCF8563 time register,
 * indexed in the same order as the register block starting at PCF8563_TIME_ADDR:
 * [0] VL_seconds, [1] minutes, [2] hours, [3] day, [4] weekday, [5] month, [6] year
 */
static const uint8_t PCF8563_TIME_MASKS[PCF8563_TIME_REG_SIZE] = {
    PCF8563_SEC_MASK,   // seconds: strip VL flag (bit 7)
    PCF8563_MIN_MASK,   // minutes: strip unused bit 7
    PCF8563_HR_MASK,    // hours: strip unused bits 6-7
    PCF8563_DAY_MASK,   // day: strip unused bits 6-7
    PCF8563_WDAY_MASK,  // weekday: strip unused bits 3-7
    PCF8563_MON_MASK,   // month: strip unused bits 5-7 (century flag included)
    0x00                // year: no mask, full byte is valid BCD (00-99)
};

/**
 * Apply the appropriate bitmask to each raw PCF8563 time register to strip
 * flag/unused bits, in place.
 *
 * @param raw_buf  Buffer of raw register bytes, length PCF8563_TIME_REG_SIZE.
 *                 Modified in place.
 */
static void apply_time_masks(uint8_t *raw_buf){
    for(int i = 0; i < PCF8563_TIME_REG_SIZE; i++){
        raw_buf[i] &= ~PCF8563_TIME_MASKS[i];
    }
    
    
}
static int is_leap_year(int year){
    return (year % 4 == 0);
}

static int get_DOY(uint8_t month, uint8_t day, uint8_t year){
    const int DAYS_BEFORE_MONTH[12] = {
        0,   // January
        31,  // February
        59,  // March
        90,  // April
        120, // May
        151, // June
        181, // July
        212, // August
        243, // September
        273, // October
        304, // November
        334  // December
    };

    int doy = DAYS_BEFORE_MONTH[month] + (day - 1);

    if(month > 1 && is_leap_year(year)){
        doy += 1;
    }

    return doy;
}

uint8_t decToBcd(uint8_t val)
{
    return (val / 10 * 16) + (val % 10);
}

uint8_t bcdToDec(uint8_t val)
{
    return (val / 16 * 10) + (val % 16);
}



