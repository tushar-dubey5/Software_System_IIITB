/*
============================================================================
Name : 6.c
Author : Tushar Dubey
Description : Write a simple program to create three threads.
Date: 19th Sept, 2024.
============================================================================
*/
#include <stdio.h>
#include <pthread.h>

void* print_thread_id(void* id) {
    printf("Thread %ld: Hello!\n", (long)id);
    return NULL;
}

int main() {
    pthread_t threads[3];

    for (long i = 0; i < 3; i++) {
        pthread_create(&threads[i], NULL, print_thread_id, (void*)i);
    }

    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
/*
OUTPUT
============================================================================
Thread 0: Hello!
Thread 1: Hello!
Thread 2: Hello!
*/