#pragma once
#include "display.h"
#include <Arduino.h>
#include <time.h>

typedef enum{
    STATUS_OK = 0,
    STATUS_ERROR
}status_t;

typedef enum{
    DBG_SERIAL = 0,
    DBG_DISPLAY,
    DBG_SERIAL_DISPLAY
}dbg_t;

extern Display lcd;

void handle_error(const char *error_msg);
void print_date_time(struct tm *timeInfo, dbg_t mode);