/*
============================================================================
Name : 16b.c
Author : Tushar Dubey
Description :Write a program to perform mandatory locking.
            b. Implement read lock
Date: 30th Aug, 2024.
============================================================================
*/
#include <unistd.h>   
#include <fcntl.h>     
#include <sys/types.h> 
#include <sys/stat.h> 
#include<stdio.h>


void main(int argc, char *argv[])
{

    char *filename;
    struct flock lock, lockStatus;
    int fileDescriptor;

    if (argc != 2)
        printf("Pass the file name to be locked as the argument!");
    else
    {
        filename = argv[1];

        lock.l_type = F_RDLCK;    
        lock.l_whence = SEEK_SET;
        lock.l_start = 0;         
        lock.l_len = 0;
        lock.l_pid = getpid();


        lockStatus = lock;

       
        fileDescriptor = open(filename, O_RDWR);


        fcntl(fileDescriptor, F_GETLK, &lockStatus);

        if (lockStatus.l_type == F_WRLCK)
            printf("File already locked for writing\n");
        else if (lockStatus.l_type == F_RDLCK)
            printf("File already locked for reading\n");
        else
        {
            
            fcntl(fileDescriptor, F_SETLK, &lock);
            printf("File is now locked for reading\n");
        }

        
}

//OUTPUT
//File is now locked for reading