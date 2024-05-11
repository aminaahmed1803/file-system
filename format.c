#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "disk.h"

//HOW TO RUN
//gcc -o format format.c disk.c
//./format DISK
// *formats a 1 MB disk*
//./format DISK -s 2
// *formats a 2 MB disk*

#define DEFAULT_DISK_SIZE_MB 1
#define FAT_ENTRY_SIZE 2
#define RESERVED_SECTORS 1

// format: filename, disk size (in mb)
int format(const char *filename, int disk_size_mb) {
    // handle filename
    if (filename == NULL || filename[0] == '\0') {
        printf("Error: Please provide a valid filename.\n");
        return -1;
    }
    // handle disk size
    if (disk_size_mb <= 0) {
        printf("Error: Invalid disk size. Defaulting to %d MB.\n", DEFAULT_DISK_SIZE_MB);
        disk_size_mb = DEFAULT_DISK_SIZE_MB;
    }
    // calculate disk size in bytes
    long disk_size_bytes = disk_size_mb * 1024 * 1024;
    printf("disk size in bytes: %ld\n", disk_size_bytes);
    // calculate FAT table size in bytes
    long fat_table_size_bytes = disk_size_bytes / BLOCK_SIZE * FAT_ENTRY_SIZE;
    printf("fat table size in bytes: %ld\n", fat_table_size_bytes);
    //malloc fat table to disk_size_bytes / BLOCK_SIZE * 2
    uint16_t *fat_table = (uint16_t *)malloc(fat_table_size_bytes);
    if (fat_table == NULL) {
        printf("Error: Unable to allocate memory for FAT table.\n");
        return -1;
    }
    int remaining_blocks = (disk_size_bytes - fat_table_size_bytes) / BLOCK_SIZE;
    printf("remaining blocks: %d\n", remaining_blocks);
    //memset a size 512 data block with zeroes
    char *data_block = (char *)malloc(BLOCK_SIZE);
    if (data_block == NULL) {
        printf("Error: Unable to allocate memory for data block.\n");
        return -1;
    }
    
    /*// initialize file system metadata
    memset(&file_system, 0, sizeof(FileSystem));
    // initialize FAT table (set all entries to 0)
    for (int i = 0; i < fat_table_size_bytes / 2; i++) {
        file_system.fat_table[i] = 0;
    }
    printf("fat table size: %d\n", sizeof(file_system.fat_table));*/
    // write file system data to disk file using fopen, fwrite, fclose
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        printf("Error: Unable to open file '%s' for writing.\n", filename);
        return -1;
    }
    fwrite(&file_system, *fat_table, 1, file);
    for(int i = 0; i < remaining_blocks; i++) {
        fwrite(data_block, BLOCK_SIZE, 1, file);
    }
    //printf("filesystem size: %d\n", sizeof(FileSystem));
    fclose(file);
    printf("Successfully formatted disk '%s' with size %d MB.\n", filename, disk_size_mb);
    return 0;
}

int main(int argc, char *argv[]) {
    // handle command line args
    if (argc < 2) {
        printf("Usage: %s <filename> [-s size_mb]\n", argv[0]);
        return 1;
    }
    const char *filename = argv[1];
    int disk_size_mb = DEFAULT_DISK_SIZE_MB;
    if (argc >= 4 && strcmp(argv[2], "-s") == 0) {
        disk_size_mb = atoi(argv[3]);
    }
    // call format
    return format(filename, disk_size_mb);
}

