#ifndef FAT_H
#define FAT_H

#include "bitmap.h"
#include <stdint.h>
#include <stddef.h> // for size_t

// constants
#define BLOCK_SIZE 512
#define CLUSTER_SIZE BLOCK_SIZE // 1 sector per cluster
#define ENTRIES_PER_BLOCK (BLOCK_SIZE / 2) // 2 bytes per FAT entry, adapted for FAT12
#define FAT_ENTRIES (4096) // max number of entries in FAT12

// FAT entry status
#define FAT_FREE 0x000
#define FAT_EOF 0xFFF
#define FAT_RESERVED 0xFF0 // range from 0xFF0 to 0xFFF could be used for various reserved meanings

// data structure to represent the FAT in memory
typedef struct {
    uint8_t entries[FAT_ENTRIES];  // FAT12 uses 12 bits per entry, but we use a 16-bit array for simplicity in access
} FAT;

// function prototypes
void fat_init(FAT *fat, Bitmap *bitmap); // initialize FAT and bitmap
int fat_allocate_cluster(FAT *fat, Bitmap *bitmap); // allocate a free cluster and return its index
void fat_free_cluster(FAT *fat, Bitmap *bitmap, int cluster); // free a cluster
int fat_get_next_cluster(const FAT *fat, int current_cluster); // get the index of the next cluster in the chain
void fat_set_next_cluster(FAT *fat, int current_cluster, int next_cluster); // set the next cluster in the chain

#endif /* FAT_H */
