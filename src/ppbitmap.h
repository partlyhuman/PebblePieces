#include <pebble.h>

GBitmap* pp_gbitmap_create(uint16_t w, uint16_t h);
void pp_gbitmap_clear(GBitmap *gbitmap, GColor color);
void pp_gbitmap_setpixel(GBitmap *gbitmap, uint16_t x, uint16_t y, GColor color);
GColor pp_gbitmap_getpixel(GBitmap *gbitmap, uint16_t x, uint16_t y);

#define pp_gbitmap_width(gbitmap) gbitmap->bounds.size.w
#define pp_gbitmap_height(gbitmap) gbitmap->bounds.size.h