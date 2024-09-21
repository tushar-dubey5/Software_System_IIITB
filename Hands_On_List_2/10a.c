/*
============================================================================
Name : 10a.c
Author : Tushar Dubey
Description : Write a separate program using sigaction system call to catch the following signals.
                a. SIGSEGV
Date: 20th Sept, 2024.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>


void handle_sigsegv(int signal, siginfo_t *info, void *context) {
    printf("Caught signal %d (SIGSEGV): Segmentation fault received.\n", signal);
    printf("Address that caused the fault: %p\n", info->si_addr);
  
    exit(1); 
}

int main() {
    struct sigaction sa;
    
  
    sa.sa_flags = SA_SIGINFO; 
    sa.sa_sigaction = handle_sigsegv; 
    sigemptyset(&sa.sa_mask);

   
    if (sigaction(SIGSEGV, &sa, NULL) == -1) {
        perror("Error setting up signal handler");
        exit(1);
    }

    printf("Signal handler for SIGSEGV set up. Triggering segmentation fault...\n");

  
    int *ptr = NULL; 
    *ptr = 42; 

    return 0;
}

/*
OUTPUT
============================================================================
Signal handler for SIGSEGV set up. Triggering segmentation fault...
Caught signal 11 (SIGSEGV): Segmentation fault received.
Address that caused the fault: (nil)
*/