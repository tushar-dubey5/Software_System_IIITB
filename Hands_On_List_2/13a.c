/*
============================================================================
Name : 13a.c
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

void handle_signal(int sig) {
    printf("Caught signal %d\n", sig);
}

int main() {
   
    signal(SIGUSR1, handle_signal);

    printf("Process PID: %d. Waiting for SIGSTOP...\n", getpid());

   
    while (1) {
        pause(); 
    }

    return 0;
}

/*
OUTPUT
============================================================================
Process PID: 9339. Waiting for SIGSTOP...


[1]+  Stopped                 ./13a
*/