#include "app_handler.h"

volatile bool app_should_exit = false;


void draw_app_icon(app_handle_t *app, int8_t idx){
    if(idx < 0){return;}

    int8_t col = idx % 3;
    int8_t row = idx / 3;

    (lcd.*(app->draw_icon))(HOME_HORZ_PAD + HOME_HORZ_SPACE*col, HOME_VERT_PAD + HOME_VERT_SPACE*row, TFT_GREEN);
}

// Non-RTOS app launcher
void launch_app(app_handle_t *app){
    (*(app->launch))(NULL);
}

// RTOS-based app launcher
void launch_app(app_handle_t *app, TaskHandle_t *h_task){

    xTaskCreate(
        app->launch,
        app->name,
        app->program_size,
        NULL,
        2,
        h_task
    );
}


bool home_button_touched(uint16_t x, uint16_t y){
    return x > HOME_X && y > HOME_Y && x < 240 && y < 240;
}

/// @brief Get app index from touch coordinates (typically 0-8)
/// @param x x-coord of touch
/// @param y y-coord of touch
/// @param curr current screen index
/// @param apps pointer to app list
/// @return App index, returns -1 if index is invalid
int8_t get_idx_from_touch(uint16_t x, uint16_t y, int8_t curr, app_handle_t **apps){

    // Not in home edge cases
    if(curr != -1 && !home_button_touched(x,y)){
        return curr;
    }

    if(curr != -1 && home_button_touched(x,y)){
        return -1;
    }


    if(x < HOME_HORZ_PAD - HOME_HORZ_SPACE/2 || y < HOME_VERT_PAD - HOME_VERT_SPACE/2) return -1;
    if(x > HOME_HORZ_PAD + (HOME_HORZ_SPACE*2))return -1;
    if(y > HOME_VERT_PAD + (HOME_VERT_SPACE*2))return-1;

    

   
    // Null app edge case
    int8_t idx = ((y - HOME_VERT_PAD + (HOME_VERT_SPACE/2)) / HOME_VERT_SPACE )*3 
            + ((x - HOME_HORZ_PAD + (HOME_HORZ_SPACE/2)) / HOME_HORZ_SPACE );

    if(idx != -1 && apps[idx] == nullptr){
        return -1;
    }

    return idx;
}

QueueHandle_t ui_button_q;
bool ui_button_pressed(uint16_t x, uint16_t y){
    return x < HOME_X || y < HOME_Y;
}


app_handle_t h_stopwatch{
    .name = "Stopwatch",
    .program_size = 2048,
    .draw_icon = &Display::drawClockSymbol,
    .launch = &stopwatch_launch,
    
};

uint8_t stopwatch_detect_button(touch_point_t *p){
    uint16_t x = p->x;
    uint16_t y = p->y;

    // Check for start button pressed
    if( x >= 50 && x < 115 && y >= 120 && y < 140){return 0;}

    // Check for stop button pressed
    if( x >= 140 && x < 195 && y >= 120 && y < 140){ return 1;}

    // Check for reset button pressed
    if( x >= 100 && x < 165 && y >= 150 && y < 170){ return 2;}
    // default case: no button pressed
    return 3;

}
void stopwatch_launch(void* param){
    touch_point_t point;
    Serial.println("Stopwatch launched!");
    lcd.drawStopWatchStart();

    bool is_stopped = true;
    bool is_reset = true;

    struct tm time_ref = {0};
    struct tm real_time = {0};
    struct tm stoppage_time = {0};

    long accumulated_sec = 0;

    while(1){
        if(app_should_exit){ vTaskDelete(NULL); }
        RTC_get_time(&real_time);

        if(xQueueReceive(ui_button_q, (void *)&point, 50/portTICK_PERIOD_MS)){
            switch(stopwatch_detect_button(&point)){
                case 0: // Start
                    if(is_reset){
                        accumulated_sec = 0;
                        RTC_get_time(&time_ref);
                    }else if(is_stopped){
                        RTC_get_time(&time_ref);
                    }
                    is_reset = false;
                    is_stopped = false;
                break;
                case 1: // Stop
                    if(!is_stopped){
                        accumulated_sec += (long)difftime(mktime(&real_time), mktime(&time_ref));
                        is_stopped = true;
                    }
                break;
                case 2: // Reset
                    is_reset = true;
                    accumulated_sec = 0;
                    lcd.print_d("00:00:00", 20, 80, 4);
                break;
                default:
                break;
            }
        }

        if(!is_stopped && !is_reset){
            long elapsed_sec = accumulated_sec + (long)difftime(mktime(&real_time), mktime(&time_ref));

            stoppage_time.tm_hour = elapsed_sec / 3600;
            stoppage_time.tm_min  = (elapsed_sec % 3600) / 60;
            stoppage_time.tm_sec  = elapsed_sec % 60;

            char str[9];
            strftime(str, sizeof(str), "%H:%M:%S", &stoppage_time);
            lcd.print_d(str, 20, 80, 4);
        }
    }
}

app_handle_t h_settings{
    .name = "Settings",
    .program_size = 1024,
    .draw_icon = &Display::drawSettingsSymbol,
    .launch = &settings_launch,
    
};
void settings_launch(void* param){
    Serial.println("Settings launched!");
    lcd.drawSettingsStart();
    while(1){
        if(app_should_exit){ vTaskDelete(NULL); }
        vTaskDelay(3000/portTICK_PERIOD_MS);
        Serial.println("Settings active!");
    }
}

app_handle_t h_fitness{
    .name = "Fitness",
    .program_size = 2048,
    .draw_icon = &Display::drawFitnessSymbol,
    .launch = &fitness_launch,
    
};
extern Accel accelerometer;
void fitness_launch(void* param){
    Serial.println("Fitness launched!");
    lcd.drawFitnessStart();
    lcd.drawStepCount(accelerometer.get_step_count());
    while(1){
        if(app_should_exit){ vTaskDelete(NULL); }

        vTaskDelay(300/portTICK_PERIOD_MS);
        lcd.drawStepCount(accelerometer.get_step_count());
    }
}