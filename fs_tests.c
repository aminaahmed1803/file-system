#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "fs_fns.h"
#include "dir.h"

// Function prototypes for test functions
void run_fopen_tests();
void run_fread_tests();
void run_fwrite_tests();
void run_fclose_tests();
void run_fseek_tests();
void run_frewind_tests();
void run_fstat_tests();
void run_fremove_tests();
void run_opendir_tests();
void run_readdir_tests();
void run_closedir_tests();
void run_mkdir_tests();
void run_rmdir_tests();

/*
Test descriptions:
1a. Open a null file
1b. Open a file with null mode
1c. Open a file with read mode
1d. Open a file with write mode
1e. Open a file with append mode
1f. Open a large existing file with read mode
2a. Read from a null file handle
2b. Read from a file handle with null buffer
2c. Read from a file handle with 0 size
2d. Read from a file handle with 0 count
2e. Read from a file handle with 0 size and 0 count
2f. Read from a file handle with large buffer
3a. Write to a null file handle
3b. Write to a file handle with null buffer
3c. Write to a file handle with 0 size
3d. Write to a file handle with 0 count
3e. Write to a file handle with 0 size and 0 count
3f. Write to a file handle with large buffer
4a. Close a null file handle
4b. Close a file handle with null file pointer
4c. Close a file handle with null file pointer
4d. Close a file handle with valid file pointer
5a. Seek on a null file handle
5b. Seek on a file handle with valid file pointer
6a. Rewind a null file handle
6b. Rewind a file handle with valid file pointer
7a. Stat a null file
7b. Stat a null buffer
7c. Stat a valid file
8a. Remove a null file
8b. Remove a valid file
9a. Open a null directory
9b. Open a valid directory
10a. Read from a null directory handle
10b. Read from a valid directory handle
11a. Close a null directory handle
11b. Close a valid directory handle
12a. Make a null directory
12b. Make a valid directory
13a. Remove a null directory
13b. Remove a valid directory
*/

// Define a helper function to print test result
void print_test_result(const char *category, const char *test_name, int passed, const char *error_message) {
    printf("%s %s: %s%s\n", category, test_name, passed ? "PASSED" : "FAILED", passed ? "" : (error_message ? error_message : ""));
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <test_number>\n", argv[0]);
        printf("       %s 0 (runs all tests)\n", argv[0]);
        printf("       %s 1 (runs test 1)\n", argv[0]);
        printf("       %s n (runs test n)\n", argv[0]);
        return 1;
    }

    // Check if the argument is a valid integer
    /*if (!is_valid_test_number(argv[1])) {
        printf("Error: Test number must be a valid integer.\n");
        return 1;
    }*/
    //Default test number: 0
    int test_number = atoi(argv[1]);

    switch (test_number) {
        case 0:
            // Run all tests
            run_fopen_tests();
            run_fread_tests();
            run_fwrite_tests();
            run_fclose_tests();
            run_fseek_tests();
            run_frewind_tests();
            run_fstat_tests();
            run_fremove_tests();
            run_opendir_tests();
            run_readdir_tests();
            run_closedir_tests();
            run_mkdir_tests();
            run_rmdir_tests();
            break;
        case 1:
            run_fopen_tests();
            break;
        case 2:
            run_fread_tests();
            break;
        case 3:
            run_fwrite_tests();
            break;
        case 4:
            run_fclose_tests();
            break;
        case 5:
            run_fseek_tests();
            break;
        case 6:
            run_frewind_tests();
            break;
        case 7:
            run_fstat_tests();
            break;
        case 8:
            run_fremove_tests();
            break;
        case 9:
            run_opendir_tests();
            break;
        case 10:
            run_readdir_tests();
            break;
        case 11:
            run_closedir_tests();
            break;
        case 12:
            run_mkdir_tests();
            break;
        case 13:
            run_rmdir_tests();
            break;
        default:
            printf("Invalid test number. Please specify a valid test number.\n");
            break;
    }

    return 0;
}

