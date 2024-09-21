/*
============================================================================
Name : 19d.c
Author : Tushar Dubey
Description : Create a FIFO file by
                c. mknod system call
Date: 20th Sept, 2024.
============================================================================
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    const char *fifo_path = "myfifo_mknod";
    
    // Create a FIFO using mknod system call
    // S_IFIFO specifies that it's a FIFO file, and 0666 sets the permissions to read/write for all users.
    if (mknod(fifo_path, S_IFIFO | 0666, 0) == -1) {
        perror("Failed to create FIFO using mknod");
        return 1;
    }

    printf("FIFO file created successfully: %s\n", fifo_path);

    return 0;
}

/*
OUTPUT
============================================================================
FIFO file created successfully: myfifo_mknod
*/