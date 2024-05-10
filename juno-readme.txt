Name: Juno Bartsch
HW7

Modifications from design document presented at the demo:
- Shift to FAT-16

Contributions:
format.c
- HOW TO RUN
- gcc -o format format.c disk.c
  ./format DISK *formats a 1 MB disk*
  ./format DISK -s N *formats an N MB disk*
- This program initializes a disk with the given size and FAT table. The disk can be given any name and the size is specified with the -s flag. All unused space is initialized as zeroes, to be written by VFS functions.

fs_fns.h
- List of function prototypes for the VFS tests as well as some data structures used in the tests.

fs_tests.c
- The tests do not currently run as we the VFS functions are not yet operational, but this is how I expect them to work:
- gcc -o fs_tests fs_tests.c disk.c
  ./fs_tests N
  N is the number test to run. The tests are as follows:
      - 0: Run all tests
      - 1: f_open
      - 2: f_read
      - 3: f_write
      - 4: f_close
      - 5: f_seek
      - 6: f_rewind
      - 7: f_stat
      - 8: f_remove
      - 9: opendir
      - 10: readdir
      - 11: closedir
      - 12: mkdir
      - 13: rmdir
      
- This program runs a series of tests on the VFS functions:
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

Shell tests
- I was unsuccessful at making a bash script to run the shell tests, but these are the tests I was hoping to run:
  - Run the shell with no arguments: should print "Usage: ./shell DISK"
  - Run the shell with a non-existent disk: should print "Error: disk not found"
  - Run the shell with a valid 1MB disk: should prompt login
  - Run the shell with a valid larger disk: should prompt login
  - (Assuming run with valid disk)
  - Run the shell with an invalid username: should print an error
  - Run the shell with an invalid password: should print an error
  - Run the shell with a valid regular user: should prompt for a command
    - If followed with superuser command: should print an error
  - Run the shell with a valid admin user: should prompt for a command
  - Run redirection symbols without a command e.g. >>: should print an error
  - Run redirection symbols with a command: should redirect the output to the file
  - Run incorrect redirection symbols: should print an error
  - Performance: try redirecting a large amount of data
  - ls a non-existent directory: should print an error
  - ls a valid directory: should list the contents of the directory
  - ls a valid file: should print the name of the file
  - ls -l a valid directory: should list the contents of the directory with permissions
  - ls -F a valid directory: should list the contents of the directory with file types
  - ls -badarg a valid directory: should print an error
  - ls -l or -F an invalid directory: should print an error
  - Performance: try listing a large number of files or large file sizes
  - chmod a non-existent file: should print an error
  - chmod a valid file using symbolic mode: should change the permissions of the file
  - chmod a valid file using absolute mode: should change the permissions of the file
  - mkdir an existing directory: should print an error
  - mkdir a new directory: should create the directory and adjust VFS data structures accordingly
  - rmdir a non-existent directory: should print an error
  - rmdir a non-empty directory: should print an error without the proper flag
  - rmdir a valid directory: should remove the directory
  - Performance: try making and removing directories at various depths
  - cd to a non-existent directory: should print an error
  - cd to a valid directory: should change the current directory
  - cd with no arguments: should change to the root directory
  - cd . or ..: should change to the current or parent directory
  - Performance: try changing directories at various depths
  - pwd with arguments: should print error invalid option
  - pwd with no arguments: should print the current directory
  - cat a non-existent file: should print no such file or directory
  - cat a valid file: should print the contents of the file
  - Performance: try reading large files
  - more (similar to cat)
  - rm a non-existent file: should print no such file or directory
  - rm a valid file: should remove the file
  - Edge cases: test with special characters, spaces, etc.
  - Test with large file sizes, nested directories, etc.
  - EXTRA CREDIT
  - Mount to a non-existent disk: should print an error
  - Mount to a valid disk: should mount to the disk at the root directory
  - Unmount a non-existent disk: should print an error
  - Unmount a valid disk: should unmount the disk
  - Performance: try mounting/ unmounting disks of various sizes
