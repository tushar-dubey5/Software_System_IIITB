/*
============================================================================
Name : 5.c
Author : Tushar Dubey
Description : Write a program to print the system limitation of
                a. maximum length of the arguments to the exec family of functions.
                b. maximum number of simultaneous process per user id.
                c. number of clock ticks (jiffy) per second.
                d. maximum number of open files
                e. size of a page
                f. total number of pages in the physical memory
                g. number of currently available pages in the physical memory.
Date: 19th Sept, 2024.
============================================================================
*/
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <sys/resource.h>
#include <sys/sysinfo.h>

int main() {
   
    long max_arg_length = sysconf(_SC_ARG_MAX);
    printf("Maximum length of arguments for exec functions: %ld bytes\n", max_arg_length);

    
    struct rlimit process_limit;
    getrlimit(RLIMIT_NPROC, &process_limit);
    printf("Max simultaneous processes per user ID: %lu\n", process_limit.rlim_cur);

    
    long clock_ticks = sysconf(_SC_CLK_TCK);
    printf("Clock ticks (jiffies) per second: %ld\n", clock_ticks);

  
    struct rlimit file_limit;
    getrlimit(RLIMIT_NOFILE, &file_limit);
    printf("Maximum number of open files: %lu\n", file_limit.rlim_cur);

   
    long page_size = sysconf(_SC_PAGESIZE);
    printf("Size of a memory page: %ld bytes\n", page_size);

    struct sysinfo sys_info;
    sysinfo(&sys_info);
    long total_pages = sys_info.totalram / page_size;
    printf("Total number of pages in physical memory: %ld\n", total_pages);

   
    long free_pages = sys_info.freeram / page_size;
    printf("Currently available pages in physical memory: %ld\n", free_pages);

    return 0;
}
/*
OUTPUT
============================================================================
Maximum length of arguments for exec functions: 2097152 bytes
Max simultaneous processes per user ID: 31100
Clock ticks (jiffies) per second: 100
Maximum number of open files: 1048576
Size of a memory page: 4096 bytes
Total number of pages in physical memory: 2009355
Currently available pages in physical memory: 66881
*/