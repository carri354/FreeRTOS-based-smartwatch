#include "mywifi.h"


/**
 * TODO
 * WRITE YOUR CLASS FUNCTION IMPLEMENTATIONS HERE
 */

 status_t MyWifi::begin(const char* identity, const char* username, const char* password){
    _identity = identity;
    _username = username;
    _password = password;

    WiFi.disconnect(true, true);
    WiFi.mode(WIFI_STA);

    return connect();
}

status_t MyWifi::connect(){
    esp_err_t ret;

    ret = esp_wifi_sta_wpa2_ent_set_identity((uint8_t *)_identity.c_str(), _identity.length());
    switch(ret){
        case ESP_ERR_INVALID_ARG:
            handle_error("SET IDENTITY INVALID ARG!");
        break;

        case ESP_ERR_NO_MEM:
            handle_error("SET IDENTITY NO MEM!");
        break;
    }

    ret = esp_wifi_sta_wpa2_ent_set_username((uint8_t *)_username.c_str(), _username.length());
    switch(ret){
        case ESP_ERR_INVALID_ARG:
            handle_error("SET USERNAME INVALID ARG!");
        break;

        case ESP_ERR_NO_MEM:
            handle_error("SET USERNAME NO MEM!");
        break;
    }

    ret = esp_wifi_sta_wpa2_ent_set_password((uint8_t *)_password.c_str(), _password.length());
    switch(ret){
        case ESP_ERR_INVALID_ARG:
            handle_error("SET PASSWORD INVALID ARG!");
        break;

        case ESP_ERR_NO_MEM:
            handle_error("SET PASSWORD NO MEM!");
        break;
    }

    ret = esp_wifi_sta_wpa2_ent_enable();
    switch(ret){
        case ESP_ERR_NO_MEM:
            handle_error("WPA2 EN NO MEM!");
        break;
    }
    


    Serial.println("Attempting WiFi connection");
    WiFi.begin(SSID);
    unsigned long t1 = millis();
    while (!isConnected()){
        unsigned long t2 = millis();
        if(t2 - t1 >= WIFI_TIMEOUT_MS){
            handle_error("WiFi connection timeout");
            return STATUS_ERROR;
        }
    }
    
    Serial.println("\nWiFi Connected successfully!");
    unsigned long t2 = millis();
    Serial.printf("Took %u ms to connect\n", (t2 - t1));
    Serial.println(WiFi.localIP());


    _lastReconnectAttempt = millis();

    return STATUS_OK;
}

bool MyWifi::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

status_t MyWifi::update() {
    if (WiFi.status() == WL_CONNECTED) return STATUS_OK;

    unsigned long now = millis();
    if (now - _lastReconnectAttempt >= RECONNECT_INTERVAL_MS) {
        Serial.println("[MyWifi] Attempting reconnect...");
        return connect();
    }
    return STATUS_ERROR;
}

void MyWifi::disconnect(){
    WiFi.disconnect(true);
}

const char* tzInfo = "EST5EDT,M3.2.0,M11.1.0";
const char* ntpServer1 = "north-america.pool.ntp.org";
const char* ntpServer2 = "time.nist.gov";

void syncTimeWithNTP(){
    configTime(0, 0, ntpServer1, ntpServer2);

    // Set time zone
    setenv("TZ", tzInfo, 1);
    tzset();

    // Wait until a valid time is received from the NTP server
    // 1577836800 is the Unix time for Jan 1, 2020
    time_t now = 0;
    while (time(&now) < 1577836800) {
        delay(500);
    }

    

}