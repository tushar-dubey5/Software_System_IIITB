/*
============================================================================
Name : 13b.c
Author : Tushar Dubey
Description : Write two programs: first program is waiting to catch SIGSTOP signal, the second program
            will send the signal (using kill system call). Find out whether the first program is able to catch
            the signal or not.
Date: 20th Sept, 2024.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int main() {
    printf("Sending SIGSTOP to process with PID: %d\n", getpid());
    
   
    int target_pid;
    printf("Enter the PID of the first program: ");
    scanf("%d", &target_pid);

  
    kill(target_pid, SIGSTOP);
    
    printf("SIGSTOP sent to process %d.\n", target_pid);
    return 0;
}

/*
OUTPUT
============================================================================
Sending SIGSTOP to process with PID: 9429
Enter the PID of the first program: 9339
SIGSTOP sent to process 9339.
*/