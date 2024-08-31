/*
============================================================================
Name : 11a.c
Author : Tushar Dubey
Description :Write a program to open a file, duplicate the file descriptor and append the file with both the
            descriptors and check whether the file is updated properly or not.
            a. use dup
Date: 31th Aug, 2024.
============================================================================
*/
#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
int main(){
    int fd, dup_fd, bytewritten;
    char *orignal = "This is an original message!";
    char *duplicate = "This is a duplicate message!";
    fd = open("examplefile.txt", O_WRONLY|O_CREAT);
    if(fd ==-1){
        printf("Error opening/creating the file!\n");
        return 1;
    }
    dup_fd = dup(fd);
    bytewritten = write(fd, orignal,strlen(orignal));
    if(bytewritten ==-1){
        printf("Error!\n");
        return 1;
    }
    bytewritten = write(dup_fd, duplicate, strlen(duplicate));
    if(bytewritten ==-1){
        printf("Error Duplicate writting!\n");
        return 1;
    }
    close(fd);
    close(dup_fd);
    return 0;

}

//OUTPUT
//This is an original message!
//This is a duplicate message!