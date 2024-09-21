/*
============================================================================
Name : 9.c
Author : Tushar Dubey
Description : Write a program to ignore a SIGINT signal then reset the default action of the SIGINT
signal - Use signal system call.
Date: 20th Sept, 2024.
============================================================================
*/

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handle_sigint(int sig) {
   
}

int main() {
    
    signal(SIGINT, handle_sigint);
    printf("SIGINT is now ignored. Press Ctrl+C to test.\n");

   
    sleep(5);

  
    signal(SIGINT, SIG_DFL);
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