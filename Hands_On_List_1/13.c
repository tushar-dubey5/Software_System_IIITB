/*
============================================================================
Name : 13.c
Author : Tushar Dubey
Description : Write a program to wait for a STDIN for 10 seconds using select. Write a proper print statement to
                verify whether the data is available within 10 seconds or not (check in $man 2 select).
Date: 30th Aug, 2024.
============================================================================
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>
#include <stdlib.h>

#define TIMEOUT_SECONDS 10

int main() {
    fd_set readfds;
    struct timeval timeout;
    int result;

    FD_ZERO(&readfds);

    FD_SET(STDIN_FILENO, &readfds);
    timeout.tv_sec = TIMEOUT_SECONDS;
    timeout.tv_usec = 0;

    result = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &timeout);

    if (result == -1) {
        perror("select()");
        return 1;
    } else if (result == 0) {
        printf("No input received within %d seconds.\n", TIMEOUT_SECONDS);
    } else {

        if (FD_ISSET(STDIN_FILENO, &readfds)) {
            printf("Input is available on STDIN.\n");
        }
    }

    return 0;
}
