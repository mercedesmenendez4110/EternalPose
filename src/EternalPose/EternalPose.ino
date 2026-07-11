#include <Arduino.h>
#include "PowerManager.h"
#include "DisplayManager.h"
#include "CompassManager.h"

void setup() {
    Serial.begin(115200);
    delay(2000); 
    
    Serial.println("[SYSTEM] Iniciando módulos...");
    
    // 1. Energía (Ya confirmado que funciona)
    PowerManager::init(); 
    Serial.println("[SYSTEM] PowerManager ok.");

    // 2. Pantalla: Descomenta la línea de abajo y sube el código
    DisplayManager::init();
    Serial.println("[SYSTEM] DisplayManager ok.");

    // 3. Brújula: Descomenta la línea de abajo y sube el código
    CompassManager::init();
    Serial.println("[SYSTEM] CompassManager ok.");
}

void loop() {
    // Si tienes el DisplayManager activo, descomenta la línea de abajo
    DisplayManager::update();

    // Si tienes el CompassManager activo, puedes imprimir el ángulo
    
    float angulo = CompassManager::getOrientationAngle();
    Serial.print("Angulo: ");
    Serial.println(angulo);
    

    delay(500); 
}