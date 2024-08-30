/*
============================================================================
Name : 20.c
Author : Tushar Dubey
Description : Find out the priority of your running program. Modify the priority with nice command.
Date: 30th Aug, 2024.
============================================================================
*/

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
void main(int argc, char *argv[]){
   if(argc!= 2){
    printf("Invalid format!\n");
    exit(0);
   }
   int priority, new = atoi(argv[0]);
    new = 10;
    priority = nice(0); 
        printf("Current priority: %d\n", priority);
        priority = nice(new);
        printf("New priority: %d\n", priority);

}

//OUTPUT
// Current priority: 0
// New priority: 10
