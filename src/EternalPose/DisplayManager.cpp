#include "DisplayManager.h"
#include <Arduino.h>
#include <lvgl.h>
#include <Arduino_GFX_Library.h>

// Tus definiciones de pines...
static const int LCD_SDIO0 = 4;
static const int LCD_SDIO1 = 5;
static const int LCD_SDIO2 = 6;
static const int LCD_SDIO3 = 7;
static const int LCD_SCLK = 11;
static const int LCD_CS = 12;
static const int LCD_RESET = 8;
static const int LCD_WIDTH = 410;
static const int LCD_HEIGHT = 502;

static Arduino_DataBus *bus = new Arduino_ESP32QSPI(LCD_CS, LCD_SCLK, LCD_SDIO0, LCD_SDIO1, LCD_SDIO2, LCD_SDIO3);
static Arduino_GFX *gfx = new Arduino_CO5300(bus, LCD_RESET, 0, LCD_WIDTH, LCD_HEIGHT, 22, 0, 0, 0);

static lv_disp_draw_buf_t draw_buf; // Buffer v8
static lv_color_t *buf;
static lv_obj_t *needle_obj;

// Callback de flush para LVGL v8
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
    
    // Buffer para LVGL v8
    uint32_t buf_size = gfx->width() * 50;
    buf = (lv_color_t *)heap_caps_malloc(buf_size * sizeof(lv_color_t), MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
    lv_disp_draw_buf_init(&draw_buf, buf, NULL, buf_size);

    // Registro del display v8
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = gfx->width();
    disp_drv.ver_res = gfx->height();
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0x0000FF), 0);
}

void DisplayManager::createNeedle() {
    needle_obj = lv_obj_create(lv_scr_act());
    lv_obj_set_size(needle_obj, 160, 20); 
    lv_obj_center(needle_obj);
    lv_obj_set_style_bg_color(needle_obj, lv_palette_main(LV_PALETTE_RED), 0);
    lv_obj_set_style_radius(needle_obj, 10, 0);
    // En LVGL v8 usamos el sistema de transformación de estilo
    lv_obj_set_style_transform_pivot_x(needle_obj, 80, 0);
    lv_obj_set_style_transform_pivot_y(needle_obj, 10, 0);
}

void DisplayManager::updateCompass(float angle) {
    int32_t lvgl_angle = (int32_t)(angle * 10);
    lv_obj_set_style_transform_angle(needle_obj, lvgl_angle, 0);
}

void DisplayManager::update() {
    lv_timer_handler();
}