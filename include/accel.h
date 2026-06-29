#pragma once
#include <Arduino.h>
#include <SensorBMA423.hpp>
#include "dbg.h"

constexpr uint8_t BMA_INT_PIN  = 39;
void IRAM_ATTR BMA423_Callback();

class Accel
{
public:
    /**
     * TODO
     * WRITE YOUR PUBLIC MEMBERS AND FUNCTION HEADERS HERE
     */
    Accel();


    void init();
    void reset_irq();
    uint32_t get_step_count();
private:
    /**
     * TODO
     * WRITE YOUR PRIVATE MEMBERS AND FUNCTIONS HEADERS HERE
     */
    SensorBMA423 bma;
    uint8_t i2c_sda;
    uint8_t i2c_scl;
    uint8_t i2c_addr;
};