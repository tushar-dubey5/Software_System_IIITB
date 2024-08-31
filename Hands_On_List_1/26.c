/*
============================================================================
Name : 26.c
Author : Tushar Dubey
Description :Write a program to execute an executable program.
            a. use some executable program
            b. pass some input to an executable program. (for example execute an executable of $./a.out name)
Date: 31th Aug, 2024.
============================================================================
*/

#include <unistd.h> 

void main()
{
    char *executable_path = "./23";
    char *arg = "23.c";

    execl(executable_path, executable_path, arg, NULL);
}

//OUTPUT
// Parent process (PID: 80536) is sleeping...
// Child process (PID: 80537) is exiting...
// Parent process (PID: 80536) is exiting...