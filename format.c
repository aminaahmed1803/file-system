#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "disk.h"

#define DEFAULT_DISK_SIZE_MB 1

typedef struct {
    // Do we need any file system metadata?
} FileSystemMetadata;

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
    // init the disk
    disk_init(filename);
    // calculate disk size in bytes
    long disk_size_bytes = disk_size_mb * 1024 * 1024;
    // alloc disk space
    char *buffer = (char *)malloc(disk_size_bytes);
    if (buffer == NULL) {
        printf("Error: Memory allocation failed.\n");
        disk_cleanup();
        return -1;
    }
    // clear disk space
    memset(buffer, 0, disk_size_bytes);
    // Write the formatted disk space to the disk
    for (int cluster = 0; cluster < (disk_size_bytes / (BLOCK_SIZE * BLOCKS_PER_CLUSTER)); cluster++) {
        disk_write_cluster(cluster, buffer);
    }
    // free the buffer
    free(buffer);
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

