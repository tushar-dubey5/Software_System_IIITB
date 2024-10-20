// admin.c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "admin.h"
#include "user.h"
#include "account.h"
#include "customer.h"
#include <sys/socket.h>
#include <ctype.h>
#include "logout.h"

#define BUFFER_SIZE 1024

static void send_message(int socket, const char *message);
static void receive_message(int socket, char *buffer);


void view_all_users(int socket_desc) {
    char buffer[BUFFER_SIZE] = {0};
    User *users = NULL;
    int user_count = get_all_users(&users);

    send_message(socket_desc, "All Users:\n");
    for (int i = 0; i < user_count; i++) {
        snprintf(buffer, BUFFER_SIZE, "ID: %d, Username: %s, Role: %s\n",
                 users[i].id, users[i].username,
                 users[i].role == CUSTOMER ? "Customer" :
                 users[i].role == EMPLOYEE ? "Employee" :
                 users[i].role == MANAGER ? "Manager" : "Admin");
        send_message(socket_desc, buffer);
    }

    free(users);
}

static void trim_whitespace(char* str) {
    char* end;

    // Trim leading space
    while (isspace((unsigned char)*str)) str++;

    // All spaces
    if (*str == 0) return;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    // Write new null terminator
    *(end + 1) = '\0';
}
void create_new_employee(int socket_desc) {
    char username[MAX_USERNAME_LENGTH] = {0};
    char password[MAX_PASSWORD_LENGTH] = {0};
    char role_str[BUFFER_SIZE] = {0};
    UserRole role;

    send_message(socket_desc, "Enter new employee username: ");
    receive_message(socket_desc, username);
    trim_whitespace(username);

    send_message(socket_desc, "Enter new employee password: ");
    receive_message(socket_desc, password);
    trim_whitespace(password);

    send_message(socket_desc, "Enter role (manager, employee): ");
    receive_message(socket_desc, role_str);
    trim_whitespace(role_str);

    for (char *p = role_str; *p; ++p) *p = tolower (*p);

    if (strcmp(role_str, "manager") == 0) {
        role = MANAGER;
    } else if (strcmp(role_str, "employee") == 0) {
        role = EMPLOYEE;
    } else {
        send_message(socket_desc, "Invalid role specified.\n");
        return;
    }

    User *new_user = create_user(username, password, role);
    if (new_user) {
        send_message(socket_desc, "Employee created successfully.\n");
    } else {
        send_message(socket_desc, "Failed to create employee. Username might already exist.\n");
    }
}

void modify_user_details(int socket_desc) {
    char buffer[BUFFER_SIZE] = {0};
    char new_username[MAX_USERNAME_LENGTH] = {0};
    User *user = NULL;
    int user_id;

    send_message(socket_desc, "Enter user ID to modify: ");
    receive_message(socket_desc, buffer);
    user_id = atoi(buffer);

    user = find_user_by_id(user_id);
    if (user == NULL) {
        send_message(socket_desc, "User not found.\n");
        return;
    }

    send_message(socket_desc, "Enter new username (press Enter to keep current): ");
    receive_message(socket_desc, new_username);
    trim_whitespace(new_username);

    if (strlen(new_username) > 0) {
        strncpy(user->username, new_username, MAX_USERNAME_LENGTH - 1);
        user->username[MAX_USERNAME_LENGTH - 1] = '\0';
    }

    if (update_user(user)) {
        send_message(socket_desc, "User details updated successfully.\n");
    } else {
        send_message(socket_desc, "Failed to update user details.\n");
    }
}

void change_user_role(int socket_desc) {
    char buffer[BUFFER_SIZE] = {0};
    char role_str[BUFFER_SIZE] = {0};
    User *user = NULL;
    int user_id;
    UserRole role;

    send_message(socket_desc, "Enter user ID to change role: ");
    receive_message(socket_desc, buffer);
    user_id = atoi(buffer);

    user = find_user_by_id(user_id);
    if (user == NULL) {
        send_message(socket_desc, "User not found.\n");
        return;
    }

    send_message(socket_desc, "Enter new role (admin, manager, employee, customer): ");
    receive_message(socket_desc, role_str);
    trim_whitespace(role_str);

    for (char* p = role_str; *p; ++p) *p = tolower(*p);

    if (strcmp(role_str, "admin") == 0) {
        role = ADMIN;
    } else if (strcmp(role_str, "manager") == 0) {
        role = MANAGER;
    } else if (strcmp(role_str, "employee") == 0) {
        role = EMPLOYEE;
    }else if (strcmp(role_str, "customer") == 0) {
        role = CUSTOMER;
    } else {
        send_message(socket_desc, "Invalid role specified.\n");
        return;
    }

    user->role = role;

    if (update_user(user)) {
        send_message(socket_desc, "User role updated successfully.\n");
    } else {
        send_message(socket_desc, "Failed to update user role.\n");
    }
}

void change_admin_password(int socket_desc, int user_id) {
    char new_password[MAX_PASSWORD_LENGTH] = {0};

    send_message(socket_desc, "Enter new password: ");
    receive_message(socket_desc, new_password);
    trim_whitespace(new_password);

    if (strlen(new_password) == 0) {
        send_message(socket_desc, "Password cannot be empty.\n");
        return;
    }

    if (update_user_password(user_id, new_password)) {
        send_message(socket_desc, "Password updated successfully.\n");
    } else {
        send_message(socket_desc, "Failed to update password.\n");
    }
}

void handle_admin_menu(int user_id, int socket_desc) {
    char buffer[BUFFER_SIZE] = {0};
    int choice;

    while (1) {
        send_message(socket_desc,
            "\nAdmin Menu:\n"
            "1. View all users\n"
            "2. Add new bank employee\n"
            "3. Modify customer/employee details\n"
            "4. Manage user roles\n"
            "5. Change password\n"
            "6. Logout\n"
            "7. Exit\n"
            "Enter your choice: ");
        receive_message(socket_desc, buffer);
        choice = atoi(buffer);

        switch (choice) {
            case 1:
                view_all_users(socket_desc);
                break;
            case 2:
                create_new_employee(socket_desc);
                break;
            case 3:
                modify_user_details(socket_desc);
                break;
            case 4:
                change_user_role(socket_desc);
                break;
            case 5:
                change_admin_password(socket_desc, user_id);
                break;
            case 6:
                logout(user_id,socket_desc);
                return;
            case 7:
                remove_logged_in_user(find_user_by_id(user_id)->username);
                send_message(socket_desc, "Exiting...\n");
                exit(0);
            default:
                send_message(socket_desc, "Invalid choice. Please try again.\n");
                break;
        }
    }
}

static void send_message(int socket, const char *message)
{
    send(socket, message, strlen(message), 0);
}

static void receive_message(int socket, char *buffer)
{
    int valread = read(socket, buffer, BUFFER_SIZE);
    if (valread > 0)
    {
        buffer[valread] = '\0';
        // Remove newline character if present
        char *newline = strchr(buffer, '\n');
        if (newline)
            *newline = '\0';
    }
}