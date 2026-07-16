#pragma once
#include "dbg.h"
#include "display.h"
#include "touch.h"
#include "mytime.h"
#include "accel.h"

constexpr int HOME_HORZ_PAD = 60;
constexpr int HOME_HORZ_SPACE = 60;

constexpr int HOME_VERT_PAD = 75;
constexpr int HOME_VERT_SPACE = 60;

constexpr int UI_BTN_QUEUE_LEN = 25;



typedef struct{

    const char *name;

    // # of bytes to allocate for this app
    uint32_t program_size; 
    // Function pointer to icon drawing function
    void (Display::*draw_icon)(int, int, uint16_t);
    // Function pointer to launch app
    void (*launch)(void *);

}app_handle_t;




void draw_app_icon(app_handle_t *, int8_t idx);
void launch_app(app_handle_t *);
void launch_app(app_handle_t *, TaskHandle_t *);
int8_t get_idx_from_touch(uint16_t x, uint16_t y, int8_t curr, app_handle_t **apps);
bool ui_button_pressed(uint16_t x, uint16_t y);

extern app_handle_t h_stopwatch;
void stopwatch_launch(void *);

extern app_handle_t h_settings;
void settings_launch(void *);

extern app_handle_t h_fitness;
void fitness_launch(void *);




