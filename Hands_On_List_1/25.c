/*
============================================================================
Name : 25.c
Author : Tushar Dubey
Description : Write a program to create three child processes. The parent should wait for a particular child (use
             waitpid system call).
Date: 30th Aug, 2024.
============================================================================
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int pid1, pid2, pid3;
    int target_pid;
    int status;

   
    pid1 = fork();
    if (pid1 == 0) {
        printf("First child (PID: %d) is running...\n", getpid());
        sleep(2); 
        printf("First child (PID: %d) is exiting...\n", getpid());
        exit(0);
    }

   
    pid2 = fork();
    if (pid2 == 0) {
        
        printf("Second child (PID: %d) is running...\n", getpid());
        sleep(4); 
        printf("Second child (PID: %d) is exiting...\n", getpid());
        exit(0);
    }

   
    pid3 = fork();
    if (pid3 == 0) {
       
        printf("Third child (PID: %d) is running...\n", getpid());
        sleep(6);  
        printf("Third child (PID: %d) is exiting...\n", getpid());
        exit(0);
    }


    target_pid = pid2;
    printf("Parent (PID: %d) is waiting for the second child (PID: %d) to finish...\n", getpid(), target_pid);
    waitpid(target_pid, &status, 0);

    if (WIFEXITED(status)) {
        printf("Parent (PID: %d) detected that the second child (PID: %d)\n", getpid(), target_pid);
    } else {
        printf("Parent (PID: %d) detected that the second child (PID: %d) did not exit normally.\n", getpid(), target_pid);
    }


    printf("Parent (PID: %d) is exiting.\n", getpid());

    return 0;
}
//OUTPUT
/*First child (PID: 72405) is running...
Parent (PID: 72404) is waiting for the second child (PID: 72406) to finish...
Second child (PID: 72406) is running...
Third child (PID: 72407) is running...
First child (PID: 72405) is exiting...
Second child (PID: 72406) is exiting...
Parent (PID: 72404) detected that the second child (PID: 72406) exited with status 0.
Parent (PID: 72404) is exiting.*/