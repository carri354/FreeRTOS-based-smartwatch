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


int8_t app_idx = 0;
app_handle_t *app_list[9];



// credentials omitted for security reasons
const char *id = "";
const char *user = "";
const char *pass = ""; 

extern volatile bool power_button_flag;
extern volatile bool touch_screen_flag;
extern volatile bool accel_flag;

void handle_interrupts(){
  if(power_button_flag){
    power_button_flag = false;
    power_unit.reset_irq();
    power_unit.toggle_power();
    
  }
  if(touch_screen_flag){
    touch_screen_flag = false;
    TS_get_points(&touch_screen);

    
    Serial.print("Touch coordinates: X= ");
    Serial.print(touch_screen.p1.x);

    Serial.print("\tY= ");
    Serial.println(touch_screen.p1.y);

  }

  if(accel_flag){
    accel_flag = false;
    // lcd.drawStepCount(accelerometer.get_step_count());
    accelerometer.reset_irq();
  }
}




void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Wire.begin();
  
  // Power unit
  pinMode(AXP_INT_PIN, INPUT_PULLUP);
  attachInterrupt((digitalPinToInterrupt(AXP_INT_PIN)), AXP202_BTN_Callback, FALLING);
  power_unit.init();
  
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
  
  // Touch Screen
  pinMode(FT6236U_INT_PIN, INPUT_PULLUP);
  attachInterrupt((digitalPinToInterrupt(FT6236U_INT_PIN)), FT6236U_Touch_Callback, FALLING);
  TS_Init();

  // Accelerometer
  pinMode(BMA_INT_PIN, INPUT_PULLDOWN);
  attachInterrupt((digitalPinToInterrupt(BMA_INT_PIN)), BMA423_Callback, RISING);
  accelerometer.init();
  
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

  
  wifi.disconnect();
  

  for(int i = 0; i < 9; i++){
    if(app_list[i]){
      draw_app_icon(app_list[i], i);
    }
  }
  
  Serial.println("Setup Complete");

}

void loop() {
  // put your main code here, to run repeatedly:
  
  handle_interrupts();

  RTC_get_time(&timeInfo);
  lcd.drawDate(&timeInfo);
  lcd.drawTime(&timeInfo);
  
  
  
}