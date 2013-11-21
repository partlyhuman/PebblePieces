///////////////////////////////////
// PebblePieces: Reusable Pebble components.
// by Roger Braunstein 2013
//
// PPBitmap
// Missing utilities for in-memory bitmap manipulation
///////////////////////////////////

#include <pebble.h>

GBitmap* ppbmp_gbitmap_create(uint16_t w, uint16_t h);
void ppbmp_clear(GBitmap *gbitmap, GColor color);
void ppbmp_fillrect(GBitmap *gbitmap, GRect rect, GColor color);
void ppbmp_setpixel(GBitmap *gbitmap, uint16_t x, uint16_t y, GColor color);
GColor ppbmp_getpixel(GBitmap *gbitmap, uint16_t x, uint16_t y);

#define ppbmp_width(gbitmap) gbitmap->bounds.size.w
#define ppbmp_height(gbitmap) gbitmap->bounds.size.h