/*
============================================================================
Name : 14.c
Author : Tushar Dubey
Description : Write a program to find the type of a file.
             a. Input should be taken from command line.
            b. program should be able to identify any type of a file
Date: 30th Aug, 2024.
============================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    struct stat file_info;

    if (stat(argv[1], &file_info) == -1) {
        perror("Error getting file information");
        return 1;
    }
   __mode_t  mode = file_info.st_mode;
   if (S_ISREG(mode)) {
        printf("Regular file\n");
    } else if (S_ISDIR(mode)) {
        printf("Directory\n");
    } else if (S_ISLNK(mode)) {
        printf("Symbolic link\n");
    } else if (S_ISCHR(mode)) {
        printf("Character device\n");
    } else if (S_ISBLK(mode)) {
        printf("Block device\n");
    } else if (S_ISFIFO(mode)) {
        printf("FIFO (named pipe)\n");
    } else if (S_ISSOCK(mode)) {
        printf("Socket\n");
    } else {
        printf("Unknown file type\n");
    }
   

    return 0;
}


// Output after running "./14 Question2.cpp" where ./14 is compiled file and Question2.cpp is the file we are tying to open we got the output
//Regular file