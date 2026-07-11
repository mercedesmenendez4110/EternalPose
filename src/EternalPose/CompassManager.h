#ifndef COMPASS_MANAGER_H
#define COMPASS_MANAGER_H

#include <Arduino.h>
#include <QMI8658.h> // La que instalaste de Lahav Gahli

class CompassManager {
public:
    static void init();
    static float getOrientationAngle(); // Devuelve el ángulo actual
};

#endif