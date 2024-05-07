#include "bitmap.h"
#include <string.h>

// initialize the bitmap, setting all bits to 0
void bitmap_init(Bitmap *bitmap) {
    memset(bitmap->bits, 0, sizeof(bitmap->bits));
}

// find the first free bit in the bitmap starting from 'start', returns -1 if no free bit is found
int bitmap_find_free(const Bitmap *bitmap, int start) {
    for (int i = start; i < BITMAP_SIZE; ++i) {
        if (!bitmap_test_bit(bitmap, i)) {
            return i;
        }
    }
    return -1;  // No free bit found
}

// set a bit to 1
void bitmap_set_bit(Bitmap *bitmap, int index) {
    bitmap->bits[index / 8] |= (1 << (index % 8));
}

// set a bit to 0
void bitmap_clear_bit(Bitmap *bitmap, int index) {
    bitmap->bits[index / 8] &= ~(1 << (index % 8));
}

// test whether a bit is free, returns 0 if free, returns non-zero if allocated
int bitmap_test_bit(const Bitmap *bitmap, int index) {
    return bitmap->bits[index / 8] & (1 << (index % 8));
}
