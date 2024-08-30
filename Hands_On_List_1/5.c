/*
============================================================================
Name : 5.c
Author : Tushar Dubey
Description : Write a program to create five new files with infinite loop. Execute the program in the background
                and check the file descriptor table at /proc/pid/fd.
Date: 30th Aug, 2024.
============================================================================
*/

#include<stdio.h>
#include<fcntl.h>
int main(){
    int i = 0;
    char filename[250];
    while(1)
    {
        if(i < 5)
        {
            i++;
            snprintf(filename, sizeof(filename), "example%d.txt", i);
            int fileDescriptor = creat(filename,0700);
            if(fileDescriptor == -1)
            {
            perror("Error! File not created");
            }
           else 
               printf("File created successfuly and FileDescriptor value is : %d",fileDescriptor);
            
        }
    }
}

//Here is the output for the above code
// lrwx------ 1 tushar tushar 64 Aug 27 16:58 0 -> /dev/pts/1
// lrwx------ 1 tushar tushar 64 Aug 27 16:58 1 -> /dev/pts/1
// lr-x------ 1 tushar tushar 64 Aug 27 16:58 103 -> /snap/code/167/usr/share/code/v8_context_snapshot.bin
// lrwx------ 1 tushar tushar 64 Aug 27 16:58 2 -> /dev/pts/1
// l-wx------ 1 tushar tushar 64 Aug 27 16:58 3 -> '/media/tushar/Disk1/Software System/example1.txt'
// lr-x------ 1 tushar tushar 64 Aug 27 16:58 38 -> /snap/code/167/usr/share/code/resources/app/node_modules.asar
// l-wx------ 1 tushar tushar 64 Aug 27 16:58 39 -> /home/tushar/.config/Code/logs/20240827T165633/ptyhost.log
// l-wx------ 1 tushar tushar 64 Aug 27 16:58 4 -> '/media/tushar/Disk1/Software System/example2.txt'
// l-wx------ 1 tushar tushar 64 Aug 27 16:58 5 -> '/media/tushar/Disk1/Software System/example3.txt'
// l-wx------ 1 tushar tushar 64 Aug 27 16:58 6 -> '/media/tushar/Disk1/Software System/example4.txt'
// l-wx------ 1 tushar tushar 64 Aug 27 16:58 7 -> '/media/tushar/Disk1/Software System/example5.txt'
