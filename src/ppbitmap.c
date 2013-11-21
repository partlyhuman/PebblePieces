#include "ppbitmap.h"

//integer divides a into b, rounding up instead of down
#define CEILDIV(a, b) ((a + (b-1)) / b)

GBitmap* pp_gbitmap_create(uint16_t w, uint16_t h) {
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

inline void pp_gbitmap_clear(GBitmap *gbitmap, GColor color) {
    memset(gbitmap->addr, (color == 0)? 0 : ~0, gbitmap->row_size_bytes * gbitmap->bounds.size.h);
}

inline void pp_gbitmap_setpixel(GBitmap *gbitmap, uint16_t x, uint16_t y, GColor color) {
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

inline GColor pp_gbitmap_getpixel(GBitmap *gbitmap, uint16_t x, uint16_t y) {
    uint8_t *data = (uint8_t *)gbitmap->addr;
    data += gbitmap->row_size_bytes * y;
    data += (x / 8);
    uint8_t mask = (1 << (x % 8)); // little endian
    return (*data & mask);
}
