#pragma once
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <Arduino.h>


#define SERVICE_UUID "af6fa958-d99f-49a6-a439-64e6cec72ae2"
#define CHARACTERISTIC_UUID "6005b371-4b62-41cc-abdc-9a701debe2e7"

class Bluetooth{
    public:

        Bluetooth();
        void init();
        void start_advertising();

    private:
        BLEServer *pServer;
        BLECharacteristic *pCharacteristic;
        BLEService *pService;
        BLEAdvertising *pAdvertising;
};