/*
============================================================================
Name : 12.c
Author : Tushar Dubey
Description : Write a program to find out the opening mode of a file. Use fcntl.
Date: 30th Aug, 2024.
============================================================================
*/
//12. 

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

    if(argc !=2){
        printf("Not in a poper format");
        return 1;
    }
    
    int fd = open(argv[1], O_WRONLY);
    if (fd == -1) {
        printf("Error opening file");
        return 1;
    }

    int flags = fcntl(fd, F_GETFL); 
    int accessMode = flags & O_ACCMODE; 
    if (accessMode == O_RDONLY) {
    printf("File is opened in read-only mode.\n");
} else if (accessMode == O_WRONLY) {
    printf("File is opened in write-only mode.\n");
} else if (accessMode == O_RDWR) {
    printf("File is opened in read-write mode.\n");
}

    close(fd);
    return 0;
}

//the above code is trying to open "example1.txt" file and here is the outpur
//File is opened in write-only mode.
// if we pass more than one file name in argument we get "Not in a poper format" as output
