#include "CompassManager.h"
#include <Wire.h>

QMI8658 sensor;

void CompassManager::init() {
    sensor.begin(); 
    Serial.println("[COMPASS] Sensor QMI8658 iniciado.");
}

float CompassManager::getOrientationAngle() {
    // CAMBIO: Ahora usamos float, tal como exige la librería
    float ax, ay, az;
    
    // Ahora los tipos coinciden perfectamente
    sensor.readAccel(ax, ay, az);
    
    // Calculamos el ángulo usando los valores float directos
    float angle = atan2(ay, ax) * 180.0 / PI;
    return angle;
}