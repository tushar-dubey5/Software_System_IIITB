/*
============================================================================
Name : 26.c
Author : Tushar Dubey
Description :  Write a program to send messages to the message queue. Check $ipcs -q  
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

    struct msg_buffer message;
    message.msg_type = 1; 

    printf("Enter a message to send to the queue: ");
    fgets(message.msg_text, sizeof(message.msg_text), stdin);
    message.msg_text[strcspn(message.msg_text, "\n")] = '\0'; 

    if (msgsnd(msgid, &message, sizeof(message.msg_text), 0) == -1) 
    {
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }

    printf("Message sent to queue successfully.\n");

    return 0;
}



/*
OUTPUT
============================================================================
Enter a message to send: This is a sample message
Message sent successfully!


On Running ipcs -q
------ Message Queues --------
key        msqid      owner      perms      used-bytes   messages    
0x41003d21 0          tushar     666        25           1           

*/
