/*
============================================================================
Name : 27a.c
Author : Tushar Dubey
Description : Write a program to receive messages from the message queue.
            a. with 0 as a flag
Date: 21st Sept, 2024.
============================================================================
*/
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TEXT 100

struct message {
    long msg_type;
    char msg_text[MAX_TEXT];
};

int main() {
    int msqid;
    key_t key = 0x41003d21 ; // Unique key for the message queue
    struct message msg;

    // Get message queue ID
    msqid = msgget(key, 0666);
    if (msqid == -1) {
        perror("msgget failed");
        exit(1);
    }

    // Receive message
    if (msgrcv(msqid, (void *)&msg, MAX_TEXT, 0, 0) == -1) {
        perror("msgrcv failed");
        exit(1);
    }

    printf("Message received: %s\n", msg.msg_text);

    return 0;
}
