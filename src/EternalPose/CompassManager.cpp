#include "CompassManager.h"
#include <Wire.h>

QMI8658 sensor;

void CompassManager::init() {
    sensor.begin();
    sensor.enableAccel(true);
    sensor.enableGyro(true);
    sensor.setGyroUnit_dps(true);
    Serial.println("[COMPASS] Sensor QMI8658 iniciado.");
}

float CompassManager::getOrientationAngle() {
    float gx, gy, gz;
    if (sensor.readGyroDPS(gx, gy, gz)) {
        static float currentHeading = 0.0f;
        static unsigned long lastMillis = 0;
        unsigned long now = millis();
        if (lastMillis == 0) {
            lastMillis = now;
            return currentHeading;
        }

        float dt = (now - lastMillis) / 1000.0f;
        lastMillis = now;
        currentHeading += gz * dt;
        if (currentHeading >= 360.0f) currentHeading -= 360.0f;
        if (currentHeading < 0.0f) currentHeading += 360.0f;
        return currentHeading;
    }

    float ax, ay, az;
    if (sensor.readAccel(ax, ay, az)) {
        float angle = atan2(ay, ax) * 180.0 / PI;
        return angle;
    }

    return 0.0f;
}
// En CompassManager.cpp, reemplazá calibrate() por esto:
void CompassManager::calibrate(float realHeading) {
    // Mezcla suave: 90% confía en el giroscopio acumulado, 10% se acerca al dato del celular
    float diff = fmod((realHeading - currentHeading + 540.0), 360.0) - 180.0; // diferencia más corta
    currentHeading += diff * 0.10f;
    if (currentHeading >= 360.0f) currentHeading -= 360.0f;
    if (currentHeading < 0.0f) currentHeading += 360.0f;
}