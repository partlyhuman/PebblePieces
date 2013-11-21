///////////////////////////////////
// PebblePieces: Reusable Pebble components.
// by Roger Braunstein 2013
//
// PPBitmap
// Missing utilities for in-memory bitmap manipulation
///////////////////////////////////

#include "ppbitmap.h"

// TODO entire module needs bounds and sanity checking
// right now every method could easily crash your app, so don't be stupid.

// integer divides a into b, rounding up instead of down
#define CEILDIV(a, b) ((a + (b-1)) / b)

GBitmap* ppbmp_gbitmap_create(uint16_t w, uint16_t h) {
    uint16_t row_size_bytes = CEILDIV(w, 8); // 8 bits per byte
    row_size_bytes = CEILDIV(row_size_bytes, 4) * 4; // round up to nearest 32 bit word
    uint8_t *bytes = (uint8_t *)malloc(h * row_size_bytes);
    memset((void *)bytes, 0, sizeof(bytes));
    GBitmap *gbitmap = (GBitmap *)malloc(sizeof(GBitmap));

    gbitmap->addr = bytes;
    gbitmap->row_size_bytes = row_size_bytes;
    gbitmap->bounds = GRect(0, 0, w, h);
    gbitmap->is_heap_allocated = true;
    // not sure if this is legal but think it's safest to zero out
    gbitmap->reserved = 0;
    gbitmap->version = 0;
    return gbitmap;
}

inline void ppbmp_clear(GBitmap *gbitmap, GColor color) {
    memset(gbitmap->addr, (color == 0)? 0 : ~0, gbitmap->row_size_bytes * gbitmap->bounds.size.h);
}

inline void ppbmp_setpixel(GBitmap *gbitmap, uint16_t x, uint16_t y, GColor color) {
    uint8_t *data = (uint8_t *)gbitmap->addr;
    data += gbitmap->row_size_bytes * y;
    data += (x / 8);
    uint8_t mask = (1 << (x % 8)); // little endian
    if (color == 1) {
        *data |= mask;
    } else if (color == 0) {
        *data &= ~mask;
    }
}

inline GColor ppbmp_getpixel(GBitmap *gbitmap, uint16_t x, uint16_t y) {
    uint8_t *data = (uint8_t *)gbitmap->addr;
    data += gbitmap->row_size_bytes * y;
    data += (x / 8);
    uint8_t mask = (1 << (x % 8)); // little endian
    return ((*data & mask) >> (x % 8));
}

void ppbmp_fillrect(GBitmap *gbitmap, GRect rect, GColor color) {
    // TODO naive implementation could use some optimization?
    for (uint16_t y = rect.origin.y; y < rect.origin.y + rect.size.h; y++) {
        for (uint16_t x = rect.origin.x; x < rect.origin.x + rect.size.w; x++) {
            ppbmp_setpixel(gbitmap, x, y, color);
        }
    }
}
