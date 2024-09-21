/*
============================================================================
Name : 18.c
Author : Tushar Dubey
Description : Write a program to find out total number of directories on the pwd.
execute ls -l | grep ^d | wc ? Use only dup2.
Date: 20th Sept, 2024.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int pipe1[2]; // Pipe for ls -l to grep
    int pipe2[2]; // Pipe for grep to wc
    pid_t pid1, pid2, pid3;

    // Create the first pipe
    if (pipe(pipe1) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Create the second pipe
    if (pipe(pipe2) == -1) {
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
        dup2(pipe1[1], STDOUT_FILENO); // Redirect stdout to pipe1 write end
        close(pipe1[0]); // Close unused read end
        close(pipe1[1]); // Close the write end after duplicating

        execlp("ls", "ls", "-l", NULL); // Execute 'ls -l'
        perror("execlp"); // If execlp fails
        exit(EXIT_FAILURE);
    }

    // Second fork to execute 'grep ^d'
    pid2 = fork();
    if (pid2 < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid2 == 0) { // Child process 2 (grep ^d)
        dup2(pipe1[0], STDIN_FILENO); // Redirect stdin to pipe1 read end
        dup2(pipe2[1], STDOUT_FILENO); // Redirect stdout to pipe2 write end
        close(pipe1[1]); // Close unused write end of pipe1
        close(pipe1[0]); // Close the read end after duplicating
        close(pipe2[0]); // Close unused read end of pipe2

        execlp("grep", "grep", "^d", NULL); // Execute 'grep ^d'
        perror("execlp"); // If execlp fails
        exit(EXIT_FAILURE);
    }

    // Third fork to execute 'wc'
    pid3 = fork();
    if (pid3 < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid3 == 0) { // Child process 3 (wc)
        dup2(pipe2[0], STDIN_FILENO); // Redirect stdin to pipe2 read end
        close(pipe2[1]); // Close unused write end of pipe2
        close(pipe2[0]); // Close the read end after duplicating

        execlp("wc", "wc", "-l", NULL); // Execute 'wc -l' to count lines
        perror("execlp"); // If execlp fails
        exit(EXIT_FAILURE);
    }

    // Parent process
    close(pipe1[0]); // Close the read end of pipe1
    close(pipe1[1]); // Close the write end of pipe1
    close(pipe2[0]); // Close the read end of pipe2
    close(pipe2[1]); // Close the write end of pipe2

    // Wait for all child processes to finish
    wait(NULL);
    wait(NULL);
    wait(NULL);

    return 0;
}
/*
OUTPUT
============================================================================
      1      10      69
*/