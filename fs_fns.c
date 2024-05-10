#include "dir.h"
#include "fat.h"
#include "disk.h"
#include "fs_fns.h"
#include <stdlib.h>
#include <string.h>

FileHandle* f_open(const char *filename, const char *mode) {
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
    if(strcmp(mode, "r") == 0) {
        fh->mode = 0;
    } else if(strcmp(mode, "w") == 0) {
        fh->mode = 1;
    } else {
        free(fh->entry);
        free(fh);
        return NULL;
    }

    return fh;
}

// Read the specified number of bytes from a file handle at the current position
int f_read(FileHandle *file_handle, void *buffer, size_t size, size_t count){
    if (file_handle->mode != 0 && file_handle->mode != 1) {
        return -1;  // Invalid mode
    }

    if (file_handle->mode == 1) {
        return -1;  // Write mode
    }

    if (file_handle->offsetInCluster >= BLOCK_SIZE) {
        return -1;  // End of file
    }

    int bytesRead = 0;
    int bytesToRead = size * count;
    int bytesRemaining = file_handle->entry->fileSize - file_handle->offsetInCluster;
    if (bytesToRead > bytesRemaining) {
        bytesToRead = bytesRemaining;
    }

    int bytesLeftInCluster = BLOCK_SIZE - file_handle->offsetInCluster;
    if (bytesToRead > bytesLeftInCluster) {
        bytesToRead = bytesLeftInCluster;
    }

    int bytesReadThisCluster = 0;
    while (bytesRead < bytesToRead) {
        void *cluster = disk_read_cluster(file_handle->currentCluster);
        if (!cluster) {
            return -1;  // Disk read error
        }

        int bytesToCopy = bytesToRead - bytesRead;
        if (bytesToCopy > bytesLeftInCluster) {
            bytesToCopy = bytesLeftInCluster;
        }

        memcpy(buffer + bytesRead, cluster + file_handle->offsetInCluster, bytesToCopy);
        bytesRead += bytesToCopy;
        bytesReadThisCluster += bytesToCopy;
        file_handle->offsetInCluster += bytesToCopy;

        if (file_handle->offsetInCluster >= BLOCK_SIZE) {
            file_handle->offsetInCluster = 0;
            //file_handle->currentCluster = get_fat_entry(file_handle->currentCluster);
            if (file_handle->currentCluster == -1) {
                return -1;  // FAT error
            }
        }

        free(cluster);
    }

    return bytesRead;
}

// Write some bytes to a file handle at the current position
int f_write(FileHandle *file_handle, const void *buffer, size_t size, size_t count){
    if (file_handle->mode != 1) {
        return -1;  // Invalid mode
    }

    if (file_handle->offsetInCluster >= BLOCK_SIZE) {
        return -1;  // End of file
    }

    int bytesWritten = 0;
    int bytesToWrite = size * count;

    int bytesLeftInCluster = BLOCK_SIZE - file_handle->offsetInCluster;
    if (bytesToWrite > bytesLeftInCluster) {
        bytesToWrite = bytesLeftInCluster;
    }

    int bytesWrittenThisCluster = 0;
    while (bytesWritten < bytesToWrite) {
        void *cluster = disk_read_cluster(file_handle->currentCluster);
        if (!cluster) {
            return -1;  // Disk read error
        }

        int bytesToCopy = bytesToWrite - bytesWritten;
        if (bytesToCopy > bytesLeftInCluster) {
            bytesToCopy = bytesLeftInCluster;
        }

        memcpy(cluster + file_handle->offsetInCluster, buffer + bytesWritten, bytesToCopy);
        bytesWritten += bytesToCopy;
        bytesWrittenThisCluster += bytesToCopy;
        file_handle->offsetInCluster += bytesToCopy;

        if (file_handle->offsetInCluster >= BLOCK_SIZE) {
            file_handle->offsetInCluster = 0;
            int nextCluster = -1;//get_fat_entry(file_handle->currentCluster);
            if (nextCluster == -1) {
                //nextCluster = fat_allocate_cluster(file_handle->currentCluster, 1);
                if (nextCluster == -1) {
                    return -1;  // FAT error
                }
                //set_fat_entry(file_handle->currentCluster, nextCluster);
            }
            file_handle->currentCluster = nextCluster;
        }

        disk_write_cluster(file_handle->currentCluster, cluster);
        free(cluster);
    }

    if (file_handle->offsetInCluster > file_handle->entry->fileSize) {
        file_handle->entry->fileSize = file_handle->offsetInCluster;
    }

    return bytesWritten;
}

