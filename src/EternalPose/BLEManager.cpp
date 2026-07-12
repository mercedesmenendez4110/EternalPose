#include "BLEManager.hpp"
#include "CompassManager.h"

volatile float BLEManager::phoneLat = 0.0;
volatile float BLEManager::phoneLon = 0.0;
volatile bool  BLEManager::gotFix = false;

void BLEManager::setCoordinates(float lat, float lon) {
    phoneLat = lat;
    phoneLon = lon;
    gotFix = true;
}

bool BLEManager::hasFix() {
    return gotFix;
}

float BLEManager::calcularBearing(float lat1, float lon1, float lat2, float lon2) {
    float phi1 = radians(lat1);
    float phi2 = radians(lat2);
    float dLon = radians(lon2 - lon1);

    float y = sin(dLon) * cos(phi2);
    float x = cos(phi1) * sin(phi2) - sin(phi1) * cos(phi2) * cos(dLon);
    float bearing = degrees(atan2(y, x));
    return fmod((bearing + 360.0), 360.0);
}

float BLEManager::getBearingToHome() {
    if (!gotFix) return -1.0;
    return calcularBearing(phoneLat, phoneLon, HOME_LAT, HOME_LON);
}

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
        String value = pCharacteristic->getValue().c_str();
        if (value.length() == 0) return;

        if (value.startsWith("G:")) {
            String data = value.substring(2);
            int comaPos = data.indexOf(',');
            if (comaPos > 0) {
                float lat = data.substring(0, comaPos).toFloat();
                float lon = data.substring(comaPos + 1).toFloat();
                BLEManager::setCoordinates(lat, lon);
                Serial.printf("GPS recibido -> lat: %.6f, lon: %.6f\n", lat, lon);
            }
        }
        else if (value.startsWith("C:")) {
            float realHeading = value.substring(2).toFloat();
            CompassManager::calibrate(realHeading);
            Serial.printf("Calibracion recibida -> %.1f\n", realHeading);
        }
    }
};

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
        Serial.println("¡CLIENTE CONECTADO!");
    };
    void onDisconnect(BLEServer* pServer) {
        Serial.println("Cliente desconectado.");
        BLEDevice::startAdvertising();
    }
};

void BLEManager::init() {
    BLEDevice::init("EternalPose_Watch");
    BLEServer *pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());
    BLEService *pService = pServer->createService(SERVICE_UUID);
    BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );
    pCharacteristic->setCallbacks(new MyCallbacks());
    pService->start();

    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->start();
    Serial.println("BLE iniciado. Esperando conexion...");
}