void run_fopen_tests() {
    // Misuse
    // 1a. Open a null file
    FileHandle *f = f_open(NULL, "r");
    print_test_result("F_open", "1a", f == NULL, "fopen(NULL, \"r\") should return NULL");
    // 1b. Open a file with null mode
    f = f_open("file.txt", NULL);
    print_test_result("F_open", "1b", f == NULL, "fopen(\"file.txt\", NULL) should return NULL");
    
    // Correct use
    // 1c. Open a file with read mode
    f = f_open("file.txt", "r");
    print_test_result("F_open", "1c", f == NULL, "fopen(\"file.txt\", \"r\") should return NULL");
    // 1d. Open a file with write mode
    f = f_open("file.txt", "w");
    print_test_result("F_open", "1d", f == NULL, "fopen(\"file.txt\", \"w\") should return NULL");
    // 1e. Open a file with append mode
    f = f_open("file.txt", "a");
    print_test_result("F_open", "1e", f == NULL, "fopen(\"file.txt\", \"a\") should return NULL");

    // Performance
    // 1f. Open a large existing file with read mode
    f = f_open("large_file.txt", "r");
}
void run_fread_tests() {
    // Misuse
    // 2a. Read from a null file handle
    FileHandle *f = f_open("file.txt", "r");
    int read = f_read(NULL, NULL, 0, 0);
    print_test_result("F_read", "2a", read == 0, "fread(NULL, NULL, 0, 0) should return 0");
    // 2b. Read from a file handle with null buffer
    read = f_read(f, NULL, 0, 0);
    print_test_result("F_read", "2b", read == 0, "fread(f, NULL, 0, 0) should return 0");
    // 2c. Read from a file handle with 0 size
    read = f_read(f, (void *)malloc(1), 0, 0);
    print_test_result("F_read", "2c", read == 0, "fread(f, (void *)malloc(1), 0, 0) should return 0");

    // Correct use
    // 2d. Read from a file handle with 0 count
    read = f_read(f, (void *)malloc(1), 1, 0);
    print_test_result("F_read", "2d", read == 0, "fread(f, (void *)malloc(1), 1, 0) should return 0");
    // 2e. Read from a file handle with 0 size and 0 count
    read = f_read(f, (void *)malloc(1), 0, 0);
    print_test_result("F_read", "2e", read == 0, "fread(f, (void *)malloc(1), 0, 0) should return 0");

    // Performance
    // 2f. Read from a file handle with large buffer
    read = f_read(f, (void *)malloc(1024), 1, 1);
}
void run_fwrite_tests() {
    // Misuse
    // 3a. Write to a null file handle
    FileHandle *f = f_open("file.txt", "w");
    int written = f_write(NULL, NULL, 0, 0);
    print_test_result("F_write", "3a", written == 0, "fwrite(NULL, NULL, 0, 0) should return 0");
    // 3b. Write to a file handle with null buffer
    written = f_write(f, NULL, 0, 0);
    print_test_result("F_write", "3b", written == 0, "fwrite(f, NULL, 0, 0) should return 0");
    // 3c. Write to a file handle with 0 size
    written = f_write(f, (void *)malloc(1), 0, 0);
    print_test_result("F_write", "3c", written == 0, "fwrite(f, (void *)malloc(1), 0, 0) should return 0");

    // Correct use
    // 3d. Write to a file handle with 0 count
    written = f_write(f, (void *)malloc(1), 1, 0);
    print_test_result("F_write", "3d", written == 0, "fwrite(f, (void *)malloc(1), 1, 0) should return 0");
    // 3e. Write to a file handle with 0 size and 0 count
    written = f_write(f, (void *)malloc(1), 0, 0);
    print_test_result("F_write", "3e", written == 0, "fwrite(f, (void *)malloc(1), 0, 0) should return 0");

    // Performance
    // 3f. Write to a file handle with large buffer
    written = f_write(f, (void *)malloc(1024), 1, 1);
}
void run_fclose_tests() {
    // Misuse
    // 4a. Close a null file handle
    f_close(NULL);
    // 4b. Close a file handle with null file pointer
    FileHandle *f = (FileHandle *)malloc(sizeof(FileHandle));
    //f->entry = NULL;
    f_close(f);
    // 4c. Close a file handle with null file pointer
    f = (FileHandle *)malloc(sizeof(FileHandle));
    //f->entry = (FILE *)malloc(sizeof(FILE));
    //f->entry = NULL;
    f_close(f);

    // Correct use
    // 4d. Close a file handle with valid file pointer
    f = f_open("file.txt", "r");
    f_close(f);
}
void run_fseek_tests() {
    // Misuse
    // 5a. Seek on a null file handle
    FileHandle *f = f_open("file.txt", "r");
    int seek = f_seek(NULL, 0, 0);
    print_test_result("F_seek", "5a", seek == -1, "fseek(NULL, 0, 0) should return -1");

    // Correct use
    // 5b. Seek on a file handle with valid file pointer
    seek = f_seek(f, 0, 0);
    print_test_result("F_seek", "5b", seek == 0, "fseek(f, 0, 0) should return 0");
}
void run_frewind_tests() {
    // Misuse
    // 6a. Rewind a null file handle
    FileHandle *f = f_open("file.txt", "r");
    f_rewind(NULL);

    // Correct use
    // 6b. Rewind a file handle with valid file pointer
    f_rewind(f);
}
void run_fstat_tests() {
    // Misuse
    // 7a. Stat a null file
    struct stat buf;
    int stat = f_stat(NULL, &buf);
    print_test_result("F_stat", "7a", stat == -1, "fstat(NULL, &buf) should return -1");
    // 7b. Stat a null buffer
    stat = f_stat("file.txt", NULL);
    print_test_result("F_stat", "7b", stat == -1, "fstat(\"file.txt\", NULL) should return -1");

    // Correct use
    // 7c. Stat a valid file
    stat = f_stat("file.txt", &buf);
    print_test_result("F_stat", "7c", stat == 0, "fstat(\"file.txt\", &buf) should return 0");
}
void run_fremove_tests() {
    // Misuse
    // 8a. Remove a null file
    int remove = f_remove(NULL);
    print_test_result("F_remove", "8a", remove == -1, "fremove(NULL) should return -1");

    // Correct use
    // 8b. Remove a valid file
    remove = f_remove("file.txt");
    print_test_result("F_remove", "8b", remove == 0, "fremove(\"file.txt\") should return 0");
}
void run_opendir_tests() {
    // Misuse
    // 9a. Open a null directory
    DirEntry *dir = f_opendir(NULL);
    print_test_result("F_opendir", "9a", dir == NULL, "fopendir(NULL) should return NULL");

    // Correct use
    // 9b. Open a valid directory
    dir = f_opendir("dir");
    print_test_result("F_opendir", "9b", dir == NULL, "fopendir(\"dir\") should return NULL");
}
void run_readdir_tests() {
    // Misuse
    // 10a. Read from a null directory handle
    DirEntry *dir = f_opendir("dir");
    struct dirent *entry = f_readdir(NULL);
    print_test_result("F_readdir", "10a", entry == NULL, "freaddir(NULL) should return NULL");

    // Correct use
    // 10b. Read from a valid directory handle
    entry = f_readdir(dir);
    print_test_result("F_readdir", "10b", entry == NULL, "freaddir(dir) should return NULL");
}
void run_closedir_tests() {
    // Misuse
    // 11a. Close a null directory handle
    f_closedir(NULL);

    // Correct use
    // 11b. Close a valid directory handle
    DirEntry *dir = f_opendir("dir");
    f_closedir(dir);
}
void run_mkdir_tests() {
    // Misuse
    // 12a. Make a null directory
    int mkdir = f_mkdir(NULL);
    print_test_result("F_mkdir", "12a", mkdir == -1, "fmkdir(NULL) should return -1");

    // Correct use
    // 12b. Make a valid directory
    mkdir = f_mkdir("dir");
    print_test_result("F_mkdir", "12b", mkdir == 0, "fmkdir(\"dir\") should return 0");
}
void run_rmdir_tests() {
    // Misuse
    // 13a. Remove a null directory
    int rmdir = f_rmdir(NULL);
    print_test_result("F_rmdir", "13a", rmdir == -1, "frmdir(NULL) should return -1");

    // Correct use
    // 13b. Remove a valid directory
    rmdir = f_rmdir("dir");
    print_test_result("F_rmdir", "13b", rmdir == 0, "frmdir(\"dir\") should return 0");
}

