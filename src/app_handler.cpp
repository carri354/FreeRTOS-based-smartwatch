#include "app_handler.h"

constexpr int HOME_HORZ_PAD = 60;
constexpr int HOME_HORZ_SPACE = 60;

constexpr int HOME_VERT_PAD = 75;
constexpr int HOME_VERT_SPACE = 60;


void draw_app_icon(app_handle_t *app, int8_t idx){
    if(idx < 0){return;}

    int8_t col = idx % 3;
    int8_t row = idx / 3;

    (lcd.*(app->draw_icon))(HOME_HORZ_PAD + HOME_HORZ_SPACE*col, HOME_VERT_PAD + HOME_VERT_SPACE*row, TFT_GREEN);
}





app_handle_t h_stopwatch{
    .draw_icon = &Display::drawClockSymbol,
    .launch = &stopwatch_launch
};
void stopwatch_launch(){
    Serial.println("Stopwatch launched!");
}

app_handle_t h_settings{
    .draw_icon = &Display::drawSettingsSymbol,
    .launch = &settings_launch
};
void settings_launch(){
    Serial.println("Settings launched!");
}

app_handle_t h_fitness{
    .draw_icon = &Display::drawFitnessSymbol,
    .launch = &fitness_launch
};
void fitness_launch(){
    Serial.println("Fitness launched!");
}