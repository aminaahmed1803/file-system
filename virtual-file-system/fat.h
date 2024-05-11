#ifndef FAT16_H
#define FAT16_H

#include <stdint.h>

/** The given offset is relative to the beginning of the file. */
#define FAT16_SEEK_SET 0
/** The given offset is relative to the current read/write position. */
#define FAT16_SEEK_CUR 1
/** The given offset is relative to the end of the file. */
#define FAT16_SEEK_END 2

#define READ_ONLY 1
#define WRITE_ONLY 2
#define READ_WRITE 3
#define APPEND 4

typedef struct fat16_dir_entry {   
    char* name;      
    uint16_t idx;
    uint32_t file_size;
    uint8_t uid;
    uint8_t attributes; 
   
}dir_entry;

struct fat16_header {
    uint32_t size;
    uint32_t fat_offset;
    uint32_t fat_size;
    uint16_t sector_size;
    uint32_t root_dir_offset;
};

typedef struct file_handle{ 
    char *name;
    u_int8_t is_dir; 
    size_t size; 
    u_int16_t nxt_idx; 
    u_int16_t prev_idx; 
}FILE;

typedef struct dir_handle {
    char *name; 
    u_int8_t is_dir;
    size_t size; 
    u_int16_t nxt_idx;
    u_int16_t prev_idx; 
}DIR;

//file header 
typedef struct file_header { 
    char *name;
    u_int8_t is_directory; 
    u_int16_t idx; 
    u_int32_t size; 
    char *data;
}file_header;

//directory header
typedef struct dir_header { 
    char *name;
    u_int8_t is_directory; 
    u_int16_t idx; 
    u_int32_t size;
    dir_entry data_in_first_block[15]; 
}dir_header;

FILE *f_open(const char *, const int);
size_t f_read(void *, size_t, size_t, FILE *);
size_t f_write(const void *, size_t, size_t, FILE *);
int f_close(FILE *);
int f_seek(FILE *, long, int);
void f_rewind(FILE *);
int f_remove(const char *);
DIR *f_opendir(const char *);
dir_entry *f_readdir(DIR *);
int f_closedir(DIR *);
int f_mkdir(const char *, char *);
int f_rmdir(const char *);

#endif
