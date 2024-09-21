/*
============================================================================
Name : 19e.c
Author : Tushar Dubey
Description : Create a FIFO file by
            e. mkfifo library function
Date: 20th Sept, 2024.
============================================================================
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>

int main() {
    const char *fifo_path = "myfifo_mkfifo";
    
    // Create a FIFO using mkfifo library function
    if (mkfifo(fifo_path, 0666) == -1) {
        if (errno == EEXIST) {
            printf("FIFO file already exists: %s\n", fifo_path);
        } else {
            perror("Failed to create FIFO using mkfifo");
            return 1;
        }
    } else {
        printf("FIFO file created successfully: %s\n", fifo_path);
    }

    return 0;
}
/*
OUTPUT
============================================================================
FIFO file created successfully: myfifo_mkfifo
*/