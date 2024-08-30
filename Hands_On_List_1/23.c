/*
============================================================================
Name : 23.c
Author : Tushar Dubey
Description : Write a program, open a file, call fork, and then write to the file by both the child as well as the
                parent processes. Check output of the file.
Date: 30th Aug, 2024.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    int pid = fork();

   
    if (pid == 0) {
      
        printf("Child process (PID: %d) is exiting...\n", getpid());
        exit(0); 
    } else {
    
        printf("Parent process (PID: %d) is sleeping...\n", getpid());
        sleep(10);
        printf("Parent process (PID: %d) is exiting...\n", getpid());
    }

    return 0;
}

//OUTPUT
// Parent process (PID: 71665) is sleeping...
// Child process (PID: 71666) is exiting...
// Parent process (PID: 71665) is exiting...