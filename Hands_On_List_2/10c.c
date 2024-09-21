/*
============================================================================
Name : 10c.c
Author : Tushar Dubey
Description : Write a separate program using sigaction system call to catch the following signals.
            c. SIGFPE
Date: 20th Sept, 2024.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>


void handle_sigfpe(int signal, siginfo_t *info, void *context) {
    printf("Caught signal %d (SIGFPE): Floating Point Exception occurred.\n", signal);
    exit(0); 
}

int main() {
    struct sigaction sa;

    
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = handle_sigfpe; 
    sigemptyset(&sa.sa_mask); 


    if (sigaction(SIGFPE, &sa, NULL) == -1) {
        perror("Error setting up signal handler");
        exit(1);
    }

    printf("Signal handler for SIGFPE set up.\n");

   
    int x = 5;
    int y = 0;
    int result = x / y;

   
    printf("Result: %d\n", result);

    return 0;
}
/*
OUTPUT
============================================================================
Signal handler for SIGFPE set up.
Caught signal 8 (SIGFPE): Floating Point Exception occurred.
*/