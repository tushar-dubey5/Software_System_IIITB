/*
============================================================================
Name : 4.c
Author : Tushar Dubey
Description : Write a program to open an existing file with read write mode. Try O_EXCL flag also.
Date: 30th Aug, 2024.
============================================================================
*/

#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
int main(){
    int fd = open("example.txt", O_RDWR| O_EXCL);
    if(fd == -1){
        printf("Error Opening the fie!");
    }
    else 
    printf("File Opened Successfully...");

    const char *message = "Hello World! This is my first message to you";
    size_t message_length = strlen(message);
  size_t total_written = 0;
   
   while (total_written < message_length) {
        ssize_t bytes_written = write(fd, message, message_length - total_written);
        
        if (bytes_written == -1) {
            perror("Error writing to file");
            close(fd);
            return 1;
        }
        
        total_written += bytes_written;
    }
    printf("Successfully Written\n");

    close(fd);
    return 0;
}

//Here is the output of the above program
//File Opened Successfully...Successfully Written
// Here is the output of the file "example.txt" in which we are performing reading and writting operation
//Hello World! This is my first message to you