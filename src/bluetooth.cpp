#include "bluetooth.h"

Bluetooth::Bluetooth(){
    pServer = NULL;
    pService = NULL;
    pCharacteristic = NULL;
}

void Bluetooth::init(){
    BLEDevice::init("T-Watch");
    pServer = BLEDevice::createServer();
    pService = pServer->createService(SERVICE_UUID);
    pCharacteristic = pService->createCharacteristic(
                                    CHARACTERISTIC_UUID,
                                    BLECharacteristic::PROPERTY_READ |
                                    BLECharacteristic::PROPERTY_WRITE|
                                    BLECharacteristic::PROPERTY_NOTIFY
                                );

    pCharacteristic->setValue("BLE Initiated");

    
}

void Bluetooth::start_advertising(){
    pService->start();

    pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);
    pAdvertising->setMinPreferred(0x12);
    BLEDevice::startAdvertising();

    
    Serial.println("Waiting for client connection...");
}