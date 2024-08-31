/*
============================================================================
Name : 11c.c
Author : Tushar Dubey
Description :Write a program to open a file, duplicate the file descriptor and append the file with both the
            descriptors and check whether the file is updated properly or not.
            a. use fcntl
Date: 31th Aug, 2024.
============================================================================
*/

#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
int main(){
    int fd, dup_fd = 3;
    char *original = "This is written by the original fd\n";
    char *duplicate = "This is written by the dupliate fd\n";
    fd = open("examplefile4,txt",O_CREAT|O_WRONLY);
    if(fd ==-1){
        printf("Error opening the file!\n");
        return 1;
    }
    int dupli = fcntl(fd, F_DUPFD, 0);
    if(dupli ==-1){
        printf("Error dup2!\n");
        return 1;
    }
    int bytewrite;
    bytewrite = write(fd, original, strlen(original));
    if(bytewrite==-1){
        printf("Error writting the message!\n");
        return 1;
        close(fd);
        close(dup_fd);

    }
 bytewrite = write(dup_fd, duplicate, strlen(duplicate));
    if(bytewrite==-1){
        printf("Error writting the message!\n");
        return 1;
        close(fd);
        close(dup_fd);

    }
close(fd);
close(dup_fd);
return 0;


}

//OUTPUT
// This is written by the original fd
// This is written by the dupliate fd
