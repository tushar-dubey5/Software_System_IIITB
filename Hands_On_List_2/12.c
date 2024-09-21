/*
============================================================================
Name : 12.c
Author : Tushar Dubey
Description : Write a program to create an orphan process. Use kill system call to send SIGKILL signal to
                the parent process from the child process.
Date: 20th Sept, 2024.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main() {
    int pid = fork();

    if (pid < 0) {
        
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
       
        printf("Parent process (PID: %d) is running. Child PID: %d\n", getpid(), pid);
        
       
        sleep(2);
        
       
        printf("Parent process (PID: %d) will be killed by the child...\n", getpid());
        pause(); 
    } else {
        
        printf("Child process (PID: %d) created. Sending SIGKILL to parent (PID: %d)\n", getpid(), getppid());
        
       
        kill(getppid(), SIGKILL);
        
        
        printf("Child process (PID: %d) is now an orphan.\n", getpid());
        
       
        sleep(5);
    }

    return 0;
}

/*
OUTPUT
============================================================================
Parent process (PID: 9133) is running. Child PID: 9134
Child process (PID: 9134) created. Sending SIGKILL to parent (PID: 9133)
Child process (PID: 9134) is now an orphan.
Killed
*/