/*
============================================================================
Name : 25.c
Author : Tushar Dubey
Description : Write a program to print a message queue's (use msqid_ds and ipc_perm structures)
                a. access permission
                b. uid, gid
                c. time of last message sent and received
                d. time of last change in the message queue
                d. size of the queue
                f. number of messages in the queue
                g. maximum number of bytes allowed
                h. pid of the msgsnd and msgrcv
Date: 21st Sept, 2024.
============================================================================
*/
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    int msqid;
    struct msqid_ds buf;

    msqid = 0; 

    if (msgctl(msqid, IPC_STAT, &buf) == -1) {
        perror("msgctl failed");
        exit(1);
    }

    printf("Access Permissions: %o\n", buf.msg_perm.mode); 
    printf("UID: %d\n", buf.msg_perm.uid);
    printf("GID: %d\n", buf.msg_perm.gid);
    printf("Time of last message sent: %s", ctime(&buf.msg_stime));
    printf("Time of last message received: %s", ctime(&buf.msg_rtime));
    printf("Time of last change in the message queue: %s", ctime(&buf.msg_ctime));
    printf("Current size of queue (in bytes): %lu\n", buf.__msg_cbytes);
    printf("Number of messages in queue: %lu\n", buf.msg_qnum);
    printf("Max number of bytes allowed in queue: %lu\n", buf.msg_qbytes);
    printf("PID of last msgsnd: %d\n", buf.msg_lspid);
    printf("PID of last msgrcv: %d\n", buf.msg_lrpid);

    return 0;
}

/*
OUTPUT
============================================================================
Access Permissions: 666
UID: 1000
GID: 1000
Time of last message sent: Thu Jan  1 05:30:00 1970
Time of last message received: Thu Jan  1 05:30:00 1970
Time of last change in the message queue: Sat Sep 21 13:44:41 2024
Current size of queue (in bytes): 0
Number of messages in queue: 0
Max number of bytes allowed in queue: 16384
PID of last msgsnd: 0
PID of last msgrcv: 0
*/