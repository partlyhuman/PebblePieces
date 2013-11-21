#include <pebble.h>
#include "pptoaster.h"

static Window *window;
char* toaster_test_strings[] = {
    "Accepted",
    "Geolocating you...",
    "Talking to phone...",
    "Loading..."
};

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
    char *message = toaster_test_strings[rand() % ARRAY_LENGTH(toaster_test_strings)];
    pptoaster_pop(message, 200, (rand() % 3));
}

static void click_config_provider(void *context) {
    window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
}

void init_pptoaster_test() {
    window = window_create();
    window_set_click_config_provider(window, click_config_provider);
    window_stack_push(window, false);
}

void deinit_pptoaster_test() {
    pptoaster_deinit();
}
