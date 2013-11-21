#include <pebble.h>
#include "ppbitmap.h"

#define MIN(a, b) ((a<b)? a : b)

static Window *window;
static GBitmap *bmp;

static void update(void *ctx) {
    static uint8_t counter;
    counter++;
    pp_gbitmap_clear(bmp, GColorWhite);
    
    //halftone
    for (uint16_t y = 0; y < pp_gbitmap_height(bmp); y += 2) {
        for (uint16_t x = (counter++) % 2; x < pp_gbitmap_width(bmp); x += 2) {
            pp_gbitmap_setpixel(bmp, x, y, GColorBlack);
        }
    }
    
    //diagonal UL->BR
    uint16_t upper = MIN(pp_gbitmap_width(bmp), pp_gbitmap_height(bmp));
    for (uint16_t y = 0; y < upper; y++) {
        pp_gbitmap_setpixel(bmp, y, y, GColorBlack);
    }
    
    
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
    
    bmp = pp_gbitmap_create(100, 124);
    GRect bounds = bmp->bounds;
    bounds.origin = GPoint(22, 22);
    BitmapLayer *bitmaplayer = bitmap_layer_create(bounds);
    bitmap_layer_set_bitmap(bitmaplayer, bmp);
    layer_add_child(windowlayer, bitmap_layer_get_layer(bitmaplayer));
    
    app_timer_register(100, update, bitmaplayer);
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