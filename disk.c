#include "disk.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static FILE *diskImage = NULL;

void disk_init(const char *disk_image_path) {
    diskImage = fopen(disk_image_path, "r+b");  // Open the disk image for reading and writing
    if (!diskImage) {
        perror("Failed to open disk image");
        exit(EXIT_FAILURE);
    }
}

void* disk_read_cluster(int clusterNumber) {
    int sectorNumber = cluster_to_sector(clusterNumber);
    void *buffer = malloc(BLOCK_SIZE * BLOCKS_PER_CLUSTER);
    if (!buffer) {
        perror("Memory allocation failed");
        return NULL;
    }
    fseek(diskImage, sectorNumber * BLOCK_SIZE, SEEK_SET);
    if (fread(buffer, BLOCK_SIZE, BLOCKS_PER_CLUSTER, diskImage) != BLOCKS_PER_CLUSTER) {
        perror("Failed to read disk");
        free(buffer);
        return NULL;
    }
    return buffer;
}

void disk_write_cluster(int clusterNumber, const void* data) {
    int sectorNumber = cluster_to_sector(clusterNumber);
    fseek(diskImage, sectorNumber * BLOCK_SIZE, SEEK_SET);
    if (fwrite(data, BLOCK_SIZE, BLOCKS_PER_CLUSTER, diskImage) != BLOCKS_PER_CLUSTER) {
        perror("Failed to write to disk");
    }
}

int cluster_to_sector(int clusterNumber) {
    // Assuming each cluster corresponds directly to one sector
    return clusterNumber * BLOCKS_PER_CLUSTER;
}

void disk_cleanup() {
    if (diskImage) {
        fclose(diskImage);
        diskImage = NULL;
    }
}
