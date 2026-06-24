#include "touch.h"
#include "driver/i2c.h"
#include "esp_err.h"
#include <string.h>
#include <stdio.h>

/**
 * TODO
 * WRITE YOUR FUNCTION IMPLEMENTATIONS HERE
 */

void TS_Init(){
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
        .scl_pullup_en = true,
        .sda_pullup_en = true,
        .scl_io_num = I2C_FT6236U_SCL,
        .sda_io_num = I2C_FT6236U_SDA
    };

    ESP_ERROR_CHECK(i2c_param_config(I2C_FT6236U_PORT, &conf));

    ESP_ERROR_CHECK(i2c_driver_install(I2C_FT6236U_PORT, conf.mode, 0, 0, 0));
}


void TS_get_points(touch_screen_t *ts){
    uint8_t read_buf[FT6236U_POINT_REG_SIZE];
    uint8_t buf_len = FT6236U_POINT_REG_SIZE;
    ESP_ERROR_CHECK(TS_read(FT6236U_P1_ADDR, read_buf, FT6236U_POINT_REG_SIZE));
    process_point(&(ts->p1), read_buf);

    ESP_ERROR_CHECK(TS_read(FT6236U_P2_ADDR, read_buf, FT6236U_POINT_REG_SIZE));
    process_point(&(ts->p2), read_buf);
}

 /**
 * Write multiple bytes to the touch screen
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
static esp_err_t TS_write(uint8_t reg_addr, uint8_t *buf, size_t buf_len){
    esp_err_t ret;
    uint8_t tx_buf[buf_len + 1];

    tx_buf[0] = reg_addr;
    memcpy(&tx_buf[1], buf, buf_len);

    ret = i2c_master_write_to_device(
        I2C_FT6236U_PORT,
        I2C_FT6236U_ADDR,
        tx_buf,
        buf_len + 1,
        pdMS_TO_TICKS(1000)
    );

    return ret;
    
}
/**
 * Read multiple bytes from the touch screen
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
static esp_err_t TS_read(uint8_t reg_addr, uint8_t *buf, size_t buf_len){
    esp_err_t ret;

    ret = i2c_master_write_to_device(
        I2C_FT6236U_PORT,
        I2C_FT6236U_ADDR,
        &reg_addr,
        1,
        pdMS_TO_TICKS(1000)
    );

    if(ret != ESP_OK){
       return ret;
    }

    ret = i2c_master_read_from_device(
        I2C_FT6236U_PORT,
        I2C_FT6236U_ADDR,
        buf,
        buf_len,
        pdMS_TO_TICKS(1000)
    );

    return ret;
}

volatile bool touch_screen_flag = false;

void IRAM_ATTR FT6236U_Touch_Callback(){
    touch_screen_flag = true;
}

void process_point(touch_point_t *point, uint8_t *data){
    point->event = data[0] & XH_EVENT_MASK;
    point->x = (((uint16_t)data[0] & XH_MSB_MASK) << 8) | (data[1]);
    point->id = data[2] & YH_ID_MASK;
    point->y = (((uint16_t)data[2] & YH_MSB_MASK) << 8) | (data[3]);
    point->weight = data[4];
    
}