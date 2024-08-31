/*
============================================================================
Name : 28.c
Author : Tushar Dubey
Description : Write a program to get maximum and minimum real time priority.
============================================================================
*/

#include <sched.h> 
#include <stdio.h> 

void main()
{
    int maxPriority, minPriority;

    maxPriority = sched_get_priority_max(SCHED_RR);
    if (maxPriority == -1)
        perror("Error while finding max priority\n");
    else
        printf("The max priority with RR Scheduling Policy is : %d\n", maxPriority);

    minPriority = sched_get_priority_min(SCHED_RR);
    if (minPriority == -1)
        perror("Error while finding min priority\n");
    else
        printf("The min priority with RR Scheduling Policy is : %d\n", minPriority);
}

//OUTPUT
// The max priority with RR Scheduling Policy is : 99
// The min priority with RR Scheduling Policy is : 1