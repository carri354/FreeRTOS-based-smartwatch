#pragma once
#include <cmath>
#include <WiFi.h>
#include "esp_wpa2.h"
#include "dbg.h"

constexpr unsigned long WIFI_TIMEOUT_MS = 15000;

void syncTimeWithNTP();

class MyWifi {
public:
/**
 * TODO
 * WRITE YOUR PUBLIC MEMBERS AND FUNCTION HEADERS HERE
 */

    MyWifi() = default;

    status_t begin(const char* identity, const char* username, const char* password);

    bool isConnected();
    status_t update(); // call from main loop for reconnect handling
    void disconnect();
private:
/**
 * TODO
 * WRITE YOUR PRIVATE MEMBERS AND FUNCTIONS HEADERS HERE
 */
    static constexpr const char* SSID = "eduroam";
    static constexpr unsigned long RECONNECT_INTERVAL_MS = 10000;

    String _identity;
    String _username;
    String _password;
    unsigned long _lastReconnectAttempt = 0;

    status_t connect();
    
};