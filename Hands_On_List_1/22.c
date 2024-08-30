/*
============================================================================
Name : 22.c
Author : Tushar Dubey
Description : Write a program, open a file, call fork, and then write to the file by both the child as well as the
                parent processes. Check output of the file. 
Date: 30th Aug, 2024.
============================================================================
*/

#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>

int main(){
    int fd = open("example5.txt", O_WRONLY|O_CREAT);
    if(fd ==-1){
        printf("Error opening/creating the file!\n");
        return 1;
    }
    int child = fork();
    if(child == -1){
        printf("Error creating the child!\n");
        return 1;
    }
    if(child ==0){
        char *buffer = "This is written by child!\n";
        write(fd, buffer, strlen(buffer));
    }
    else {
    char *parentBuffer = "This is written by the parent!\n";
    write(fd, parentBuffer, strlen(parentBuffer));

    }
    close(fd);
}

//OUTPUT written in example5.txt file
// This is written by the parent!
// This is written by child!
