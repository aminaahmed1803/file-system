#include "fat.h"
#include <string.h>

// initialize FAT and bitmap
void fat_init(FAT *fat, Bitmap *bitmap) {
    memset(fat->entries, 0, sizeof(fat->entries));  // Set all entries to 0 indicating all clusters are free
    bitmap_init(bitmap);  // Initialize the bitmap as all free
}

// allocate a free cluster and return its index
int fat_allocate_cluster(FAT *fat, Bitmap *bitmap) {
    int freeIndex = bitmap_find_free(bitmap, 0);  // find the first free cluster using the bitmap
    if (freeIndex != -1) {
        bitmap_set_bit(bitmap, freeIndex);  // mark this cluster to 1 in bitmap
        fat->entries[freeIndex] = FAT_EOF;  // mark as EOF (last cluster in chain)
    }
    return freeIndex;
}

// free a cluster
void fat_free_cluster(FAT *fat, Bitmap *bitmap, int cluster) {
    if (cluster >= 0 && cluster < FAT_ENTRIES) {
        fat->entries[cluster] = FAT_FREE;
        bitmap_clear_bit(bitmap, cluster);  // set the bit to 0 in bitmap
    }
}

// get the index of the next cluster in the chain
int fat_get_next_cluster(const FAT *fat, int current_cluster) {
    if (current_cluster >= 0 && current_cluster < FAT_ENTRIES) {
        return fat->entries[current_cluster];
    }
    return -1;  // error or EOF
}

// set the next cluster in the chain
void fat_set_next_cluster(FAT *fat, int current_cluster, int next_cluster) {
    if (current_cluster >= 0 && current_cluster < FAT_ENTRIES && next_cluster >= 0 && next_cluster < FAT_ENTRIES) {
        fat->entries[current_cluster] = next_cluster;
    }
}

