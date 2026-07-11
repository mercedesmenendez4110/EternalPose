#include "PowerManager.h"

XPowersAXP2101 PMU;

void PowerManager::init() {
    // Usamos los pines correctos del diagrama para SDA (15) y SCL (14)
    Wire.begin(15, 14); 

    if (!PMU.init(Wire, 15, 14, AXP2101_SLAVE_ADDRESS)) {
        Serial.println("[ERROR] PMU no encontrado");
        return;
    }

    // Configuración base de energía
    PMU.enableALDO1();
    PMU.setALDO1Voltage(3300);
    
    PMU.enableDC1();
    PMU.setDC1Voltage(3300);

    Serial.println("[POWER] Energía inicializada.");
}