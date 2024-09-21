/*
============================================================================
Name : 22.c
Author : Tushar Dubey
Description : Write a program to wait for data to be written into FIFO within 10 seconds, use select
system call with FIFO.
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
#include <sys/select.h>

int main() {
    const char *fifoName = "/tmp/myfifo";
    
    // Create the FIFO (named pipe)
    if (mkfifo(fifoName, 0666) == -1) {
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }

    // Fork a child process for the writer
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // Child process (Writer)
        sleep(5); // Simulate a delay before writing
        int fd = open(fifoName, O_WRONLY);
        const char *message = "Hello from FIFO Writer!";
        write(fd, message, strlen(message) + 1);
        printf("Writer: Message sent: %s\n", message);
        close(fd);
        exit(0);
    } else { // Parent process (Reader)
        int fd = open(fifoName, O_RDONLY | O_NONBLOCK);
        if (fd == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }

        fd_set readfds;
        struct timeval timeout;
        char buffer[100];

        // Set the timeout to 10 seconds
        timeout.tv_sec = 10;
        timeout.tv_usec = 0;

        // Wait for data in the FIFO
        while (1) {
            FD_ZERO(&readfds);
            FD_SET(fd, &readfds);

            int activity = select(fd + 1, &readfds, NULL, NULL, &timeout);
            
            if (activity < 0) {
                perror("select");
                close(fd);
                exit(EXIT_FAILURE);
            }

            if (activity == 0) {
                printf("Timeout: No data received within 10 seconds.\n");
                break; // Exit the loop on timeout
            }

            if (FD_ISSET(fd, &readfds)) {
                read(fd, buffer, sizeof(buffer));
                printf("Reader: Message received: %s\n", buffer);
                break; // Exit after reading the message
            }
        }

        // Clean up
        close(fd);
        unlink(fifoName); // Remove the FIFO
    }

    return 0;
}
