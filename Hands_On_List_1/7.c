/*
============================================================================
Name : 7.c
Author : Tushar Dubey
Description :  Write a program to copy file1 into file2 ($cp file1 file2).
Date: 30th Aug, 2024.
============================================================================
*/


#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
int main(){
    ssize_t bytesRead,bytesWrite;
    char buffer[1024];
    int fd = open("example1.txt",O_RDONLY);
    if(fd == -1){
        printf("Error reading file");
    }
   int fdWrite = open("example2.txt",O_WRONLY|O_CREAT);
   if(fdWrite == -1){
        printf("Error Opening Writting file");
        close(fd);
   }
   while ((bytesRead = read(fd, buffer, sizeof(buffer)-1)) > 0) {
        bytesWrite = write(fdWrite, buffer, bytesRead);
        if (bytesWrite != bytesRead) {
            printf("Error writing");
            close(fd);
            close(fdWrite);
            exit(0);
        }
    }
    close(fd);
    close(fdWrite);

}