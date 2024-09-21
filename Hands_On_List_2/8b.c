/*
============================================================================
Name : 8b.c
Author : Tushar Dubey
Description : Write a separate program using signal system call to catch the following signals.
            b. SIGINT
Date: 19th Sept, 2024.
============================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>


void handle_sigint(int signal) {
    printf("Caught signal %d (SIGINT): Interrupt signal received.\n", signal);

    exit(0); 
}

int main() {
   
    if (signal(SIGINT, handle_sigint) == SIG_ERR) {
        perror("Error setting up signal handler");
        exit(1);
    }

    printf("Signal handler for SIGINT set up. Press Ctrl+C to trigger SIGINT...\n");

   
    while (1) {
        sleep(1); 
    }

    return 0;
}


/*
OUTPUT
============================================================================
Signal handler for SIGINT set up. Press Ctrl+C to trigger SIGINT...
^CCaught signal 2 (SIGINT): Interrupt signal received.
*/