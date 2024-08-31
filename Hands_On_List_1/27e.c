/*
============================================================================
Name : 27e.c
Author : Tushar Dubey
Description : Write a program to execute ls -Rl by the following system calls
                e. execvp
Date: 31th Aug, 2024.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() 
{
    char *args[] = { "ls", "-Rl", NULL };
    if (execvp("ls", args) == -1) 
    {
        perror("execvp failed");
        exit(0);
    }
    return 0;
}

//OUTPUT
/*total 764
-rwxrwxr-x 1 tushar tushar 16168 Aug 31 15:17 10
-rw-rw-r-- 1 tushar tushar  1458 Aug 31 15:21 10.c
-rwxrwxr-x 1 tushar tushar 16168 Aug 31 15:31 11a
-rw-rw-r-- 1 tushar tushar  1236 Aug 31 15:31 11a.c
-rwxrwxr-x 1 tushar tushar 16168 Aug 31 15:46 11b
-rw-rw-r-- 1 tushar tushar  1448 Aug 31 15:46 11b.c
-rwxrwxr-x 1 tushar tushar 16168 Aug 31 15:49 11c
-rw-rw-r-- 1 tushar tushar  1454 Aug 31 15:49 11c.c
-rwxrwxr-x 1 tushar tushar 16128 Aug 31 02:00 12
-rw-rw-r-- 1 tushar tushar  1189 Aug 31 02:41 12.c
-rwxrwxr-x 1 tushar tushar 16136 Aug 31 02:00 13
-rw-rw-r-- 1 tushar tushar  1144 Aug 31 02:41 13.c
-rwxrwxr-x 1 tushar tushar 16088 Aug 29 14:20 14
-rw-rw-r-- 1 tushar tushar  1395 Aug 31 02:33 14.c
-rwxrwxr-x 1 tushar tushar 16024 Aug 29 14:25 15
-rw-rw-r-- 1 tushar tushar  5450 Aug 31 02:32 15.c
-rwxrwxr-x 1 tushar tushar 16176 Aug 29 14:39 16a
-rw-rw-r-- 1 tushar tushar  1380 Aug 31 02:32 16a.c
-rwxrwxr-x 1 tushar tushar 16176 Aug 29 14:44 16b
-rw-rw-r-- 1 tushar tushar  1385 Aug 31 02:31 16b.c
-rwxrwxr-x 1 tushar tushar 16304 Aug 29 15:12 17
-rw-rw-r-- 1 tushar tushar  2590 Aug 31 02:25 17.c
-rwxrwxr-x 1 tushar tushar 16000 Aug 29 16:13 19
-rw-rw-r-- 1 tushar tushar   795 Aug 31 02:30 19.c
-rwxrwxr-x 1 tushar tushar 15776 Aug 31 01:57 2
-rw-rw-r-- 1 tushar tushar  2154 Aug 31 02:23 2.c
-rwxrwxr-x 1 tushar tushar 16120 Aug 31 01:16 20
-rw-rw-r-- 1 tushar tushar   739 Aug 31 02:30 20.c
-rwxrwxr-x 1 tushar tushar 16128 Aug 31 01:24 21
-rw-rw-r-- 1 tushar tushar   671 Aug 31 02:45 21.c
-rwxrwxr-x 1 tushar tushar 16168 Aug 31 01:34 22
-rw-rw-r-- 1 tushar tushar  1115 Aug 31 02:45 22.c
-rwxrwxr-x 1 tushar tushar 16128 Aug 31 01:38 23
-rw-rw-r-- 1 tushar tushar   951 Aug 31 02:28 23.c
-rwxrwxr-x 1 tushar tushar 16128 Aug 31 01:47 24
-rw-rw-r-- 1 tushar tushar   737 Aug 31 02:28 24.c
-rwxrwxr-x 1 tushar tushar 16224 Aug 31 01:52 25
-rw-rw-r-- 1 tushar tushar  2134 Aug 31 02:27 25.c
-rwxrwxr-x 1 tushar tushar 15960 Aug 31 03:33 26
-rw-rw-r-- 1 tushar tushar   337 Aug 31 03:34 26.c
-rwxrwxr-x 1 tushar tushar 16040 Aug 31 16:04 27
-rw-rw-r-- 1 tushar tushar  4296 Aug 31 16:05 27a.c
-rwxrwxr-x 1 tushar tushar 16048 Aug 31 16:06 27b
-rw-rw-r-- 1 tushar tushar  4561 Aug 31 16:07 27b.c
-rwxrwxr-x 1 tushar tushar 16096 Aug 31 16:08 27c
-rw-rw-r-- 1 tushar tushar  4652 Aug 31 16:08 27c.c
-rwxrwxr-x 1 tushar tushar 16096 Aug 31 16:09 27d
-rw-rw-r-- 1 tushar tushar  4692 Aug 31 16:09 27d.c
-rwxrwxr-x 1 tushar tushar 16096 Aug 31 16:10 27e
-rw-rw-r-- 1 tushar tushar   549 Aug 31 16:10 27e.c
-rwxrwxr-x 1 tushar tushar 16120 Aug 31 03:38 28
-rw-rw-r-- 1 tushar tushar   546 Aug 31 03:38 28.c
-rwxrwxr-x 1 tushar tushar 16248 Aug 31 16:00 29
-rw-rw-r-- 1 tushar tushar  2052 Aug 31 16:00 29.c
-rwxrwxr-x 1 tushar tushar 16040 Aug 31 01:59 3
-rw-rw-r-- 1 tushar tushar   745 Aug 31 02:23 3.c
-rwxrwxr-x 1 tushar tushar 16416 Aug 31 16:02 30
-rw-rw-r-- 1 tushar tushar  1425 Aug 31 16:02 30.c
-rwxrwxr-x 1 tushar tushar 16208 Aug 31 01:59 4
-rw-rw-r-- 1 tushar tushar  1364 Aug 31 02:44 4.c
-rwxrwxr-x 1 tushar tushar 16072 Aug 31 01:59 5
-rw-rw-r-- 1 tushar tushar  2002 Aug 31 02:44 5.c
-rwxrwxr-x 1 tushar tushar 16096 Aug 31 01:59 6
-rw-rw-r-- 1 tushar tushar   731 Aug 31 02:44 6.c
-rwxrwxr-x 1 tushar tushar 16216 Aug 31 02:00 7
-rw-rw-r-- 1 tushar tushar  1013 Aug 31 02:43 7.c
-rwxrwxr-x 1 tushar tushar 16168 Aug 31 02:00 8
-rw-rw-r-- 1 tushar tushar   758 Aug 31 02:42 8.c
-rwxrwxr-x 1 tushar tushar 16240 Aug 31 15:04 9
-rw-rw-r-- 1 tushar tushar  2018 Aug 31 15:16 9.c
-rwx------ 1 tushar tushar    51 Aug 27 17:39 example1.txt
-rwx------ 1 tushar tushar    51 Aug 27 18:07 example2.txt
-rwx------ 1 tushar tushar   266 Aug 28 19:05 example3.txt
-rwx------ 1 tushar tushar     0 Aug 27 16:57 example4.txt
-rwx------ 1 tushar tushar    57 Aug 31 01:34 example5.txt
-r-xrwx--T 1 tushar tushar    56 Aug 31 15:31 examplefile.txt
--wx--x--T 1 tushar tushar    70 Aug 31 15:46 examplefile2,txt
-rwx--x--T 1 tushar tushar    70 Aug 31 15:46 examplefile3,txt
-rw-r-x--- 1 tushar tushar    70 Aug 31 15:49 examplefile4,txt
-rw-r--r-- 1 tushar tushar    30 Aug 31 15:17 testfile.txt
-rw------- 1 tushar tushar     7 Aug 29 15:12 ticket.txt*/