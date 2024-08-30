/*
============================================================================
Name : 24.c
Author : Tushar Dubey
Description :Write a program to create an orphan process.
Date: 30th Aug, 2024.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    int pid = fork();

  if (pid == 0) {
       
        sleep(5);
        printf("Child process with pid %d is now orphan! ",getpid());
        sleep(10);  
    } else {
        
        printf("Parent process (PID: %d) is exiting...\n", getpid());
        exit(0); 
    }

    return 0;
}
//OUTPUT
//Parent process (PID: 72266) is exiting...
//Child process with pid 72267 is now orphan!