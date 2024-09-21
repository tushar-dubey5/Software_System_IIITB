/*
============================================================================
Name : 8g.c
Author : Tushar Dubey
Description : Write a separate program using signal system call to catch the following signals.
            g. SIGPROF (use setitimer system call)
Date: 20th Sept, 2024.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>


void handle_sigprof(int signal) {
    printf("Caught signal %d (SIGPROF): Profiling alarm signal received.\n", signal);
    
    exit(0);
}

int main() {
   
    if (signal(SIGPROF, handle_sigprof) == SIG_ERR) {
        perror("Error setting up signal handler");
        exit(1);
    }

    printf("Signal handler for SIGPROF set up. Setting a profiling timer...\n");


    struct itimerval timer;
    timer.it_value.tv_sec = 5;  
    timer.it_value.tv_usec = 0; 
    timer.it_interval.tv_sec = 0; 
    timer.it_interval.tv_usec = 0; 


    if (setitimer(ITIMER_PROF, &timer, NULL) == -1) {
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
Signal handler for SIGPROF set up. Setting a profiling timer...
Caught signal 27 (SIGPROF): Profiling alarm signal received.
*/