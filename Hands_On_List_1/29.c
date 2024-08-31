/*
============================================================================
Name : 29.c
Author : Tushar Dubey
Description : Write a program to get scheduling policy and modify the scheduling policy (SCHED_FIFO, SCHED_RR).
Date: 31th Aug, 2024.
============================================================================
*/


#include <sched.h> 
#include <sys/types.h> 
#include <unistd.h>    
#include <stdio.h>     

int main()
{
    int currentPolicy;
    pid_t pid;
    pid = getpid();
    currentPolicy = sched_getscheduler(pid);
    struct sched_param priority;
    
    priority.sched_priority = 10;

    switch (currentPolicy)
    {
    case SCHED_FIFO:
        printf("Current policy is FIFO\n");
        if(sched_setscheduler(pid, SCHED_RR, &priority) == -1)
            perror("sched_setscheduler");
        currentPolicy = sched_getscheduler(pid);
        if(currentPolicy == -1)
            perror("sched_getscheduler");
        printf("Current policy is %d\n", currentPolicy);
        break;
    case SCHED_RR:
        printf("Current policy is RR\n");
        if(sched_setscheduler(pid, SCHED_FIFO, &priority) == -1)
            perror("sched_setscheduler");
        currentPolicy = sched_getscheduler(pid);
        if(currentPolicy == -1)
            perror("sched_getscheduler");
        printf("Current policy is %d\n", currentPolicy);
        break;
    case SCHED_OTHER:
        printf("Current policy is OTHER\n");
        if(sched_setscheduler(pid, SCHED_RR, &priority) == -1)
            perror("sched_setscheduler");
        currentPolicy = sched_getscheduler(pid);
        if(currentPolicy == -1)
            perror("sched_getscheduler");
        printf("Current policy is %d\n", currentPolicy);
        break;
    default:
        perror("Error while getting current policy\n");
    }

    return 0;
}

/*
============================================================================
Output:
    Current policy is OTHER
    Current policy is 2
============================================================================
*/