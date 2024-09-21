/*
============================================================================
Name : 21a.c
Author : Tushar Dubey
Description : Write two programs so that both can communicate by FIFO -Use two way communications.
Date: 21st Sept, 2024.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main() {
    const char *fifo1 = "/tmp/myfifo1"; // FIFO for writing
    const char *fifo2 = "/tmp/myfifo2"; // FIFO for reading

    // Create the FIFOs (named pipes)
    mkfifo(fifo1, 0666);
    mkfifo(fifo2, 0666);

    char buffer[100];

    while (1) {
        // Open FIFO for writing
        int fd1 = open(fifo1, O_WRONLY);
        printf("Enter message to send: ");
        char message[100];
        fgets(message, sizeof(message), stdin);
        write(fd1, message, strlen(message) + 1); // Send message
        close(fd1);

        // Open FIFO for reading
        int fd2 = open(fifo2, O_RDONLY);
        read(fd2, buffer, sizeof(buffer)); // Read response
        printf("Received: %s\n", buffer);
        close(fd2);
    }

    // Cleanup (not reached in this example)
    unlink(fifo1);
    unlink(fifo2);

    return 0;
}

/*
OUTPUT
============================================================================
Enter message to send: Hello This is 21a
Received: Hello this is 21b

Enter message to send: ^C
*/