#ifndef FSLIB_H
#define FSLIB_H

#include "dir.h"  // Assuming DirEntry and related definitions are in dir.h
#include <stddef.h>  // For size_t definition

// File mode flags
#define FS_READ   0x1
#define FS_WRITE  0x2
#define FS_APPEND 0x4

typedef struct {
    DirEntry *entry;        // Pointer to the directory entry of the file
    int currentCluster;     // Current cluster being accessed
    int offsetInCluster;    // Byte offset within the current cluster
    int mode;               // Mode the file was opened in
} FileHandle;

// Function prototypes
FileHandle* f_open(const char *filename, int mode);
int f_close(FileHandle *fh);
size_t f_read(void *buffer, size_t size, size_t count, FileHandle *fh);
size_t f_write(const void *buffer, size_t size, size_t count, FileHandle *fh);
int f_seek(FileHandle *fh, long offset, int origin);
int f_stat(const char *filename, DirEntry *stat);

#endif /* FSLIB_H */
