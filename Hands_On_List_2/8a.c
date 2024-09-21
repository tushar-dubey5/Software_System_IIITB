/*
============================================================================
Name : 8a.c
Author : Tushar Dubey
Description : Write a separate program using signal system call to catch the following signals.
            a. SIGSEGV
Date: 19th Sept, 2024.
============================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void handle_sigsegv(int signal) {
    printf("Caught signal %d (SIGSEGV): Segmentation fault occurred.\n", signal);
    exit(1);
}

int main() {
    
    if (signal(SIGSEGV, handle_sigsegv) == SIG_ERR) {
        perror("Error setting up signal handler");
        exit(1);
    }

    printf("Signal handler for SIGSEGV set up. Triggering SIGSEGV...\n");

    int *ptr = NULL;
    *ptr = 42;  

    return 0;
}

/*
OUTPUT
============================================================================
Signal handler for SIGSEGV set up. Triggering SIGSEGV...
Caught signal 11 (SIGSEGV): Segmentation fault occurred.
*/
