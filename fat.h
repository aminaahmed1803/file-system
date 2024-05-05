#ifndef FAT_H
#define FAT_H

#include <stdint.h>
#include <stdbool.h>

#define FAT_ENTRY_SIZE 4 // FAT32 uses 4 bytes per FAT entry
#define EOF_CLUSTER 0x0FFFFFFF // End of file marker for FAT32

// Initialize the File Allocation Table
void init_fat(uint32_t disk_size, uint32_t cluster_size);

// Allocate a new cluster in the FAT
uint32_t allocate_cluster(void);

// Free a cluster in the FAT
void free_cluster(uint32_t cluster);

// Get the next cluster in a chain from the FAT
uint32_t get_next_cluster(uint32_t cluster);

// Set the next cluster in a chain in the FAT
void set_next_cluster(uint32_t current_cluster, uint32_t next_cluster);

#endif
