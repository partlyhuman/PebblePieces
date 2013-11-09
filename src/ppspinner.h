#pragma once
#include <pebble.h>

typedef Layer PPSpinnerLayer;

PPSpinnerLayer *ppspinner_create(GRect frame, int segments, int speed, int corner_radius);
void ppspinner_destroy(PPSpinnerLayer *);
void ppspinner_start(PPSpinnerLayer *);
void ppspinner_stop(PPSpinnerLayer *);
