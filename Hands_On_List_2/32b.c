/*
============================================================================
Name : 32b.c
Author : Tushar Dubey
Description :  Write a program to implement semaphore to protect any critical section.
                b. protect shared memory from concurrent write access
Date: 21st Sept, 2024.
============================================================================
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <fcntl.h>


int main() 
{
    sem_t *semaphore = sem_open("/shared_memory_semaphore", O_CREAT, 0644, 1);
    if (semaphore == SEM_FAILED) 
    {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    key_t key = ftok("/tmp", 65);
    int shmid = shmget(key, 1024, 0666 | IPC_CREAT);
    char *data = (char*) shmat(shmid, NULL, 0);

    sem_wait(semaphore); 
    strcpy(data, "Hello from shared memory!"); 
    printf("Data written to shared memory: %s\n", data);
    sem_post(semaphore); 

    shmdt(data); 
    sem_close(semaphore); 
}


/*
OUTPUT
============================================================================
    Data written to shared memory: Hello from shared memory!
*/