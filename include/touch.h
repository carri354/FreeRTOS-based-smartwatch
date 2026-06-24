
#pragma once
#ifndef TOUCH_H
#define TOUCH_H

#include <stdint.h>
#include <stdbool.h>
#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_attr.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * TODO
 * CREATE YOUR FUNCTION HEADERS HERE
 */

#define I2C_FT6236U_SCL 32
#define I2C_FT6236U_SDA 23
#define I2C_FT6236U_ADDR 0x38
#define I2C_FT6236U_PORT I2C_NUM_1
#define I2C_MASTER_FREQ_HZ 100000

#define FT6236U_INT_PIN 38

#define FT6236U_POINT_REG_SIZE 6
#define FT6236U_P1_ADDR    0x03
#define FT6236U_P2_ADDR    0x09

#define XH_EVENT_MASK  (0x03 << 6)
#define XH_MSB_MASK    (0x0F << 0)
#define YH_ID_MASK     (0x0F << 4)
#define YH_MSB_MASK    (0x0F << 0)


typedef struct{
    uint8_t event;
    uint16_t x;
    uint16_t y;
    uint8_t id;
    uint8_t weight;
}touch_point_t;

typedef struct{
    touch_point_t p1;
    touch_point_t p2;
}touch_screen_t;

// Public facing functions
void TS_Init();
void TS_get_points(touch_screen_t *);

// Low-level communication functions
static esp_err_t TS_write(uint8_t reg_addr, uint8_t *buf, size_t buf_len);
static esp_err_t TS_read(uint8_t reg_addr, uint8_t *buf, size_t buf_len);
void IRAM_ATTR FT6236U_Touch_Callback();

// Helper functions
void process_point(touch_point_t *point, uint8_t *data);

#ifdef __cplusplus
}
#endif

#endif
