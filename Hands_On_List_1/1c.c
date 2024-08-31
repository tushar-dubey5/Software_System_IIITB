/*
============================================================================
Name : 1c.c
Author : Tushar Dubey
Description : Create the following types of a files using (i) shell command (ii) system call
 		        c. FIFO (mkfifo Library Function or mknod system call)
Date: 31th Aug, 2024.
============================================================================
*/



#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

int main() {
    const char *fifo_name = "myfifo";

    if (mkfifo(fifo_name, 0666) == -1) {
        perror("mkfifo");
        return 1;
    }

    printf("Success");
    return 0;
}

// Output
// Success