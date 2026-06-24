#include <Arduino.h>
#include "display.h"
#include "power.h"
#include "mywifi.h"
#include "mytime.h"
#include "touch.h"
#include "accel.h"
#include "dbg.h"

Power power_unit;
Display lcd;
MyWifi wifi;
struct tm timeInfo;
touch_screen_t touch_screen;

const char *id = "@umlguest";
const char *user = "summeruser305@umlguest";
const char *pass = 0; // password omitted for security reasons

extern volatile bool power_button_flag;
extern volatile bool touch_screen_flag;

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
}




void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
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

  
  wifi.disconnect();
  
  Serial.println("Setup Complete");

}

void loop() {
  // put your main code here, to run repeatedly:
  
  handle_interrupts();

  RTC_get_time(&timeInfo);
  lcd.drawDate(&timeInfo);
  lcd.drawTime(&timeInfo);
  
  
}