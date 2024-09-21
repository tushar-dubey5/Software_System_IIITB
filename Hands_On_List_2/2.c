/*
============================================================================
Name : 2.c
Author : Tushar Dubey
Description : Write a program to print the system resource limits. Use getrlimit system call.
Date: 19th Sept, 2024.
============================================================================
*/
#include <stdio.h>
#include <sys/resource.h>
#include <stdlib.h>

void print_limit(int resource, const char* resource_name) {
    struct rlimit limit;

    
    if (getrlimit(resource, &limit) == -1) {
        perror("Error getting limit");
        exit(1);
    }

   
    printf("%s: \n", resource_name);
    if (limit.rlim_cur == RLIM_INFINITY)
        printf("  Current limit: Unlimited\n");
    else
        printf("  Current limit: %ld\n", limit.rlim_cur);

    if (limit.rlim_max == RLIM_INFINITY)
        printf("  Maximum limit: Unlimited\n");
    else
        printf("  Maximum limit: %ld\n", limit.rlim_max);

    printf("\n");
}

int main() {
   
    print_limit(RLIMIT_CPU, "CPU time (seconds)");
    print_limit(RLIMIT_FSIZE, "Maximum file size (bytes)");
    print_limit(RLIMIT_DATA, "Maximum data segment size (bytes)");
    print_limit(RLIMIT_STACK, "Maximum stack size (bytes)");
    print_limit(RLIMIT_CORE, "Maximum core file size (bytes)");
    print_limit(RLIMIT_RSS, "Maximum resident set size (bytes)");
    print_limit(RLIMIT_NOFILE, "Maximum number of open files");
    print_limit(RLIMIT_NPROC, "Maximum number of processes");
    print_limit(RLIMIT_MEMLOCK, "Maximum locked memory (bytes)");

    return 0;
}

/*
OUTPUT
============================================================================
CPU time (seconds): 
  Current limit: Unlimited
  Maximum limit: Unlimited

Maximum file size (bytes): 
  Current limit: Unlimited
  Maximum limit: Unlimited

Maximum data segment size (bytes): 
  Current limit: Unlimited
  Maximum limit: Unlimited

Maximum stack size (bytes): 
  Current limit: 8388608
  Maximum limit: Unlimited

Maximum core file size (bytes): 
  Current limit: 0
  Maximum limit: Unlimited

Maximum resident set size (bytes): 
  Current limit: Unlimited
  Maximum limit: Unlimited

Maximum number of open files: 
  Current limit: 1048576
  Maximum limit: 1048576

Maximum number of processes: 
  Current limit: 31100
  Maximum limit: 31100

Maximum locked memory (bytes): 
  Current limit: 1028788224
  Maximum limit: 1028788224

*/
