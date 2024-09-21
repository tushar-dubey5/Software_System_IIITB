
/*
============================================================================
Name : 3.c
Author : Tushar Dubey
Description : Write a program to set (any one) system resource limit. Use setrlimit system call.
Date: 19th Sept, 2024.
============================================================================
*/
#include <stdio.h>
#include <sys/resource.h>
#include <stdlib.h>

int main() {
    struct rlimit limit;

   
    if (getrlimit(RLIMIT_FSIZE, &limit) == -1) {
        perror("Error getting current file size limit");
        exit(1);
    }

    printf("Current file size limit: soft = %ld, hard = %ld\n", limit.rlim_cur, limit.rlim_max);

  
    limit.rlim_cur = 1024 * 1024 * 10; 
    limit.rlim_max = 1024 * 1024 * 50; 

    if (setrlimit(RLIMIT_FSIZE, &limit) == -1) {
        perror("Error setting file size limit");
        exit(1);
    }

   
    if (getrlimit(RLIMIT_FSIZE, &limit) == -1) {
        perror("Error getting updated file size limit");
        exit(1);
    }

    printf("Updated file size limit: soft = %ld, hard = %ld\n", limit.rlim_cur, limit.rlim_max);

    return 0;
}

/*
OUTPUT
============================================================================
Current file size limit: soft = -1, hard = -1
Updated file size limit: soft = 10485760, hard = 52428800
*/