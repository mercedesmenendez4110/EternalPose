#include "DisplayManager.h"
#include <Arduino.h>
#include <lvgl.h>
#include <Arduino_GFX_Library.h>

// Definiciones de hardware
static const int LCD_WIDTH = 410;
static const int LCD_HEIGHT = 502;
static const int LCD_SCLK = 11;
static const int LCD_CS = 12;
static const int LCD_SDIO0 = 4;
static const int LCD_SDIO1 = 5;
static const int LCD_SDIO2 = 6;
static const int LCD_SDIO3 = 7;
static const int LCD_RESET = 8;

static Arduino_DataBus *bus = new Arduino_ESP32QSPI(LCD_CS, LCD_SCLK, LCD_SDIO0, LCD_SDIO1, LCD_SDIO2, LCD_SDIO3);
static Arduino_GFX *gfx = new Arduino_CO5300(bus, LCD_RESET, 0, LCD_WIDTH, LCD_HEIGHT, 22, 0, 0, 0);

static lv_disp_draw_buf_t draw_buf;
static lv_color_t *draw_buf_ptr; // Renombrado para evitar confusión
static lv_obj_t *canvas;

// Búfer para el lienzo de la aguja (200x200 para ahorrar RAM)
static lv_color_t canvas_buf[200 * 200]; 

void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);
    gfx->draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);
    lv_disp_flush_ready(disp);
}

void DisplayManager::init() {
    gfx->begin();
    gfx->fillScreen(0x0000);

    lv_init();
    uint32_t buf_size = gfx->width() * 50;
    draw_buf_ptr = (lv_color_t *)heap_caps_malloc(buf_size * sizeof(lv_color_t), MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
    lv_disp_draw_buf_init(&draw_buf, draw_buf_ptr, NULL, buf_size);

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = gfx->width();
    disp_drv.ver_res = gfx->height();
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);
}

void DisplayManager::createNeedle() {
    canvas = lv_canvas_create(lv_scr_act());
    lv_canvas_set_buffer(canvas, canvas_buf, 200, 200, LV_IMG_CF_TRUE_COLOR);
    lv_obj_center(canvas); // Centramos el canvas en la pantalla
    lv_canvas_fill_bg(canvas, lv_color_hex(0x000000), LV_OPA_COVER);
}

void DisplayManager::updateCompass(float angle) {
    if(canvas == NULL) return;
    
    // 1. Borramos el canvas anterior (rellenamos de negro)
    lv_canvas_fill_bg(canvas, lv_color_hex(0x000000), LV_OPA_COVER);
    
    // 2. Calculamos (usamos los mismos valores que funcionaron)
    int centerX = 100;
    int centerY = 100;
    int length = 80;
    
    // Ajuste de ángulo: LVGL/Matemáticas
    float rad = (angle - 90) * PI / 180.0;
    int x2 = centerX + (int)(length * cos(rad));
    int y2 = centerY + (int)(length * sin(rad));
    
    // 3. Dibujamos
    lv_point_t points[2] = {{centerX, centerY}, {x2, y2}};
    lv_draw_line_dsc_t line_dsc;
    lv_draw_line_dsc_init(&line_dsc);
    line_dsc.color = lv_palette_main(LV_PALETTE_RED);
    line_dsc.width = 5;
    
    lv_canvas_draw_line(canvas, points, 2, &line_dsc);
    
    // 4. ¡IMPORTANTE! Forzamos el redibujado de la pantalla
    lv_obj_invalidate(canvas);
}

void DisplayManager::update() {
    static uint32_t lastTick = 0;
    uint32_t now = millis();
    if (lastTick == 0) lastTick = now; // primera llamada
    lv_tick_inc(now - lastTick);       // <-- ESTO FALTABA: le avisa a LVGL cuánto tiempo pasó
    lastTick = now;

    lv_timer_handler();
}