#pragma once
#include <pebble.h>

typedef enum {
  PPToasterAppearFromBottom = 0,
  PPToasterAppearFromTop,
  PPToasterAppearFromMiddle,
} PPToasterAppearDirection;

void pptoaster_pop(char* message, int duration, PPToasterAppearDirection direction);

void pptoaster_init(Window *parent);
void pptoaster_deinit();
void pptoaster_set_system_font(char* font_name);
