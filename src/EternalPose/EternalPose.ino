#include <Arduino.h>
#include "PowerManager.h"
#include "DisplayManager.h"
#include "CompassManager.h"

void setup() {
    Serial.begin(115200);
    delay(2000); 
    
    Serial.println("[SYSTEM] Iniciando módulos...");
    
    PowerManager::init(); 
    Serial.println("[SYSTEM] PowerManager ok.");

    DisplayManager::init();
    Serial.println("[SYSTEM] DisplayManager ok.");

    // Damos tiempo a que LVGL se acomode antes de crear objetos gráficos
    delay(100);
    
    DisplayManager::createNeedle();
    Serial.println("[SYSTEM] Interfaz gráfica lista.");

    CompassManager::init();
    Serial.println("[SYSTEM] CompassManager ok.");
}

void loop() {
    // Obtenemos el ángulo del sensor
    float angulo = CompassManager::getOrientationAngle();
    
    // Giramos la aguja visualmente
    DisplayManager::updateCompass(angulo);

    // Mantenemos viva la pantalla
    DisplayManager::update();

    delay(20); 
}