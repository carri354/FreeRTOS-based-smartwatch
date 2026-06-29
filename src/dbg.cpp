#include "dbg.h"

void handle_error(const char *error_msg){
    Serial.print("ERROR: ");
    Serial.println(error_msg);
    lcd.print_d(error_msg, 20, 200);
}

void print_date_time(struct tm *timeInfo, dbg_t mode) {
  char formattedTime[80];  // Buffer to store the formatted string
  strftime(formattedTime, sizeof(formattedTime), "%A, %B %d %Y %H:%M:%S", timeInfo);

  switch(mode){
    case DBG_SERIAL:
        Serial.println(formattedTime);
    break;
    case DBG_DISPLAY:
        lcd.print_d(formattedTime, 0, 200);
    break;
    case DBG_SERIAL_DISPLAY:
        Serial.println(formattedTime);
        lcd.print_d(formattedTime, 0, 200);
    break;
  }
  
}