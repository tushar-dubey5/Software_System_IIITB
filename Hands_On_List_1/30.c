/*
============================================================================
Name : 30.c
Author : Tushar Dubey
Description : Write a program to run a script at a specific time using a Daemon process
Date: 31th Aug, 2024.
============================================================================
*/


#include <time.h>    
#include <stdio.h>    
#include <stdlib.h>    
#include <sys/types.h> 
#include <unistd.h>    
#include <sys/stat.h>  

int main(int argc, char *argv[])
{

    time_t now, end; 
    struct tm *deadline;                

    pid_t child;

    if (argc < 2)
        printf("Pass at least one argument\n");
    else
    {
        time(&now);
        deadline = localtime(&now);

        deadline->tm_hour = atoi(argv[1]);
        deadline->tm_min = argv[2] == NULL ? 0 : atoi(argv[2]);
        deadline->tm_sec = argv[3] == NULL ? 0 : atoi(argv[3]);

        end = mktime(deadline); 

        if ((child = fork()) == 0)
        {
            setsid();
            chdir("/");
            umask(0);
            do
            {
                time(&now);
            } while (difftime(end, now) > 0);
            printf("Script Executed\n");
            exit(0);
        }
        exit(0);
    }

    return 0;
}
/*
============================================================================
Output:
    Script Executed
============================================================================
*/