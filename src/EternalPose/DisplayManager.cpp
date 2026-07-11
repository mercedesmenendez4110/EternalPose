#include "DisplayManager.h"
#include <lvgl.h>
#include <TFT_eSPI.h>
// ----------------------------------------

TFT_eSPI tft = TFT_eSPI();

void DisplayManager::init() {
    Serial.println("[DISPLAY] Iniciando hardware de pantalla...");
    
    tft.begin();
    tft.setRotation(1); 
    
    lv_init();
    
    // Reservamos memoria específicamente en la PSRAM
    // 320 * 20 píxeles * 4 bytes (por ser color de 32 bits)
    uint32_t buf_size = 320 * 20;
    lv_color_t *buf = (lv_color_t *)ps_malloc(buf_size * sizeof(lv_color_t));
    
    if (buf == NULL) {
        Serial.println("[ERROR] No se pudo reservar memoria en PSRAM");
        return;
    }

    static lv_disp_draw_buf_t draw_buf;
    lv_disp_draw_buf_init(&draw_buf, buf, NULL, buf_size);
    
    // ... el resto de tu código de inicialización igual ...
}

void DisplayManager::update() {
    lv_timer_handler(); // En la versión 9 de LVGL se llama lv_timer_handler
}