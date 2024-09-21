/*
============================================================================
Name : 1c.c
Author : Tushar Dubey
Description : Write a separate program (for each time domain) to set a interval timer in 10sec and
            10micro second
            c. ITIMER_PROF
Date: 19th Sept, 2024.
============================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>

// Signal handler for SIGPROF
void timer_handler(int signum) {
    printf("Timer expired! (every 10 seconds of CPU time)\n");
}

int main() {
    struct itimerval timer;

   
    signal(SIGPROF, timer_handler);

    // Set the initial timer to 10 seconds
    timer.it_value.tv_sec = 10;
    timer.it_value.tv_usec = 0;

 
    timer.it_interval.tv_sec = 10;
    timer.it_interval.tv_usec = 0;

   
    if (setitimer(ITIMER_PROF, &timer, NULL) == -1) {
        perror("Error setting timer");
        exit(1);
    }

  
    while (1) {
        // Simulating some CPU-bound work
        for (volatile int i = 0; i < 100000000; ++i);
    }

    return 0;
}
/*
OUTPUT
============================================================================
Timer expired! (every 10 seconds of CPU time)
Timer expired! (every 10 seconds of CPU time)
*/