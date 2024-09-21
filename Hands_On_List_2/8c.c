/*
============================================================================
Name : 8c.c
Author : Tushar Dubey
Description : Write a separate program using signal system call to catch the following signals.
                c. SIGFPE
Date: 20th Sept, 2024.
============================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>


void handle_sigfpe(int signal) {
    printf("Caught signal %d (SIGFPE): Floating-point exception occurred.\n", signal);
    
    exit(1); 
}

int main() {
    
    if (signal(SIGFPE, handle_sigfpe) == SIG_ERR) {
        perror("Error setting up signal handler");
        exit(1);
    }

    printf("Signal handler for SIGFPE set up. Generating SIGFPE...\n");

    
    int x = 1;
    int y = 0;
    int z = x / y; 

    printf("Result of division: %d\n", z);

    return 0;
}

/*
OUTPUT
============================================================================
Signal handler for SIGFPE set up. Generating SIGFPE...
Caught signal 8 (SIGFPE): Floating-point exception occurred.
*/