#include <pebble.h>
#include "ppspinner.h"
#include "pptoaster.h"

///////////////////////////////////
// PebblePieces: Reusable Pebble components.
// Partlyhuman inc. 2013
// 
// This file serves as a demonstration of the available components.
//////////////////////////////////

char* toaster_test_strings[] = {
  "ERROR 404!",
  "Geolocating you...",
  "Communicating with phone...",
  "Loading..."
};

static Window *window;
static Layer *demo_layers[1];

static void demo_layers_push(Layer *layer) {
  static int demo_layer_current = 0;
  demo_layers[demo_layer_current++] = layer;
}

static void change_demo_layer_by(int delta) {  
  static int displayed_layer_number = -1;
  static Layer *displayed_layer = NULL;

  switch (displayed_layer_number) {
    case 0: // spinner
      ppspinner_stop(displayed_layer);
      break;
  }

  if (displayed_layer_number >= 0) {
    layer_remove_from_parent(demo_layers[displayed_layer_number]);
  }
  displayed_layer_number = (displayed_layer_number + delta) % ARRAY_LENGTH(demo_layers);
  displayed_layer = demo_layers[displayed_layer_number];
  layer_add_child(window_get_root_layer(window), displayed_layer);

  switch (displayed_layer_number) {
    case 0: // spinner
      ppspinner_start(displayed_layer);
      break;
  }
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {

  char *message = toaster_test_strings[rand() % ARRAY_LENGTH(toaster_test_strings)];
  pptoaster_pop(message, 1500, (rand() % 3));
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  change_demo_layer_by(+1);
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  change_demo_layer_by(-1);
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void window_load(Window *window) {
  // Layer *window_layer = window_get_root_layer(window);
  // GRect bounds = layer_get_bounds(window_layer);

  demo_layers_push(ppspinner_create(
    GRect(20, 20, 25, 10), //frame
    3, //segments
    2, //corner radius
    250 //ms between updates
  ));
  change_demo_layer_by(+1);

  pptoaster_init(window);
}

static void window_unload(Window *window) {

}

static void init(void) {
  window = window_create();
  window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  window_stack_push(window, false);
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
