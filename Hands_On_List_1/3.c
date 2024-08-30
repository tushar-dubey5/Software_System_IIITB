/*
============================================================================
Name : 3.c
Author : Tushar Dubey
Description : Write a program to create a file and print the file descriptor value. Use creat ( ) system call
Date: 30th Aug, 2024.
============================================================================
*/

#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>

int main(){
    int fileDescriptor = creat("NewFile.txt",0700);
    if(fileDescriptor == -1){
    perror("Error! File not created");
    }
    else 
    printf("File created successfuly and FileDescriptor value is : %d ",fileDescriptor);
    return 1;
}

// On running the above program we got the output
//File created successfuly and FileDescriptor value is : 3