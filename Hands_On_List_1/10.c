/*
============================================================================
Name : 10.c
Author : Tushar Dubey
Description :Write a program to open a file with read write mode, write 10 bytes, move the file pointer by 10
            bytes (use lseek) and write again 10 bytes.
            a. check the return value of lseek
            b. open the file with od and check the empty spaces in between the data.
Date: 31th Aug, 2024.
============================================================================
*/


#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    int fd;
    int offset;
    ssize_t bytes_written;
    const char *first_data = "1234567890"; 
    const char *second_data = "ABCDEFGHIJ";

    
    fd = open("testfile.txt", O_RDWR | O_CREAT, 0644);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    
    bytes_written = write(fd, first_data, 10);
    if (bytes_written != 10) {
        perror("write");
        close(fd);
        return 1;
    }

    
    offset = lseek(fd, 10, SEEK_CUR);
    if (offset == -1) {
        perror("lseek");
        close(fd);
        return 1;
    }

    printf("lseek() returned offset: %ld\n", (long) offset);

    
    bytes_written = write(fd, second_data, 10);
    if (bytes_written != 10) {
        perror("write");
        close(fd);
        return 1;
    }

  
    close(fd);

    return 0;
}

//OUTPUT
//lseek() returned offset: 20