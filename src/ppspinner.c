///////////////////////////////////
// PebblePieces: Reusable Pebble components.
// by Roger Braunstein 2013
//
// PPSpinner
// Animated, customizable "stepping bar" indefinite loader
///////////////////////////////////

#include "ppspinner.h"

typedef struct {

  // "public"
  int segments;
  int corner_radius;
  int update_ms;

  // "private"
  int current_frame;
  AppTimer *timer;

} PPSpinnerData;


static void ppspinner_handle_timer(void *layer) {
  PPSpinnerData *data = (PPSpinnerData *)layer_get_data(layer);
  data->current_frame++;
  data->timer = app_timer_register(data->update_ms, ppspinner_handle_timer, layer);
  layer_mark_dirty((Layer *)layer);
}

static void ppspinner_update(Layer *layer, GContext *g) {
  PPSpinnerData *data = (PPSpinnerData *)layer_get_data(layer);
  GRect bounds = layer_get_frame(layer);
  // cache some values on stack
  int frame_number = data->current_frame;
  int corner_radius = data->corner_radius;
  int segments = data->segments; 

  // # w = width of segment # g = width of gap # z = ratio of width to gap # s = segments
  // # we know w, s, z # we want to find w, g
  // all_width = w s + g (s-1) // g = w z
  // all_width = w s + (w z)(s-1)
  // w = all_width / (s z + s - z) 
  float gap_to_piece_ratio = 0.25;
  int piece_width = (bounds.size.w + 0.0) / (segments * gap_to_piece_ratio + segments - gap_to_piece_ratio);
  int piece_gap = piece_width * gap_to_piece_ratio;
  int piece_height = bounds.size.h;

  graphics_context_set_stroke_color(g, GColorBlack);
  graphics_context_set_fill_color(g, GColorBlack);
  for (int i = 0, x = 0; i < segments; i++, x += piece_width + piece_gap) {
    GRect piece_frame = GRect(x, 0, piece_width, piece_height);
    graphics_draw_round_rect(g, piece_frame, corner_radius);
    if (frame_number % segments == i) {
      graphics_fill_rect(g, piece_frame, corner_radius, GCornersAll);
    }
  }
}

void ppspinner_start(PPSpinnerLayer *layer) {
  PPSpinnerData *data = (PPSpinnerData *)layer_get_data(layer);
  if (data->timer != NULL) {
    APP_LOG(APP_LOG_LEVEL_WARNING, "Already running");
  } else {
    data->timer = app_timer_register(data->update_ms, ppspinner_handle_timer, layer);
  }
}

void ppspinner_stop(PPSpinnerLayer *layer) {
  PPSpinnerData *data = (PPSpinnerData *)layer_get_data(layer);
  if (data->timer == NULL) {
    APP_LOG(APP_LOG_LEVEL_WARNING, "Already stopped");
  } else {
    app_timer_cancel(data->timer);
    data->timer = NULL;
  }
}

void ppspinner_destroy(PPSpinnerLayer *spinner) {
  ppspinner_stop(spinner);
  layer_destroy(spinner);
}

PPSpinnerLayer *ppspinner_create(GRect frame, int segments, int corner_radius, int update_ms) {
  PPSpinnerLayer *layer = layer_create_with_data(frame, sizeof(PPSpinnerData));
  layer_set_update_proc(layer, ppspinner_update);

  PPSpinnerData *data = (PPSpinnerData *)layer_get_data(layer);
  data->segments = segments;
  data->update_ms = update_ms;
  data->corner_radius = corner_radius;
  data->current_frame = 0;
  data->timer = NULL;

  return layer;
}
