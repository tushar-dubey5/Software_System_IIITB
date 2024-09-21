/*
============================================================================
Name : 30.c
Author : Tushar Dubey
Description : Write a program to create a shared memory.
                a. write some data to the shared memory
                b. attach with O_RDONLY and check whether you are able to overwrite.
                c. detach the shared memory
                d. remove the shared memory
Date: 21st Sept, 2024.
============================================================================
*/
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SHM_SIZE 1024
#define SHM_KEY 0x41003d21

void create_and_write_shm() {
    int shmid;
    char *shm;

    shmid = shmget(SHM_KEY, SHM_SIZE, 0666 | IPC_CREAT);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    shm = (char *)shmat(shmid, NULL, 0);
    if (shm == (char *)-1) {
        perror("shmat");
        exit(1);
    }

    snprintf(shm, SHM_SIZE, "Hello, this is shared memory!");

    if (shmdt(shm) == -1) {
        perror("shmdt");
        exit(1);
    }
}

void attach_and_check_shm() {
    int shmid;
    char *shm;

    shmid = shmget(SHM_KEY, SHM_SIZE, 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    shm = (char *)shmat(shmid, NULL, SHM_RDONLY);
    if (shm == (char *)-1) {
        perror("shmat");
        exit(1);
    }

    printf("Data from shared memory: %s\n", shm);

    if (shmdt(shm) == -1) {
        perror("shmdt");
        exit(1);
    }
}

void remove_shm() {
    int shmid;

    shmid = shmget(SHM_KEY, SHM_SIZE, 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl");
        exit(1);
    }
}

int main() {
    create_and_write_shm();
    sleep(1);
    attach_and_check_shm();
    remove_shm();
    return 0;
}

/*
OUTPUT
============================================================================
Data from shared memory: Hello, this is shared memory!
*/