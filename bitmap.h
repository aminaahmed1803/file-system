// use bitmap to manage free memory
#ifndef BITMAP_H
#define BITMAP_H

#include <stdint.h>
#include <stddef.h> // for size_t

// constants
#define BITMAP_SIZE 4096  // total number of clusters (same as maximum FAT entries)

// data structure to represent the bitmap
typedef struct {
    uint8_t bits[BITMAP_SIZE / 8]; // 8 bits in each uint8_t
} Bitmap;

// function prototypes
void bitmap_init(Bitmap *bitmap); // initialize the bitmap, setting all bits to 0
int bitmap_find_free(const Bitmap *bitmap, int start); // find the first free bit in the bitmap starting from 'start', returns -1 if no free bit is found
void bitmap_set_bit(Bitmap *bitmap, int index); // set a bit to 1
void bitmap_clear_bit(Bitmap *bitmap, int index); // set a bit to 0
int bitmap_test_bit(const Bitmap *bitmap, int index); // test whether a bit is free, returns 0 if free, returns non-zero if allocated

#endif /* BITMAP_H */
