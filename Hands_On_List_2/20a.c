/*
============================================================================
Name : 20a.c
Author : Tushar Dubey
Description : Write two programs so that both can communicate by FIFO -Use one way communication.
Date: 21st Sept, 2024.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
    const char *fifoName = "/tmp/myfifo";
    
    // Create the FIFO (named pipe)
    if (mkfifo(fifoName, 0666) == -1) {
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }

    // Open the FIFO for writing
    int fd = open(fifoName, O_WRONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    const char *message = "Hello from FIFO Writer!";
    write(fd, message, sizeof(message));
    
    printf("Message sent: %s\n", message);

    // Close the FIFO
    close(fd);
    
    return 0;
}

/*
OUTPUT
============================================================================
Message received: Hello from FIFO Writer!
*/
