#include "app_handler.h"




void draw_app_icon(app_handle_t *app, int8_t idx){
    if(idx < 0){return;}

    int8_t col = idx % 3;
    int8_t row = idx / 3;

    (lcd.*(app->draw_icon))(HOME_HORZ_PAD + HOME_HORZ_SPACE*col, HOME_VERT_PAD + HOME_VERT_SPACE*row, TFT_GREEN);
}

void launch_app(app_handle_t *app){
    (*(app->launch))();
}

/// @brief Get app index from touch coordinates (typically 0-8)
/// @param x x-coord of touch
/// @param y y-coord of touch
/// @return App index, returns -1 if index is invalid
int8_t get_idx_from_touch(uint16_t x, uint16_t y){
    if(x < HOME_HORZ_PAD - HOME_HORZ_SPACE/2 || y < HOME_VERT_PAD - HOME_VERT_SPACE/2) return -1;
    if(x > HOME_HORZ_PAD + (HOME_HORZ_SPACE*2))return -1;
    if(y > HOME_VERT_PAD + (HOME_VERT_SPACE*2))return-1;



   

    return ((y - HOME_VERT_PAD + (HOME_VERT_SPACE/2)) / HOME_VERT_SPACE )*3 
            + ((x - HOME_HORZ_PAD + (HOME_HORZ_SPACE/2)) / HOME_HORZ_SPACE );
}




app_handle_t h_stopwatch{
    .draw_icon = &Display::drawClockSymbol,
    .launch = &stopwatch_launch
};
void stopwatch_launch(){
    
    Serial.println("Stopwatch launched!");
    lcd.drawStopWatchStart();

    
    
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