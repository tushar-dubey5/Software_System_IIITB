/*
============================================================================
Name : 8f.c
Author : Tushar Dubey
Description : Write a separate program using signal system call to catch the following signals.
            f. SIGVTALRM (use setitimer system call)
Date: 20th Sept, 2024.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>


void handle_sigvtalrm(int signal) {
    printf("Caught signal %d (SIGVTALRM): Virtual alarm signal received.\n", signal);
   
    exit(0); 
}

int main() {
    
    if (signal(SIGVTALRM, handle_sigvtalrm) == SIG_ERR) {
        perror("Error setting up signal handler");
        exit(1);
    }

    printf("Signal handler for SIGVTALRM set up. Setting a virtual timer...\n");

    
    struct itimerval timer;
    timer.it_value.tv_sec = 5;  
    timer.it_value.tv_usec = 0; 
    timer.it_interval.tv_sec = 0; 
    timer.it_interval.tv_usec = 0; 


    if (setitimer(ITIMER_VIRTUAL, &timer, NULL) == -1) {
        perror("Error setting timer");
        exit(1);
    }

   
    while (1) {
        
        pause(); 
    }

    return 0;
}

/*
OUTPUT
============================================================================
Signal handler for SIGVTALRM set up. Setting a virtual timer...
Caught signal 27 (SIGVTALRM): Virtual alarm signal received.
*/