/*
============================================================================
Name : 7.c
Author : Tushar Dubey
Description : Write a simple program to print the created thread ids.
Date: 19th Sept, 2024.
============================================================================
*/
#include <stdio.h>
#include <pthread.h>

void* print_thread_id(void* arg) {
    pthread_t thread_id = pthread_self();
    printf("Created thread ID: %lu\n", (unsigned long)thread_id);
    return NULL;
}

int main() {
    pthread_t threads[3];

    for (int i = 0; i < 3; i++) {
        pthread_create(&threads[i], NULL, print_thread_id, NULL);
    }

    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}

/*
OUTPUT
============================================================================
Created thread ID: 133226794321600
Created thread ID: 133226783835840
Created thread ID: 133226639132352
*/