/*
============================================================================
Name : 16.c
Author : Tushar Dubey
Description : Write a program to send and receive data from parent to child vice versa. Use two way
communication.
Date: 20th Sept, 2024.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main() {
    int pipe1[2]; 
    int pipe2[2]; 
    int pid;
    char buffer[100];

   
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    
    pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) { 
        const char* parent_message = "Hello from parent!";
        close(pipe1[0]);
        close(pipe2[1]); 
        
      
        write(pipe1[1], parent_message, strlen(parent_message) + 1);
        printf("Parent sent: %s\n", parent_message);
        
      
        read(pipe2[0], buffer, sizeof(buffer));
        printf("Parent received: %s\n", buffer);

        close(pipe1[1]);
        close(pipe2[0]);
    } else { 
        const char* child_message = "Hello from child!";
        close(pipe1[1]);
        close(pipe2[0]); 
        
       
        read(pipe1[0], buffer, sizeof(buffer));
        printf("Child received: %s\n", buffer);
        
      
        write(pipe2[1], child_message, strlen(child_message) + 1);
        printf("Child sent: %s\n", child_message);

        close(pipe1[0]); 
        close(pipe2[1]);
        }

    return 0;
}

/*
OUTPUT
============================================================================
Parent sent: Hello from parent!
Child received: Hello from parent!
Child sent: Hello from child!
Parent received: Hello from child!
*/
