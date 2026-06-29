#pragma once
#include "dbg.h"
#include "display.h"
typedef struct{
    // Function pointer to icon drawing function
    void (Display::*draw_icon)(int, int, uint16_t);
    // Function pointer to launch app
    void (*launch)(void);
}app_handle_t;


void draw_app_icon(app_handle_t *, int8_t idx);


extern app_handle_t h_stopwatch;
void stopwatch_launch();

extern app_handle_t h_settings;
void settings_launch();

extern app_handle_t h_fitness;
void fitness_launch();




