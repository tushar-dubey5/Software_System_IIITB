/*
============================================================================
Name : 24.c
Author : Tushar Dubey
Description :  Write a program to create a message queue and print the key and message queue id.  
Date: 21st Sept, 2024.
============================================================================
*/


#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>

int main() 
{
    key_t key = ftok("fifo_file", 65);

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

    printf("Message Queue Key: %d\n", key);
    printf("Message Queue ID: %d\n", msgid);

    return 0;
}



/*
OUTPUT
============================================================================
Message Queue Key: 1090534689
Message Queue ID: 0
*/