///////////////////////////////////
// PebblePieces: Reusable Pebble components.
// by Roger Braunstein 2013
//
// PPToaster
// Animated, self-dismissing modal notifications with minimal setup
///////////////////////////////////

#include "pptoaster.h"

#define PPTOASTER_ANIMATION_DURATION 350
#define PPTOASTER_EDGE_MARGIN 4
#define PPTOASTER_HEIGHT 20
#define PPTOASTER_CORNER_RADIUS 2
#define PPTOASTER_TEXT_INSET 1

#define LOG_RECT(R) APP_LOG(APP_LOG_LEVEL_DEBUG, "%d,%d %dx%d", R.origin.x, R.origin.y, R.size.w, R.size.h)

struct PPToasterModule {
  Layer *layer;
  PropertyAnimation *propanim;
  GFont font;
  GRect layer_frame;
  GSize window_size;
  char message[128];
  int show_duration;
  PPToasterAppearDirection direction;
};
struct PPToasterModule pptoaster;

// a few forward declarations for utility functions
void pptoaster_lazy_init();
void pptoaster_bind_to_window();
void pptoaster_destroy_animation();

void pptoaster_draw(struct Layer *layer, GContext *g) {
  static GTextLayoutCacheRef text_layout_cache;

  GRect bounds = layer_get_frame(layer);
  bounds.origin = GPointZero;

  graphics_context_set_fill_color(g, GColorWhite);
  graphics_context_set_stroke_color(g, GColorBlack);
  graphics_context_set_text_color(g, GColorBlack);
  graphics_fill_rect(g, bounds, PPTOASTER_CORNER_RADIUS, GCornersAll);
  graphics_draw_round_rect(g, bounds, PPTOASTER_CORNER_RADIUS);
  
  if (bounds.size.h > PPTOASTER_TEXT_INSET * 2) {
    bounds = grect_crop(bounds, PPTOASTER_TEXT_INSET);
  }

  graphics_draw_text(g, pptoaster.message, pptoaster.font, bounds, GTextOverflowModeFill, GTextAlignmentCenter, text_layout_cache);
}

void pptoaster_handle_animation_out_stopped(Animation *animation, bool finished, void *property_animation_context) {
  if (!finished) return;
  pptoaster_destroy_animation();
  layer_set_hidden(pptoaster.layer, true);
}

void pptoaster_handle_animation_in_stopped(Animation *animation, bool finished, void *property_animation_context) {
  if (!finished) return;
  static GRect toframe;

  GSize size = pptoaster.layer_frame.size;
  toframe.size = size;
  toframe.origin.x = PPTOASTER_EDGE_MARGIN;
  switch (pptoaster.direction) {
    case PPToasterAppearFromBottom:
      toframe.origin.y = pptoaster.window_size.h;
      break;
    case PPToasterAppearFromTop:
      toframe.origin.y = -size.h - PPTOASTER_EDGE_MARGIN;
      break;
    case PPToasterAppearFromMiddle:
      toframe.origin.y = pptoaster.window_size.h / 2;
      toframe.size.h = 1;
      break;
  }

  pptoaster_destroy_animation();
  pptoaster.propanim = property_animation_create_layer_frame(pptoaster.layer, NULL, &toframe);
  Animation *internal_animation = &pptoaster.propanim->animation;
  animation_set_delay(internal_animation, pptoaster.show_duration);
  animation_set_duration(internal_animation, PPTOASTER_ANIMATION_DURATION);
  animation_set_handlers(internal_animation, (AnimationHandlers){.stopped = pptoaster_handle_animation_out_stopped}, NULL);
  animation_schedule(internal_animation);
}

void pptoaster_handle_animation_in_started(Animation *animation, void *ctx) {
  layer_set_hidden(pptoaster.layer, false);
}

void pptoaster_pop(char* message, int show_duration, PPToasterAppearDirection direction) {
  static GRect fromframe, toframe;
  if (show_duration <= 0) return;

  pptoaster_lazy_init();
  pptoaster_bind_to_window();

  strncpy(pptoaster.message, message, sizeof(pptoaster.message));
  pptoaster.show_duration = show_duration;
  pptoaster.direction = direction;

  // destroy existing animation
  if (NULL != pptoaster.propanim) {
    pptoaster_destroy_animation();
  }

  // setup animations
  GSize size = pptoaster.layer_frame.size;
  fromframe.size = toframe.size = size;
  fromframe.origin.x = toframe.origin.x = PPTOASTER_EDGE_MARGIN;
  switch (pptoaster.direction) {
    case PPToasterAppearFromBottom:
      fromframe.origin.y = pptoaster.window_size.h;
      toframe.origin.y = pptoaster.window_size.h - size.h - PPTOASTER_EDGE_MARGIN;
      break;
    case PPToasterAppearFromTop:
      fromframe.origin.y = -size.h - PPTOASTER_EDGE_MARGIN;
      toframe.origin.y = PPTOASTER_EDGE_MARGIN;
      break;
    case PPToasterAppearFromMiddle:
      fromframe.origin.y = pptoaster.window_size.h / 2;
      toframe.origin.y = (pptoaster.window_size.h - size.h) / 2;
      fromframe.size.h = 1;
      break;
  }

  pptoaster.propanim = property_animation_create_layer_frame(pptoaster.layer, &fromframe, &toframe);
  Animation *internal_animation = &pptoaster.propanim->animation;
  animation_set_duration(internal_animation, PPTOASTER_ANIMATION_DURATION);
  animation_set_handlers(internal_animation, (AnimationHandlers){
    .started = pptoaster_handle_animation_in_started,
    .stopped = pptoaster_handle_animation_in_stopped
  }, NULL);
  animation_schedule(internal_animation);
}

void pptoaster_set_system_font(char *font_name) {
  pptoaster.font = fonts_get_system_font(font_name);
}

void pptoaster_destroy_animation() {
  animation_unschedule(&pptoaster.propanim->animation);
  property_animation_destroy(pptoaster.propanim);
  pptoaster.propanim = NULL;
}

void pptoaster_bind_to_window() {
  layer_remove_from_parent(pptoaster.layer);

  Window *window = window_stack_get_top_window();
  pptoaster.window_size = layer_get_frame(window_get_root_layer(window)).size;
  if (!window_get_fullscreen(window)) {
    // manually adjust for status bar ( not reported in root layer height :/ )
    pptoaster.window_size.h -= 15;
  }

  pptoaster.layer_frame = GRect(0, 0, pptoaster.window_size.w - PPTOASTER_EDGE_MARGIN*2, PPTOASTER_HEIGHT);
  layer_set_frame(pptoaster.layer, pptoaster.layer_frame);

  // this should also ensure the toaster is at the top depth
  layer_add_child(window_get_root_layer(window), pptoaster.layer);
}

void pptoaster_lazy_init() {
  if (pptoaster.layer != NULL) {
    return;
  }

  pptoaster.layer = layer_create(GRectZero);
  layer_set_update_proc(pptoaster.layer, pptoaster_draw);
  layer_set_hidden(pptoaster.layer, true);

  // defaults
  pptoaster.message[0] = '\0';
  pptoaster_set_system_font(FONT_KEY_GOTHIC_14);
}

void pptoaster_deinit() {
  if (NULL != pptoaster.propanim) {
    property_animation_destroy(pptoaster.propanim);
  }
  if (pptoaster.layer) {
    layer_remove_from_parent(pptoaster.layer);
    layer_destroy(pptoaster.layer);
  }
}