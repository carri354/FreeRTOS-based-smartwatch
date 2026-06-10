#include "mytime.h"
#include "driver/i2c.h"
#include "esp_err.h"
#include <string.h>

/**
 * TODO
 * WRITE YOUR FUNCTION IMPLEMENTATIONS HERE
 */

uint8_t decToBcd(uint8_t val)
{
    return (val / 10 * 16) + (val % 10);
}

uint8_t bcdToDec(uint8_t val)
{
    return (val / 16 * 10) + (val % 16);
}