/*
============================================================================
Name : 20b.c
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
    
    // Open the FIFO for reading
    int fd = open(fifoName, O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    char buffer[100];
    read(fd, buffer, sizeof(buffer));
    
    printf("Message received: %s\n", buffer);

    // Close the FIFO
    close(fd);
    
    // Optionally, remove the FIFO
    unlink(fifoName);
    
    return 0;
}

/*
OUTPUT
============================================================================
Message sent: Hello from FIFO Writer!
*/