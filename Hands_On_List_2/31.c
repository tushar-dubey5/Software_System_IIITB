/*
============================================================================
Name : 31.c
Author : Tushar Dubey
Description : Write a program to create a semaphore and initialize value to the semaphore.
                a. create a binary semaphore
                b. create a counting semaphore
Date: 21st Sept, 2024.
============================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>

#define SEM_NAME_BINARY "/binary_semaphore"
#define SEM_NAME_COUNTING "/counting_semaphore"

void create_and_init_binary_semaphore() {
    sem_t *sem = sem_open(SEM_NAME_BINARY, O_CREAT | O_EXCL, 0666, 1);
    if (sem == SEM_FAILED) {
        perror("sem_open for binary semaphore");
        exit(1);
    }

    printf("Binary semaphore created and initialized.\n");

    if (sem_close(sem) == -1) {
        perror("sem_close");
        exit(1);
    }
}

void create_and_init_counting_semaphore(int initial_value) {
    sem_t *sem = sem_open(SEM_NAME_COUNTING, O_CREAT | O_EXCL, 0666, initial_value);
    if (sem == SEM_FAILED) {
        perror("sem_open for counting semaphore");
        exit(1);
    }

    printf("Counting semaphore created and initialized with value %d.\n", initial_value);

    if (sem_close(sem) == -1) {
        perror("sem_close");
        exit(1);
    }
}

int main() {
    create_and_init_binary_semaphore();
    create_and_init_counting_semaphore(5);

    // Clean up semaphores
    if (sem_unlink(SEM_NAME_BINARY) == -1) {
        perror("sem_unlink for binary semaphore");
    }
    
    if (sem_unlink(SEM_NAME_COUNTING) == -1) {
        perror("sem_unlink for counting semaphore");
    }

    return 0;
}
/*
OUTPUT
============================================================================
Binary semaphore created and initialized.
Counting semaphore created and initialized with value 5.
*/