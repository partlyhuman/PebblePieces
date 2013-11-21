#include <pebble.h>
#define NDEBUG
#include <assert.h>
#include "ppbitmap.h"

#define MIN(a, b) ((a<b)? a : b)

static Window *window;
static GBitmap *bmp;

static void update(void *ctx) {
    static uint8_t counter;
    counter++;
    ppbmp_clear(bmp, GColorWhite);
    
    //halftone
    for (uint16_t y = 0; y < ppbmp_height(bmp); y += 2) {
        for (uint16_t x = (counter++) % 2; x < ppbmp_width(bmp); x += 2) {
            ppbmp_setpixel(bmp, x, y, GColorBlack);
        }
    }
    
    //diagonal UL->BR
    uint16_t upper = MIN(ppbmp_width(bmp), ppbmp_height(bmp));
    for (uint16_t y = 0; y < upper; y++) {
        ppbmp_setpixel(bmp, y, y, GColorBlack);
    }
    
    //block
    ppbmp_fillrect(bmp, GRect(10, 10, 10 + (counter % 10), 10 + (counter % 10)), GColorBlack);
    
    
    layer_mark_dirty(bitmap_layer_get_layer((BitmapLayer *)ctx));
    app_timer_register(100, update, ctx);
}

static void window_load(Window *window) {
    Layer *windowlayer = window_get_root_layer(window);
    
    //    GRect rect = GRect(10, 10, 60, 20);
    //    TextLayer *textlayer = text_layer_create(rect);
    //    text_layer_set_text_color(textlayer, GColorWhite);
    //    text_layer_set_background_color(textlayer, GColorBlack);
    //    text_layer_set_text(textlayer, "Hello world");
    //    layer_add_child(windowlayer, text_layer_get_layer(textlayer));
    
    bmp = ppbmp_gbitmap_create(100, 124);
    uint16_t y = 50;
    for (uint16_t x = 0; x < ppbmp_width(bmp); x++) {
        ppbmp_setpixel(bmp, x, y, GColorWhite);
    }
    y = 51;
    for (uint16_t x = 0; x < ppbmp_width(bmp); x++) {
        ppbmp_setpixel(bmp, x, y, GColorBlack);
    }
    assert(GColorWhite == pp_gbitmap_getpixel(bmp, 50, 50));
    assert(GColorBlack == pp_gbitmap_getpixel(bmp, 51, 51));
    
    
    GRect bounds = bmp->bounds;
    bounds.origin = GPoint(22, 22);
    BitmapLayer *bitmaplayer = bitmap_layer_create(bounds);
    bitmap_layer_set_bitmap(bitmaplayer, bmp);
    layer_add_child(windowlayer, bitmap_layer_get_layer(bitmaplayer));
    
    app_timer_register(1500, update, bitmaplayer);
}

static void window_unload(Window *window) {
    gbitmap_destroy(bmp);
}

void init_ppbitmap_test(void) {
    window = window_create();
    window_set_fullscreen(window, true);
    //  window_set_click_config_provider(window, click_config_provider);
    window_set_window_handlers(window, (WindowHandlers) {
        .load = window_load,
        .unload = window_unload,
    });
    window_stack_push(window, false);
}