/*
============================================================================
Name : 21b.c
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
    const char *fifo1 = "/tmp/myfifo1"; // FIFO for reading
    const char *fifo2 = "/tmp/myfifo2"; // FIFO for writing

    char buffer[100];

    while (1) {
        // Open FIFO for reading
        int fd1 = open(fifo1, O_RDONLY);
        read(fd1, buffer, sizeof(buffer)); // Read message
        printf("Received: %s\n", buffer);
        close(fd1);

        // Open FIFO for writing
        int fd2 = open(fifo2, O_WRONLY);
        printf("Enter message to send: ");
        char message[100];
        fgets(message, sizeof(message), stdin);
        write(fd2, message, strlen(message) + 1); // Send response
        close(fd2);
    }

    // Cleanup (not reached in this example)
    return 0;
}
/*
OUTPUT
============================================================================
Received: Hello This is 21a

Enter message to send: Hello this is 21b
Received: Hello This is 21a
*/