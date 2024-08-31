/*
============================================================================
Name : 1b.c
Author : Tushar Dubey
Description : Create the following types of a files using (i) shell command (ii) system call
 		b. hard link (link system call)
Date: 30th Aug, 2024.
============================================================================
*/


#include <stdio.h>
#include <unistd.h>

int main()
{
        const char *target_file = "file1";
        const char *hardlink_name = "file1_hard_link";

        if(link(target_file, hardlink_name) == -1)
        {
                perror("hardlink");
                return 1;
        }
        printf("Success");
        return 0;
}

//OUTPUT
//Success