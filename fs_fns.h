//Temp .h file for FS fns so that I can write tests
#include <stdio.h>
#include "dir.h"

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

// Stat structure
struct stat {
    unsigned int st_size;  // Size of the file in bytes
    // Add any additional attributes needed for file information
};
// Open the specified file with the specified access
FileHandle* f_open(const char *filename, const char *mode);

// Read the specified number of bytes from a file handle at the current position
int f_read(FileHandle *file_handle, void *buffer, size_t size, size_t count);

// Write some bytes to a file handle at the current position
int f_write(FileHandle *file_handle, const void *buffer, size_t size, size_t count);

// Close a file handle
void f_close(FileHandle *file_handle);

// Move to a specified position in a file
int f_seek(FileHandle *file_handle, long offset, int origin);

// Move to the start of the file
void f_rewind(FileHandle *file_handle);

// Retrieve information about a file
int f_stat(const char *filename, struct stat *buf);

// Delete a file
int f_remove(const char *filename);

// Open a "directory file" for reading, and return a directory handle
DirEntry* f_opendir(const char *dirname);

// Returns a pointer to a "directory entry" structure representing the next directory entry
struct dirent* f_readdir(DirEntry *dir_handle);

// Close an open directory file
int f_closedir(DirEntry *dir_handle);

// Make a new directory at the specified location
int f_mkdir(const char *dirname);

// Delete a specified directory, removing entire contents and contents of all subdirectories
int f_rmdir(const char *dirname);
