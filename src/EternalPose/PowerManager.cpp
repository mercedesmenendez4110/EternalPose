#include "PowerManager.h"

XPowersAXP2101 PMU;

void PowerManager::init() {
    Wire.begin(15, 14); 

    if (!PMU.init(Wire, 15, 14, AXP2101_SLAVE_ADDRESS)) {
        Serial.println("[ERROR] PMU no encontrado");
        return;
    }

    // Configuración que YA tenías
    PMU.enableALDO1();
    PMU.setALDO1Voltage(3300);
    PMU.enableDC1();
    PMU.setDC1Voltage(3300);

    // --- NUEVO: Habilitar los otros canales para la pantalla ---
    PMU.enableALDO2();
    PMU.setALDO2Voltage(3300);
    PMU.enableALDO3();
    PMU.setALDO3Voltage(3300);
    PMU.enableBLDO1(); // A veces el backlight está en BLDO1
    PMU.setBLDO1Voltage(3300);
    
    Serial.println("[POWER] Energía inicializada y canales extra habilitados.");
}