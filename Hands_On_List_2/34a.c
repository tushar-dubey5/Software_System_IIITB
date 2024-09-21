/*
============================================================================
Name : 34a.c
Author : Tushar Dubey
Description :  Write a program to communicate between two machines using socket. 
Date: 21st Sept, 2024.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size = sizeof(client_addr);
    char buffer[1024];

    // Create the server socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    // Define server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080); // Port 8080
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the specified IP and port
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Binding failed");
        exit(1);
    }

    // Listen for incoming connections (queue size of 5)
    if (listen(server_socket, 5) < 0) {
        perror("Listening failed");
        exit(1);
    }

    printf("Server is listening on port 8080...\n");

    while (1) {
        // Accept incoming connection
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addr_size);
        if (client_socket < 0) {
            perror("Connection failed");
            exit(1);
        }

        // Create a child process to handle the client
        if (fork() == 0) {  // In the child process
            // Read message from the client
            memset(buffer, 0, sizeof(buffer)); // Clear buffer
            read(client_socket, buffer, sizeof(buffer));
            printf("Message from client: %s\n", buffer);

            // Send response to the client
            write(client_socket, "Hello from server", 17);

            // Close the client socket
            close(client_socket);
            exit(0);  // Exit child process after handling client
        }

        // Parent process continues to listen for new connections
    }

    return 0;
}

/*
OUTPUT
============================================================================
Server is listening on port 8080...
*/
