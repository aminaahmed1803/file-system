#ifndef DISK_H
#define DISK_H

#include <stdint.h>
#include <stddef.h>

#define BLOCK_SIZE 512
#define BLOCKS_PER_CLUSTER 1

// function prototypes
void disk_init(const char *disk_image_path);
void* disk_read_cluster(int clusterNumber);
void disk_write_cluster(int clusterNumber, const void* data);
int cluster_to_sector(int clusterNumber);
void disk_cleanup();

#endif /* DISK_H */
