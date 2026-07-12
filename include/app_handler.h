#pragma once
#include "dbg.h"
#include "display.h"


constexpr int HOME_HORZ_PAD = 60;
constexpr int HOME_HORZ_SPACE = 60;

constexpr int HOME_VERT_PAD = 75;
constexpr int HOME_VERT_SPACE = 60;




typedef struct{
    // Function pointer to icon drawing function
    void (Display::*draw_icon)(int, int, uint16_t);
    // Function pointer to launch app
    void (*launch)(void);
}app_handle_t;


void draw_app_icon(app_handle_t *, int8_t idx);
void launch_app(app_handle_t *);
int8_t get_idx_from_touch(uint16_t x, uint16_t y);

extern app_handle_t h_stopwatch;
void stopwatch_launch();

extern app_handle_t h_settings;
void settings_launch();

extern app_handle_t h_fitness;
void fitness_launch();




