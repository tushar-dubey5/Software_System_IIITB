#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "manager.h"
#include "user.h"
#include "account.h"
#include "loan.h"
#include "feedback.h"
#include "customer.h"
#include <sys/socket.h>
#include<ctype.h>
#include "logout.h"

#define BUFFER_SIZE 1024

// Helper function prototypes
static void send_message(int socket, const char* message);
static void receive_message(int socket, char* buffer);

static void trim_whitespace(char *str)
{
    char *end;

    // Trim leading space
    while (isspace((unsigned char)*str))
        str++;

    // All spaces
    if (*str == 0)
        return;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end))
        end--;

    // Write new null terminator
    *(end + 1) = '\0';
}

void activate_deactivate_account(int manager_id, int socket) {
    char buffer[BUFFER_SIZE];
    int account_id;
    int action;

    send_message(socket, "Enter the account ID to activate/deactivate: ");
    receive_message(socket, buffer);
    account_id = atoi(buffer);

    Account* account = find_account_by_id(account_id);
    if (account == NULL) {
        send_message(socket, "Error: Account not found.\n");
        return;
    }

    send_message(socket, "Enter 1 to activate or 0 to deactivate the account: ");
    receive_message(socket, buffer);
    action = atoi(buffer);

    if (action != 0 && action != 1) {
        send_message(socket, "Error: Invalid action. Please enter 0 or 1.\n");
        return;
    }

    if (set_account_status(account_id, action)) {
        char success_msg[BUFFER_SIZE];
        snprintf(success_msg, BUFFER_SIZE, "Account %d has been %s.\n", account_id, action ? "activated" : "deactivated");
        send_message(socket, success_msg);
    } else {
        send_message(socket, "Error: Failed to update account status.\n");
    }
}

void assign_loan_application(int manager_id, int socket) {
    char buffer[BUFFER_SIZE];
    int loan_id, employee_id;

    send_message(socket, "Enter the loan application ID to assign: ");
    receive_message(socket, buffer);
    loan_id = atoi(buffer);

    Loan* loan = find_loan_by_id(loan_id);
    if (loan == NULL) {
        send_message(socket, "Error: Loan application not found.\n");
        return;
    }

    // Check if the loan is already assigned to an employee
    if (loan->assigned_employee_id != -1) {
        char error_msg[BUFFER_SIZE];
        snprintf(error_msg, BUFFER_SIZE, "Error: Loan application %d is already assigned to employee %d.\n", loan_id, loan->assigned_employee_id);
        send_message(socket, error_msg);
        return;
    }

    if (loan->status != PENDING) {
        send_message(socket, "Error: This loan application is not pending.\n");
        return;
    }

    send_message(socket, "Enter the employee ID to assign this loan application: ");
    receive_message(socket, buffer);
    trim_whitespace(buffer);
    employee_id = atoi(buffer);

    User* employee = find_user_by_id(employee_id);
    if (employee == NULL || employee->role != EMPLOYEE) {
        send_message(socket, "Error: Invalid employee ID.\n");
        return;
    }

    if (assign_loan_to_employee(loan_id, employee_id)) {
        char success_msg[BUFFER_SIZE];
        snprintf(success_msg, BUFFER_SIZE, "Loan application %d has been assigned to employee %d.\n", loan_id, employee_id);
        send_message(socket, success_msg);
    } else {
        send_message(socket, "Error: Failed to assign loan application.\n");
    }
}

void review_customer_feedback(int manager_id, int socket) {
    char buffer[BUFFER_SIZE * 10] = "Customer Feedback:\n";
    char feedback_str[BUFFER_SIZE];
    int count = 0;

    Feedback* feedbacks;
    int feedback_count = get_all_feedback(&feedbacks);

    if (feedback_count == 0) {
        send_message(socket, "No customer feedback found.\n");
        return;
    }

    for (int i = 0; i < feedback_count; i++) {
        snprintf(feedback_str, BUFFER_SIZE, "ID: %d, User: %d, Rating: %d, Comment: %s\n",
                 feedbacks[i].id, feedbacks[i].user_id, feedbacks[i].rating, feedbacks[i].comment);
        strcat(buffer, feedback_str);
        count++;

        if (count == 10) {
            send_message(socket, buffer);
            buffer[0] = '\0';
            count = 0;
        }
    }

    if (count > 0) {
        send_message(socket, buffer);
    }

    free(feedbacks);
}


void handle_manager_menu(int manager_id, int socket) {
    char buffer[BUFFER_SIZE];
    int choice;

    while (1) {
        send_message(socket, "\nManager Menu:\n"
                             "1. Activate/Deactivate Customer Account\n"
                             "2. Assign Loan Application to Employee\n"
                             "3. Review Customer Feedback\n"
                             "4. Change Password\n"
                             "5. Logout\n"
                             "Enter your choice: ");

        receive_message(socket, buffer);
        choice = atoi(buffer);

        switch (choice) {
            case 1:
                activate_deactivate_account(manager_id, socket);
                break;
            case 2:
                assign_loan_application(manager_id, socket);
                break;
            case 3:
                review_customer_feedback(manager_id, socket);
                break;
            case 4:
                change_password(manager_id, socket);
                break;
            case 5:
                logout(manager_id,socket);
                return;
            default:
                send_message(socket, "Invalid choice. Please try again.\n");
        }
    }
}

static void send_message(int socket, const char* message) {
    send(socket, message, strlen(message), 0);
}

static void receive_message(int socket, char* buffer) {
    int valread = read(socket, buffer, BUFFER_SIZE);
    if (valread > 0) {
        buffer[valread] = '\0';
        // Remove newline character if present
        char* newline = strchr(buffer, '\n');
        if (newline) *newline = '\0';
    }
}