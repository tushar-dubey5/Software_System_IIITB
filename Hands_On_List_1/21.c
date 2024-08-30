/*
============================================================================
Name : 21.c
Author : Tushar Dubey
Description : Write a program, call fork and print the parent and child process id.
Date: 30th Aug, 2024.
============================================================================
*/

#include<stdio.h>
#include<unistd.h>
int main(){
    int child = fork();
    if(child == -1){
        printf("Error in creating a child!\n");
            }
    if(child ==0){
        printf("Child has been created with child pid of %d and parent pid of %d",getpid(), getppid());

    }
}

//OUTPUT
//Child has been created with child pid of 71270 and parent pid of 71269