///////////////////////////////////
// PebblePieces: Reusable Pebble components.
// by Roger Braunstein 2013
//
// PPToaster
// Animated, self-dismissing modal notifications with minimal setup
///////////////////////////////////

#pragma once
#include <pebble.h>

typedef enum {
  PPToasterAppearFromBottom = 0,
  PPToasterAppearFromTop,
  PPToasterAppearFromMiddle,
} PPToasterAppearDirection;

void pptoaster_pop(char* message, int duration, PPToasterAppearDirection direction);
void pptoaster_set_system_font(char* font_name);
void pptoaster_deinit();
