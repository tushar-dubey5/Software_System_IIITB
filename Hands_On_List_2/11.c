/*
============================================================================
Name : 11c.c
Author : Tushar Dubey
Description : Write a program to ignore a SIGINT signal then reset the default action of the SIGINT signal -
use sigaction system call.
Date: 20th Sept, 2024.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void handle_sigint(int sig) {
   
}

int main() {
    struct sigaction sa;

    sa.sa_handler = handle_sigint;
    sa.sa_flags = 0;               
    sigemptyset(&sa.sa_mask);       

    // Ignore SIGINT
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
    printf("SIGINT is now ignored. Press Ctrl+C to test.\n");

    
    sleep(5);

   
    sa.sa_handler = SIG_DFL;
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
    printf("SIGINT has been reset to default action. Press Ctrl+C to terminate the program.\n");

  
    while (1) {
        pause(); 
    }

    return 0;
}


/*
OUTPUT
============================================================================
---WHEN "Ctrl+C" PRESSED---
SIGINT is now ignored. Press Ctrl+C to test.
^CSIGINT has been reset to default action. Press Ctrl+C to terminate the program.
^C\


---WHEN "Ctrl+C" IS NOT PRESSED---
SIGINT is now ignored. Press Ctrl+C to test.
SIGINT has been reset to default action. Press Ctrl+C to terminate the program.
^C
*/