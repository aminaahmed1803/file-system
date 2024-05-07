#ifndef DIR_H
#define DIR_H

#include "fat.h"
#include "bitmap.h"
#include <stdint.h>

#define MAX_DIR_ENTRIES 16  // Adjust as needed
#define DIRECTORY_ATTRIBUTE 0x10

// Directory entry structure definition
typedef struct {
    char     name[8];       // Filename (8 characters, space-padded)
    char     extension[3];  // File extension (3 characters)
    uint8_t  attributes;    // File attributes (bitfield)
    uint8_t  reserved;      // Reserved for future use
    uint16_t createTime;    // Creation time
    uint16_t createDate;    // Creation date
    uint16_t lastAccessDate;// Last access date
    uint16_t firstCluster;  // First cluster of file or directory
    uint16_t writeTime;     // Last write time
    uint16_t writeDate;     // Last write date
    uint32_t fileSize;      // Size of file in bytes (0 for directories)
} DirEntry;

// Function prototypes
void directory_init(FAT *fat, Bitmap *bitmap, DirEntry *dir, int startCluster);
int directory_add_entry(FAT *fat, Bitmap *bitmap, DirEntry *parentDir, DirEntry *newEntry, int parentDirCluster);
void directory_list_contents(FAT *fat, DirEntry *dir, int startCluster);

#endif /* DIR_H */
