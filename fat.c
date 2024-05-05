#include "fat.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Assuming a simple in-memory FAT for demonstration
static uint32_t *fat_table = NULL;
static uint32_t total_clusters = 0;

void init_fat(uint32_t disk_size, uint32_t cluster_size) {
    total_clusters = disk_size / cluster_size;
    fat_table = malloc(total_clusters * FAT_ENTRY_SIZE);
    if (fat_table == NULL) {
        fprintf(stderr, "Error allocating FAT table.\n");
        exit(EXIT_FAILURE);
    }
    memset(fat_table, 0, total_clusters * FAT_ENTRY_SIZE);
}

uint32_t allocate_cluster(void) {
    for (uint32_t i = 0; i < total_clusters; i++) {
        if (fat_table[i] == 0) { // 0 indicates a free cluster
            fat_table[i] = EOF_CLUSTER; // Mark as end of the file
            return i;
        }
    }
    return EOF_CLUSTER; // No free cluster found
}

void free_cluster(uint32_t cluster) {
    if (cluster < total_clusters) {
        fat_table[cluster] = 0; // Mark as free
    }
}

uint32_t get_next_cluster(uint32_t cluster) {
    if (cluster < total_clusters) {
        return fat_table[cluster];
    }
    return EOF_CLUSTER;
}

void set_next_cluster(uint32_t current_cluster, uint32_t next_cluster) {
    if (current_cluster < total_clusters) {
        fat_table[current_cluster] = next_cluster;
    }
}

