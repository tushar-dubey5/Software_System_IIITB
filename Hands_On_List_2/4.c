/*
============================================================================
Name : 4.c
Author : Tushar Dubey
Description : Write a program to measure how much time is taken to execute 100 getppid ( )
                system call. Use time stamp counter. 
Date: 19th Sep, 2024.
============================================================================
*/
#include <stdio.h>
#include <unistd.h>
#include <time.h>

int main() {
    struct timespec start, end;

   
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < 100; i++) {
        getppid(); // Call getppid()
    }

  
    clock_gettime(CLOCK_MONOTONIC, &end);

   
    long seconds = end.tv_sec - start.tv_sec;
    long nanoseconds = end.tv_nsec - start.tv_nsec;
    long total_nanoseconds = seconds * 1e9 + nanoseconds;

    printf("Time taken for 100 getppid() calls: %ld nanoseconds\n", total_nanoseconds);
    
    return 0;
}
/*
OUTPUT
============================================================================
Time taken for 100 getppid() calls: 318608 nanoseconds
*/

