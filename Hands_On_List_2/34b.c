/*
============================================================================
Name : 34b.c
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
#include <pthread.h>

#define PORT 8080
#define BUFFER_SIZE 1024

// Function to handle each client connection
void *handle_client(void *client_socket) {
    int sock = *((int *)client_socket);
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE); // Clear buffer

    // Read message from the client
    read(sock, buffer, sizeof(buffer));
    printf("Message from client: %s\n", buffer);

    // Send response to the client
    write(sock, "Hello from server", 17);

    // Close the client socket
    close(sock);

    return NULL;
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size = sizeof(client_addr);
    pthread_t thread_id;

    // Create the server socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    // Define server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT); // Port 8080
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

    printf("Server is listening on port %d...\n", PORT);

    while (1) {
        // Accept incoming connection
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addr_size);
        if (client_socket < 0) {
            perror("Connection failed");
            exit(1);
        }

        // Create a thread to handle the client
        if (pthread_create(&thread_id, NULL, handle_client, &client_socket) != 0) {
            perror("Thread creation failed");
            exit(1);
        }

        // Detach the thread to allow it to clean up after itself
        pthread_detach(thread_id);
    }

    return 0;
}

/*
OUTPUT
============================================================================
Server is listening on port 8080...
*/