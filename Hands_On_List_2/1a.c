/*
============================================================================
Name : 1a.c
Author : Tushar Dubey
Description : Write a separate program (for each time domain) to set a interval timer in 10sec and
            10micro second
            a. ITIMER_REAL
Date: 19th Sept, 2024.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>


void timer_handler(int signum) {
    printf("Timer expired! (every 10 seconds)\n");
}

int main() {
    struct itimerval timer;

  
    signal(SIGALRM, timer_handler);

    timer.it_value.tv_sec = 10;
    timer.it_value.tv_usec = 0;


    timer.it_interval.tv_sec = 10;
    timer.it_interval.tv_usec = 0;

   
    if (setitimer(ITIMER_REAL, &timer, NULL) == -1) {
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
Timer expired! (every 10 seconds)
Timer expired! (every 10 seconds)
*/