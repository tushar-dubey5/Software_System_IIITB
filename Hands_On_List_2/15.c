/*
============================================================================
Name : 15.c
Author : Tushar Dubey
Description : Write a simple program to send some data from parent to the child process.
Date: 20th Sept, 2024.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main() {
    int pipefd[2]; 
    int pid;
    char buffer[100]; 
    const char* message = "Hello from parent process!";

   
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

   
    pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) { 
        close(pipefd[0]); 
        write(pipefd[1], message, strlen(message) + 1); 
        close(pipefd[1]);
        printf("Parent sent: %s\n", message);
    } else { 
        close(pipefd[1]); 
        read(pipefd[0], buffer, sizeof(buffer));
        printf("Child received: %s\n", buffer);
        close(pipefd[0]); 
    }

    return 0;
}

/*
OUTPUT
============================================================================
Parent sent: Hello from parent process!
Child received: Hello from parent process!
*/