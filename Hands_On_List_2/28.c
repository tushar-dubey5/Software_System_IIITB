/*
============================================================================
Name : 28.c
Author : Tushar Dubey
Description :  Write a program to change the exiting message queue permission. (use msqid_ds structure) 
Date: 21st Sept, 2024.
============================================================================
*/


#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>

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

    printf("Message queue created with ID: %d\n", msgid);

    struct msqid_ds mq_info;

    if (msgctl(msgid, IPC_STAT, &mq_info) == -1) 
    {
        perror("msgctl IPC_STAT");
        exit(EXIT_FAILURE);
    }

    printf("Current permissions: %o\n", mq_info.msg_perm.mode);

    mq_info.msg_perm.mode = 0777;

    if (msgctl(msgid, IPC_SET, &mq_info) == -1) 
    {
        perror("msgctl IPC_SET");
        exit(EXIT_FAILURE);
    }

    printf("Permissions changed to: %o\n", mq_info.msg_perm.mode);

    if (msgctl(msgid, IPC_RMID, NULL) == -1) 
    {
        perror("msgctl IPC_RMID");
        exit(EXIT_FAILURE);
    }

    return 0;
}


/*
OUTPUT
============================================================================
Current permissions: 666
Message queue permissions changed successfully!
Updated permissions: 777
*/