/*
============================================================================
Name : 23.c
Author : Tushar Dubey
Description :  Write a program to print the maximum number of files can be opened within a process and size of a pipe (circular buffer).  
Date: 21st Sept, 2024.
============================================================================
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>


int main() 
{
    struct rlimit rl;
    
    if (getrlimit(RLIMIT_NOFILE, &rl) == -1) {
        perror("getrlimit");
        exit(EXIT_FAILURE);
    }
    printf("Maximum number of files a process can open: %ld\n", rl.rlim_cur);


    int pipe_fd[2];
    
    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    int flags = fcntl(pipe_fd[1], F_GETFL, 0);
    fcntl(pipe_fd[1], F_SETFL, flags | O_NONBLOCK);

    int pipe_size = 0;
    char buffer[1024];
    while(1) 
    {
        int written = write(pipe_fd[1], buffer, sizeof(buffer));
        if (written == -1) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) 
            {
                break; 
            } 
            else 
            {
                perror("write");
                exit(EXIT_FAILURE);
            }
        }
        pipe_size += written;
    }
    
    printf("Size of a pipe (circular buffer): %d bytes\n", pipe_size);

    close(pipe_fd[0]);
    close(pipe_fd[1]);

    return 0;
}



/*
OUTPUT
============================================================================
Maximum number of open files: 1048576
Pipe buffer size: 65536 bytes
*/