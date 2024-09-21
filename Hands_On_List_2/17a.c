/*
============================================================================
Name : 17a.c
Author : Tushar Dubey
Description : Write a program to execute ls -l | wc.
            a. use dup
Date: 20th Sept, 2024.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int pipefd[2]; // Array for the pipe file descriptors
    int pid1, pid2;

    // Create the pipe
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // First fork to execute 'ls -l'
    pid1 = fork();
    if (pid1 < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid1 == 0) { // Child process 1 (ls -l)
        dup2(pipefd[1], STDOUT_FILENO); // Redirect stdout to pipe write end
        close(pipefd[0]); // Close unused read end
        close(pipefd[1]); // Close the write end after duplicating

        execlp("ls", "ls", "-l", NULL); // Execute 'ls -l'
        perror("execlp"); // If execlp fails
        exit(EXIT_FAILURE);
    }

    // Second fork to execute 'wc'
    pid2 = fork();
    if (pid2 < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid2 == 0) { // Child process 2 (wc)
        dup2(pipefd[0], STDIN_FILENO); // Redirect stdin to pipe read end
        close(pipefd[1]); // Close unused write end
        close(pipefd[0]); // Close the read end after duplicating

        execlp("wc", "wc", NULL); // Execute 'wc'
        perror("execlp"); // If execlp fails
        exit(EXIT_FAILURE);
    }

    // Parent process
    close(pipefd[0]); // Close the read end of the pipe
    close(pipefd[1]); // Close the write end of the pipe

    // Wait for both child processes to finish
    wait(NULL);
    wait(NULL);

    return 0;
}

/*
OUTPUT
============================================================================
28     245    1356
*/