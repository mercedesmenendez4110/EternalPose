#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Arduino.h>

class DisplayManager {
public:
    static void init();
    static void createNeedle();
    static void updateCompass(float angle);
    static void update();
};

#endif