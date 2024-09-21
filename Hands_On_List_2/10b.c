/*
============================================================================
Name : 10b.c
Author : Tushar Dubey
Description : Write a separate program using sigaction system call to catch the following signals.
                b. SIGINT
Date: 20th Sept, 2024.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>


void handle_sigint(int signal, siginfo_t *info, void *context) {
    printf("Caught signal %d (SIGINT): Interrupt signal received (Ctrl+C pressed).\n", signal);
    printf("Signal originating from process: %d\n", info->si_pid);
    exit(0); 
}

int main() {
    struct sigaction sa;

   
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = handle_sigint;
    sigemptyset(&sa.sa_mask); 

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("Error setting up signal handler");
        exit(1);
    }

    printf("Signal handler for SIGINT set up. Press Ctrl+C to trigger SIGINT...\n");

   
    while (1) {
        printf("Running... Press Ctrl+C to exit.\n");
        sleep(1); 
    }

    return 0;
}

/*
OUTPUT
============================================================================
Signal handler for SIGINT set up. Press Ctrl+C to trigger SIGINT...
Running... Press Ctrl+C to exit.
Running... Press Ctrl+C to exit.
Running... Press Ctrl+C to exit.
Running... Press Ctrl+C to exit.
Running... Press Ctrl+C to exit.
^CCaught signal 2 (SIGINT): Interrupt signal received (Ctrl+C pressed).
Signal originating from process: 0
*/