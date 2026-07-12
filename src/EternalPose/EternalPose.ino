#include <Arduino.h>
#include "PowerManager.h"
#include "DisplayManager.h"
#include "CompassManager.h"
#include "BLEManager.hpp"

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

    BLEManager::init();
}

void loop() {
    float anguloFinal = 0.0;

    if (BLEManager::hasFix()) {
        float rumboCasa = BLEManager::getBearingToHome();          // hacia dónde está casa (0-360, absoluto)
        float orientacionReloj = CompassManager::getOrientationAngle(); // hacia dónde apunta el reloj ahora
        anguloFinal = fmod((rumboCasa - orientacionReloj + 360.0), 360.0);
    }

    static float lastAngle = -999.0;
    if (abs(anguloFinal - lastAngle) > 0.5) {
        DisplayManager::updateCompass(anguloFinal);
        lastAngle = anguloFinal;
    }

    DisplayManager::update();
}