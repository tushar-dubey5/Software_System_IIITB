/*
============================================================================
Name : 14.c
Author : Tushar Dubey
Description : Write a simple program to create a pipe, write to the pipe, read from pipe and display on
            the monitor.
Date: 20th Sept, 2024.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main() {
    int pipefd[2]; 
    char buffer[100]; 
    const char* message = "Hello, this is a message from the pipe!";

   
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

  
    write(pipefd[1], message, strlen(message) + 1); 
    close(pipefd[1]); 


    read(pipefd[0], buffer, sizeof(buffer));
    printf("Received from pipe: %s\n", buffer);
    
    close(pipefd[0]); 

    return 0;
}
/*
OUTPUT
============================================================================
Received from pipe: Hello, this is a message from the pipe!
*/