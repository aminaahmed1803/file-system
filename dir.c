#include "dir.h"
#include <string.h>  // For memset and memcpy
#include <stdio.h>   // For printf

// Initialize a directory on a specific cluster
void directory_init(FAT *fat, Bitmap *bitmap, DirEntry *dir, int startCluster) {
    memset(dir, 0, sizeof(DirEntry) * MAX_DIR_ENTRIES); // Clear the directory entry structure
    bitmap_set_bit(bitmap, startCluster);  // Mark the start cluster as used
    fat_set_next_cluster(fat, startCluster, FAT_EOF);  // Mark in FAT as end of file to indicate no further clusters
}

// Add a directory entry to a specified directory
int directory_add_entry(FAT *fat, Bitmap *bitmap, DirEntry *parentDir, DirEntry *newEntry, int parentDirCluster) {
    // Find a free entry in the existing directory
    for (int i = 0; i < MAX_DIR_ENTRIES; i++) {
        if (parentDir[i].name[0] == '\0' || parentDir[i].name[0] == 0xE5) { // Check for empty or deleted entry
            memcpy(&parentDir[i], newEntry, sizeof(DirEntry));
            return i; // Success
        }
    }

    // Directory is full, need to expand
    int newCluster = fat_allocate_cluster(fat, bitmap);
    if (newCluster == -1) return -1; // No free clusters

    fat_set_next_cluster(fat, parentDirCluster, newCluster); // Link new cluster
    parentDir = (DirEntry *)malloc(sizeof(DirEntry) * MAX_DIR_ENTRIES); // Map new cluster to memory space
    memset(parentDir, 0, sizeof(DirEntry) * MAX_DIR_ENTRIES); // Clear new directory space
    memcpy(&parentDir[0], newEntry, sizeof(DirEntry)); // Add entry to new cluster
    return 0; // Success
}

// List the contents of a directory
void directory_list_contents(DirEntry *dir) {
    // This function should iterate through a directory's entries
    // This is a simplified example assuming dir points to an array of DirEntry
    for (int i = 0; dir[i].name[0] != '\0'; i++) {
        if (dir[i].name[0] == 0xE5) continue;  // Skip deleted entries
        printf("Name: %.8s.%.3s\n", dir[i].name, dir[i].extension);
        // Additional details can be printed similarly
    }
}
