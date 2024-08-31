/*
============================================================================
Name : 9.c
Author : Tushar Dubey
Description :Write a program to print the following information about a given file.
            a. inode
            b. number of hard links
            c. uid
            d. gid
            e. size
            f. block size
            g. number of blocks
            h. time of last access
            i. time of last modification
            j. time of last change
Date: 31th Aug, 2024.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    struct stat fileStat;
    
    // Get file information
    if (stat(argv[1], &fileStat) == -1) {
        perror("stat");
        return 1;
    }

    // Print file information
    printf("File: %s\n", argv[1]);
    printf("Inode: %ld\n", (long) fileStat.st_ino);
    printf("Number of hard links: %ld\n", (long) fileStat.st_nlink);
    printf("User ID: %d\n", fileStat.st_uid);
    printf("Group ID: %d\n", fileStat.st_gid);
    printf("Size: %ld bytes\n", (long) fileStat.st_size);
    printf("Block size: %ld bytes\n", (long) fileStat.st_blksize);
    printf("Number of blocks: %ld\n", (long) fileStat.st_blocks);
    
    // Print the times of last access, modification, and change
    printf("Time of last access: %s", ctime(&fileStat.st_atime));
    printf("Time of last modification: %s", ctime(&fileStat.st_mtime));
    printf("Time of last change: %s", ctime(&fileStat.st_ctime));

    return 0;
}

//OUTPUT
/*File: example1.txt
Inode: 28442631
Number of hard links: 1
User ID: 1000
Group ID: 1000
Size: 51 bytes
Block size: 4096 bytes
Number of blocks: 8
Time of last access: Sat Aug 31 02:52:58 2024
Time of last modification: Tue Aug 27 17:39:11 2024
Time of last change: Sat Aug 31 02:52:33 2024*/