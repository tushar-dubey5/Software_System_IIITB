/*
============================================================================
Name : 8d.c
Author : Tushar Dubey
Description : Write a separate program using signal system call to catch the following signals.
                d. SIGALRM (use alarm system call)
Date: 20th Sept, 2024.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>


void handle_sigalrm(int signal) {
    printf("Caught signal %d (SIGALRM): Alarm signal received.\n", signal);
   
    exit(0); 
}

int main() {

    if (signal(SIGALRM, handle_sigalrm) == SIG_ERR) {
        perror("Error setting up signal handler");
        exit(1);
    }

    printf("Signal handler for SIGALRM set up. Setting an alarm for 5 seconds...\n");

    
    alarm(5);

    
    while (1) {
        
        pause(); 
    }

    return 0;
}

/*
OUTPUT
============================================================================
Signal handler for SIGALRM set up. Setting an alarm for 5 seconds...
Caught signal 14 (SIGALRM): Alarm signal received.
*/