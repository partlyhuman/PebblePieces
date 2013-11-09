///////////////////////////////////
// PebblePieces: Reusable Pebble components.
// by Roger Braunstein 2013
//
// PPSpinner
// Animated, customizable "stepping bar" indefinite loader
///////////////////////////////////

#pragma once
#include <pebble.h>

typedef Layer PPSpinnerLayer;

PPSpinnerLayer *ppspinner_create(GRect frame, int segments, int corner_radius, int update_ms);

void ppspinner_destroy(PPSpinnerLayer *);
void ppspinner_start(PPSpinnerLayer *);
void ppspinner_stop(PPSpinnerLayer *);
