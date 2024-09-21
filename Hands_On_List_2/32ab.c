/*
============================================================================
Name : 32ab.c
Author : Tushar Dubey
Description :  Write a program to implement semaphore to protect any critical section.
                a. rewrite the ticket number creation program using semaphore
Date: 21st Sept, 2024.
============================================================================
*/


#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>


int main() 
{
    sem_t *semaphore = sem_open("/ticket_semaphore", 0); 
    if (semaphore == SEM_FAILED) 
    {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    int ticket_number;

    sem_wait(semaphore); 
    FILE *file = fopen("ticket.txt", "r+");
    if (file == NULL) 
    {
        perror("fopen");
        sem_post(semaphore); 
        exit(EXIT_FAILURE);
    }
    fscanf(file, "%d", &ticket_number);
    ticket_number++; 
    fseek(file, 0, SEEK_SET);
    fprintf(file, "%d", ticket_number);
    fclose(file);
    printf("New ticket number: %d\n", ticket_number);
    sem_post(semaphore); 

    sem_close(semaphore); 
    return 0;
}


/*
OUTPUT
============================================================================
    New ticket number: 2
*/