/*
============================================================================
Name : 6.c
Author : Tushar Dubey
Description : Write a program to take input from STDIN and display on STDOUT. Use only read/write system calls
Date: 30th Aug, 2024.
============================================================================
*/

#include <unistd.h>  
#include <fcntl.h>   
#include<stdio.h>

int main() {
    char buffer[1024];  
    ssize_t bytesRead;

    bytesRead = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);  
    printf("%zd = byteRead ",bytesRead);
    if (bytesRead == -1) {
        write(STDOUT_FILENO, "Error reading input\n", 20);
        return 1;
    }
    write(STDOUT_FILENO, buffer, bytesRead);

    return 0;
}
