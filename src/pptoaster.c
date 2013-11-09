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

void pptoaster_draw(struct Layer *layer, GContext *g) {
  static GTextLayoutCacheRef text_layout_cache;

  GRect bounds = layer_get_frame(layer);
  bounds.origin = GPointZero;

  LOG_RECT(bounds);
  graphics_context_set_fill_color(g, GColorWhite);
  graphics_context_set_stroke_color(g, GColorBlack);
  graphics_context_set_text_color(g, GColorBlack);
  graphics_fill_rect(g, bounds, PPTOASTER_CORNER_RADIUS, GCornersAll);
  graphics_draw_round_rect(g, bounds, PPTOASTER_CORNER_RADIUS);
  
  if (bounds.size.h > PPTOASTER_TEXT_INSET * 2) {
    bounds = grect_crop(bounds, PPTOASTER_TEXT_INSET);
  }

  graphics_draw_text(g, pptoaster.message, pptoaster.font, bounds, GTextOverflowModeFill, GTextAlignmentCenter, text_layout_cache);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Draw-");
}

void pptoaster_handle_animation_out_stopped(Animation *animation, bool finished, void *property_animation_context) {
  property_animation_destroy(pptoaster.propanim);
  layer_set_hidden(pptoaster.layer, true);
  pptoaster.propanim = NULL;
}

void pptoaster_handle_animation_in_stopped(Animation *animation, bool finished, void *property_animation_context) {
  static GRect toframe;

  GSize size = pptoaster.layer_frame.size;
  toframe.size = size;
  toframe.origin = GPoint(PPTOASTER_EDGE_MARGIN, pptoaster.direction? pptoaster.window_size.h : -size.h - PPTOASTER_EDGE_MARGIN);
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

  // property_animation_destroy(pptoaster.propanim); // is this causing the crash??
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

  strncpy(pptoaster.message, message, sizeof(pptoaster.message));
  pptoaster.show_duration = show_duration;
  pptoaster.direction = direction;

  if (NULL != pptoaster.propanim) {
    property_animation_destroy(pptoaster.propanim);
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

void pptoaster_deinit() {
  if (NULL != pptoaster.propanim) {
    property_animation_destroy(pptoaster.propanim);
  }
  if (pptoaster.layer) {
    layer_remove_from_parent(pptoaster.layer);
    layer_destroy(pptoaster.layer);
  }
}

void pptoaster_init(Window *parent) {
  //TODO make sure we haven't already been initialized
  GRect bounds = layer_get_frame(window_get_root_layer(parent));
  GRect toaster_frame = GRect(0, 0, bounds.size.w - PPTOASTER_EDGE_MARGIN*2, PPTOASTER_HEIGHT);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Made layer at size %dx%d", toaster_frame.size.w, toaster_frame.size.h);
  pptoaster.layer = layer_create(toaster_frame);
  pptoaster.layer_frame = toaster_frame;
  pptoaster.window_size = bounds.size;
  if (window_get_fullscreen(parent)) {
    pptoaster.window_size.h -= 10;
  }
  layer_set_update_proc(pptoaster.layer, pptoaster_draw);
  layer_set_hidden(pptoaster.layer, true);
  layer_add_child(window_get_root_layer(parent), pptoaster.layer);

  // defaults
  pptoaster.propanim = NULL;
  pptoaster.message[0] = '\0';
  pptoaster_set_system_font(FONT_KEY_GOTHIC_14);
}
