#include <Arduino.h>
#include "display.h"
#include "power.h"
#include "mywifi.h"
#include "mytime.h"
#include "touch.h"
#include "accel.h"
#include "dbg.h"
#include "app_handler.h"

Power power_unit;
Display lcd;
MyWifi wifi;
struct tm timeInfo;
touch_screen_t touch_screen;
Accel accelerometer;


int8_t app_idx = -1;
int8_t last_app_idx = -1;
app_handle_t *app_list[9];

// Shared flag: apps must poll this in their main loop and self-delete when true.
// home_page sets it, waits for the task to actually die, then clears it.
extern volatile bool app_should_exit;

// credentials omitted for security reasons
const char *id = "";
const char *user = "";
const char *pass = ""; 


extern SemaphoreHandle_t power_sem;
extern SemaphoreHandle_t touch_sem;
extern SemaphoreHandle_t accel_sem;

extern QueueHandle_t ui_button_q;

TaskHandle_t h_curr_task;

// FreeRTOS tasks
void manage_power(void *param){
  
  pinMode(AXP_INT_PIN, INPUT_PULLUP);
  attachInterrupt((digitalPinToInterrupt(AXP_INT_PIN)), AXP202_BTN_Callback, FALLING);
  power_unit.init();

  while(1){
    if(xSemaphoreTake(power_sem, portMAX_DELAY) == pdTRUE){
      power_unit.reset_irq();
      power_unit.toggle_power();
    }
    
  }
  
}

void detect_touch(void *param){
  
  pinMode(FT6236U_INT_PIN, INPUT_PULLUP);
  attachInterrupt((digitalPinToInterrupt(FT6236U_INT_PIN)), FT6236U_Touch_Callback, FALLING);
  TS_Init();

  while(1){
    if(xSemaphoreTake(touch_sem, portMAX_DELAY) == pdTRUE){
      TS_get_points(&touch_screen);

      
      app_idx = get_idx_from_touch(touch_screen.p1.x, touch_screen.p1.y, app_idx, app_list);

      if(ui_button_pressed(touch_screen.p1.x,touch_screen.p1.y) && app_idx != -1){
        xQueueSend(ui_button_q, (void *)&touch_screen.p1, 10);
      }

      Serial.print("Touch coordinates: X= ");
      Serial.print(touch_screen.p1.x);

      Serial.print("\tY= ");
      Serial.print(touch_screen.p1.y);

      Serial.print("\tIndex: ");
      Serial.println(app_idx);

    }
  }
  
}

void handle_accelerometer(void *param){
  pinMode(BMA_INT_PIN, INPUT_PULLDOWN);
  attachInterrupt((digitalPinToInterrupt(BMA_INT_PIN)), BMA423_Callback, RISING);
  accelerometer.init();

  while(1){
    if(xSemaphoreTake(accel_sem, portMAX_DELAY) == pdTRUE){
      // lcd.drawStepCount(accelerometer.get_step_count());
      accelerometer.reset_irq();
    }
  }
  
}




void home_startup(){
  lcd.clear_screen();
  for(int i = 0; i < 9; i++){
    if(app_list[i]) draw_app_icon(app_list[i], i);
  }
}

void home_page(void *param){
  home_startup();

  while(1){
    bool on_home_screen = (app_idx == -1);
    bool app_idx_changed = (app_idx != last_app_idx);

    if(on_home_screen){
      if(app_idx_changed){
        last_app_idx = -1;

        // Task deletion sequence
        app_should_exit = true;
        while(eTaskGetState(h_curr_task) != eDeleted){
          vTaskDelay(1 / portTICK_PERIOD_MS);
        }
        app_should_exit = false;

        home_startup();
      }
      RTC_get_time(&timeInfo);
      lcd.drawDate(&timeInfo);
      lcd.drawTime(&timeInfo);
    }
    else if(app_idx_changed){
      last_app_idx = app_idx;
      launch_app(app_list[app_idx], &h_curr_task);
    }

    vTaskDelay(200 / portTICK_PERIOD_MS);
  }
}



void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Wire.begin();
  vTaskDelay(1000/portTICK_PERIOD_MS);
  


  accel_sem = xSemaphoreCreateBinary();
  touch_sem = xSemaphoreCreateBinary();
  power_sem = xSemaphoreCreateBinary();

  ui_button_q = xQueueCreate(UI_BTN_QUEUE_LEN, sizeof(touch_point_t));
  // Power Unit
  xTaskCreate(
    manage_power,
    "Power Button Routine",
    2048,
    NULL,
    2,
    NULL
  );
  
  // LCD
  lcd.init();
  
 
  // WiFi
  wifi.begin(id, user, pass);

  // Time
  RTC_init();

  if(wifi.isConnected()){
    syncTimeWithNTP();
    if (!getLocalTime(&timeInfo, 2000)) {
      Serial.println("Failed to obtain time");
      return;
    }
    RTC_set_time(&timeInfo);
  }

  wifi.disconnect();
  
  // Touch screen
  xTaskCreate(
    detect_touch,
    "Touch Screen Routine",
    1536,
    NULL,
    2,
    NULL
  );

  // Accelerometer
  
  xTaskCreate(
    handle_accelerometer,
    "Accelerometer Routine",
    2048,
    NULL,
    2,
    NULL
  );
  
  // App list
  app_list[0] = &h_stopwatch;
  app_list[1] = &h_fitness;
  app_list[2] = &h_settings;

  app_list[3] = nullptr;
  app_list[4] = nullptr;
  app_list[5] = nullptr;
  app_list[6] = nullptr;
  app_list[7] = nullptr;
  app_list[8] = nullptr;



  
  
  

  xTaskCreate(
    home_page,
    "Home Page Routine",
    2048,
    NULL,
    2,
    NULL
  );



  Serial.println("Setup Complete");

  vTaskDelete(NULL);
  
}

void loop() {
  // Program should never reach this point
}

