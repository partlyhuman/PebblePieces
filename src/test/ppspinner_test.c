#include <pebble.h>
#include "ppspinner.h"

static Window *window;
PPSpinnerLayer *spinner;

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
    ppspinner_start(spinner);
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
    ppspinner_stop(spinner);
}

static void click_config_provider(void *context) {
    window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
    window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void window_load(Window *window) {
    spinner = ppspinner_create(
                               GRect(20, 20, 25, 10), //frame
                               3, //segments
                               2, //corner radius
                               250 //ms between updates
                               );
    layer_add_child(window_get_root_layer(window), spinner);
    ppspinner_start(spinner);
}

static void window_unload(Window *window) {
    ppspinner_destroy(spinner);
}

void init_ppspinner_test() {
    window = window_create();
    window_set_click_config_provider(window, click_config_provider);
    window_set_window_handlers(window, (WindowHandlers) {
        .load = window_load,
        .unload = window_unload,
    });
    window_stack_push(window, false);
}
