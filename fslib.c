#include "fslib.h"
#include "dir.h"
#include "fat.h"
#include "disk.h"
#include <stdlib.h>
#include <string.h>

FileHandle* f_open(const char *filename, int mode) {
    DirEntry *dirEntry = find_dir_entry(filename);  // Assume this function searches for the file
    if (!dirEntry) {
        return NULL;  // File not found
    }

    FileHandle *fh = malloc(sizeof(FileHandle));
    if (!fh) {
        return NULL;  // Memory allocation failed
    }

    fh->entry = malloc(sizeof(DirEntry));
    if (!fh->entry) {
        free(fh);
        return NULL;
    }
    memcpy(fh->entry, dirEntry, sizeof(DirEntry));

    fh->currentCluster = dirEntry->firstCluster;
    fh->offsetInCluster = 0;
    fh->mode = mode;

    return fh;
}