// Close a file handle
void f_close(FileHandle *file_handle){
    free(file_handle->entry);
    free(file_handle);
}

// Move to a specified position in a file
int f_seek(FileHandle *file_handle, long offset, int origin){
    if (origin == 0) {
        file_handle->offsetInCluster = offset;
    } else if (origin == 1) {
        file_handle->offsetInCluster += offset;
    } else if (origin == 2) {
        file_handle->offsetInCluster = file_handle->entry->fileSize + offset;
    } else {
        return -1;  // Invalid origin
    }

    if (file_handle->offsetInCluster < 0) {
        file_handle->offsetInCluster = 0;
    } else if (file_handle->offsetInCluster > file_handle->entry->fileSize) {
        file_handle->offsetInCluster = file_handle->entry->fileSize;
    }

    return 0;
}

// Move to the start of the file
void f_rewind(FileHandle *file_handle){
    file_handle->offsetInCluster = 0;
}

// Retrieve information about a file
int f_stat(const char *filename, struct stat *buf){
    DirEntry *dirEntry = find_dir_entry(filename);  // Assume this function searches for the file
    if (!dirEntry) {
        return -1;  // File not found
    }

    #include <sys/stat.h>

    buf->st_size = dirEntry->fileSize;

    return 0;
}

// Delete a file
int f_remove(const char *filename){
    DirEntry *dirEntry = find_dir_entry(filename);  // Assume this function searches for the file
    if (!dirEntry) {
        return -1;  // File not found
    }

    int cluster = dirEntry->firstCluster;
    //while (cluster != -1) {
        //int nextCluster = get_fat_entry(cluster);
        //intcluster = nextCluster;
    //}

    delete_dir_entry(dirEntry);

    return 0;
}

// Open a "directory file" for reading, and return a directory handle
DirEntry* f_opendir(const char *dirname){
    DirEntry *dirEntry = find_dir_entry(dirname);  // Assume this function searches for the directory
    if (!dirEntry) {
        return NULL;  // Directory not found
    }

    return dirEntry;
}

// Returns a pointer to a "directory entry" structure representing the next directory entry
struct dirent* f_readdir(DirEntry *dir_handle){
    static int entryIndex = 0;
    if (entryIndex >= MAX_DIR_ENTRIES) {
        return NULL;  // End of directory
    }

    return NULL;//&dir_handle->entries[entryIndex++];
}

// Close an open directory file
int f_closedir(DirEntry *dir_handle){
    return 0;
}

// Make a new directory at the specified location
int f_mkdir(const char *dirname){
    DirEntry *dirEntry = find_dir_entry(dirname);  // Assume this function searches for the directory
    if (dirEntry) {
        return -1;  // Directory already exists
    }

    create_dir_entry(dirname, DIRECTORY_ATTRIBUTE);

    return 0;
}

// Delete a specified directory, removing entire contents and contents of all subdirectories
int f_rmdir(const char *dirname){
    DirEntry *dirEntry = find_dir_entry(dirname);  // Assume this function searches for the directory
    if (!dirEntry) {
        return -1;  // Directory not found
    }

    int cluster = dirEntry->firstCluster;
    //while (cluster != -1) {
        //int nextCluster = get_fat_entry(cluster);
        //free_cluster(cluster);
        //cluster = nextCluster;
    //}

    delete_dir_entry(dirEntry);

    return 0;
}
