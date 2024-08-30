/*
============================================================================
Name : 19.c
Author : Tushar Dubey
Description : Write a program to find out time taken to execute getpid system call. Use time stamp counter.
Date: 30th Aug, 2024.
============================================================================
*/

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <x86intrin.h> 

int main() {
    uint64_t start, end;
    int pid;

    start = __rdtsc();

 
    pid = getpid();

    end = __rdtsc();

    // Calculate the elapsed time
    uint64_t elapsed = end - start;

    // Output the results
    printf("Process ID: %d\n", pid);
    printf("Time taken (in cycles): %lu\n", elapsed);

    return 0;
}

//OUTPUT
/*Process ID: 57137
Time taken (in cycles): 6283*/