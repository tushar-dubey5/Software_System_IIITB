/*
============================================================================
Name : 27b.c
Author : Tushar Dubey
Description :  Write a program to receive messages from the message queue.
                b. with IPC_NOWAIT as a flag  
Date: 21st Sept, 2024.
============================================================================
*/


#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

struct msg_buffer 
{
    long msg_type;
    char msg_text[100];
};

int main() 
{
    key_t key = ftok("/tmp", 65);

    if (key == -1) 
    {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    int msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) 
    {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    struct msg_buffer message;

    if (msgrcv(msgid, &message, sizeof(message.msg_text), 1, IPC_NOWAIT) == -1) 
    {
        perror("msgrcv");
        exit(EXIT_FAILURE);
    }

    printf("Message received: %s\n", message.msg_text);

    return 0;
}


/*
============================================================================
Message received: Hello programmer
*/