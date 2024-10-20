#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <errno.h>
#include <signal.h>
#include <ctype.h>
#include "user.h"
#include "account.h"
#include "transaction.h"
#include "loan.h"
#include "customer.h"
#include "manager.h"
#include "employee.h"
#include "admin.h"
#include "logout.h"

#define PORT 8080
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

// Mutex for thread-safe operations
pthread_mutex_t file_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t transaction_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t account_mutex = PTHREAD_MUTEX_INITIALIZER;

// Thread function prototypes
void *handle_client(void *socket_desc);
void send_message(int socket, const char *message);
void receive_message(int socket, char *buffer);
User *handle_login(int socket);
// void handle_logout(int socket, User *user);

// Structure to pass data to threads
typedef struct {
    int socket;
    struct sockaddr_in address;
} client_data;

// Function to safely lock file operations
void lock_file_operations() {
    pthread_mutex_lock(&file_mutex);
}

// Function to safely unlock file operations
void unlock_file_operations() {
    pthread_mutex_unlock(&file_mutex);
}

// Function to safely lock transaction operations
void lock_transaction_operations() {
    pthread_mutex_lock(&transaction_mutex);
}

// Function to safely unlock transaction operations
void unlock_transaction_operations() {
    pthread_mutex_unlock(&transaction_mutex);
}

// Function to safely lock account operations
void lock_account_operations() {
    pthread_mutex_lock(&account_mutex);
}

// Function to safely unlock account operations
void unlock_account_operations() {
    pthread_mutex_unlock(&account_mutex);
}

static void trim_whitespace(char* str) {
    char* end;
    while(isspace((unsigned char)*str)) str++;
    if(*str == 0) return;
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;
    *(end + 1) = '\0';
}

int main() {
    int server_fd;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    pthread_t thread_id;

    // Initialize systems
    if (!initialize_user_system() || !initialize_account_system() || 
        !initialize_transaction_system() || !initialize_loan_system()) {
        fprintf(stderr, "Failed to initialize systems\n");
        exit(EXIT_FAILURE);
    }

    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set socket options
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(server_fd, MAX_CLIENTS) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    while (1) {
        client_data *client = malloc(sizeof(client_data));
        client->socket = accept(server_fd, (struct sockaddr *)&client->address, 
                              (socklen_t*)&addrlen);
        
        if (client->socket < 0) {
            perror("accept");
            free(client);
            continue;
        }

        // Create a new thread for each client
        if (pthread_create(&thread_id, NULL, handle_client, (void*)client) < 0) {
            perror("could not create thread");
            close(client->socket);
            free(client);
            continue;
        }

        pthread_detach(thread_id);
    }

    // Cleanup
    pthread_mutex_destroy(&file_mutex);
    pthread_mutex_destroy(&transaction_mutex);
    pthread_mutex_destroy(&account_mutex);
    
    close_transaction_system();
    close_account_system();
    close_user_system();

    return 0;
}

void *handle_client(void *data) {
    client_data *client = (client_data*)data;
    int socket_desc = client->socket;
    char buffer[BUFFER_SIZE] = {0};
    
    send_message(socket_desc, "Welcome to the Banking Management System\n");

    User *user = handle_login(socket_desc);
    
    if (user != NULL) {
        if (user->role == CUSTOMER) {
            handle_customer_menu(user->id, socket_desc);
        }
        else if (user->role == MANAGER) {
            handle_manager_menu(user->id, socket_desc);
        }
        else if (user->role == EMPLOYEE) {
            handle_employee_menu(user->id, socket_desc);
        }
        else if (user->role == ADMIN) {
            handle_admin_menu(user->id, socket_desc);
        }
    }

    close(socket_desc);
    free(client);
    return NULL;
}

void send_message(int socket, const char *message) {
    send(socket, message, strlen(message), 0);
}

void receive_message(int socket, char *buffer) {
    int valread = read(socket, buffer, BUFFER_SIZE);
    if (valread > 0) {
        buffer[valread] = '\0';
        char *newline = strchr(buffer, '\n');
        if (newline)
            *newline = '\0';
        trim_whitespace(buffer);
    }
}

void logout(int user_id, int socket)
{
    User *user = find_user_by_id(user_id);
    if (user != NULL)
    {
        if (remove_logged_in_user(user->username))
        {
            send_message(socket, "Successfully logged out. Goodbye!\n");
        }
        else
        {
            send_message(socket, "Error: Could not log out properly. Please try again later.\n");
        }
    }
    else
    {
        send_message(socket, "Error: User not found.\n");
    }
}

User *handle_login(int socket) {
    char username[MAX_USERNAME_LENGTH] = {0};
    char password[MAX_PASSWORD_LENGTH] = {0};
    User *user = NULL;

    while (1) {
        send_message(socket, "Username: ");
        receive_message(socket, username);

        if (is_user_logged_in(username)) {
            send_message(socket, "You are already logged in from another session.\n");
            continue;
        }
        
        send_message(socket, "Password: ");
        receive_message(socket, password);

        lock_file_operations();
        if (authenticate_user(username, password)) {
            user = find_user_by_username(username);
            unlock_file_operations();
            break;
        }
        unlock_file_operations();
        
        send_message(socket, "Invalid username or password. Please try again.\n");
    }
     
    if (user != NULL) {
        char welcome_msg[BUFFER_SIZE];
        snprintf(welcome_msg, BUFFER_SIZE, "Welcome, %s! You are logged in as %s.\n",
                username,
                user->role == CUSTOMER ? "Customer" : 
                user->role == EMPLOYEE ? "Employee" :
                user->role == MANAGER ? "Manager" : "Administrator");
        send_message(socket, welcome_msg);

        if (!add_logged_in_user(username)) {
            send_message(socket, "Error logging in. Please try again later.\n");
            return NULL;
        }
    }

    return user;
}

// void handle_logout(int socket, User *user) {
//     if (user == NULL) {
//         send_message(socket, "No user is currently logged in.\n");
//         return;
//     }
    
//     switch (user->role) {
//         case CUSTOMER:
//             logout_customer(user);
//             break;
//         case EMPLOYEE:
//             logout_employee(user);
//             break;
//         case MANAGER:
//             logout_manager(user);
//             break;
//         case ADMIN:
//             logout_admin(user);
//             break;
//         default:
//             send_message(socket, "Invalid user role.\n");
//             break;
//     }

//     send_message(socket, "You have successfully logged out.\n");
// }