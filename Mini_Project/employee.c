#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "employee.h"
#include "transaction.h"
#include "account.h"
#include "loan.h"
#include "user.h"
#include "customer.h"
#include <sys/socket.h>
#include <ctype.h>
#include "logout.h"

#define BUFFER_SIZE 1024

// Helper function prototypes
static void send_message(int socket, const char *message);
static void receive_message(int socket, char *buffer);

void trim_whitespace(char *str)
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

void add_new_customer(int employee_id, int socket)
{
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];

    send_message(socket, "Enter new customer's username: ");
    receive_message(socket, username);
    trim_whitespace(username);

    send_message(socket, "Enter new customer's password: ");
    receive_message(socket, password);
    trim_whitespace(password);

    if (strlen(username) == 0 || strlen(password) == 0)
    {
        send_message(socket, "Username or password cannot be empty.\n");
        return;
    }

    User *new_user = create_user(username, password, CUSTOMER);
    if (new_user == NULL)
    {
        send_message(socket, "Failed to create new customer.\n");
        return;
    }

    Account *new_account = create_account(new_user->id);
    if (new_account == NULL)
    {
        send_message(socket, "Failed to create account for new customer.\n");
        return;
    }

    char success_msg[BUFFER_SIZE];
    snprintf(success_msg, BUFFER_SIZE, "New customer created successfully. User ID: %d, Account ID: %d\n",
             new_user->id, new_account->id);
    send_message(socket, success_msg);
}

void modify_customer_details(int employee_id, int socket)
{
    char buffer[BUFFER_SIZE];
    int user_id;

    send_message(socket, "Enter customer's User ID: ");
    receive_message(socket, buffer);
    trim_whitespace(buffer);
    user_id = atoi(buffer);

    User *user = find_user_by_id(user_id);
    if (user == NULL || user->role != CUSTOMER)
    {
        send_message(socket, "Customer not found.\n");
        return;
    }

    send_message(socket, "Enter new username (or press enter to keep current): ");
    receive_message(socket, buffer);
    trim_whitespace(buffer);

    if (strlen(buffer) > 0)
    {
        strncpy(user->username, buffer, MAX_USERNAME_LENGTH - 1);
        user->username[MAX_USERNAME_LENGTH - 1] = '\0';
    }

    send_message(socket, "Enter new password (or press enter to keep current): ");
    receive_message(socket, buffer);
    trim_whitespace(buffer);


    if (strlen(buffer) > 0)
    {
        update_user_password(user->id, buffer);
    }

    if (update_user(user))
    {
        send_message(socket, "Customer details updated successfully.\n");
    }
    else
    {
        send_message(socket, "Failed to update customer details.\n");
    }
}

void process_loan_applications(int employee_id, int socket)
{
    Loan *pending_loans;
    int count = get_pending_loans(&pending_loans);

    if (count == 0)
    {
        send_message(socket, "No pending loan applications.\n");
        return;
    }

    char loan_list[BUFFER_SIZE * 10] = "Pending Loan Applications:\n";
    char loan_str[BUFFER_SIZE];

    for (int i = 0; i < count; i++)
    {
        snprintf(loan_str, BUFFER_SIZE, "%d. User ID: %d, Amount: Rs%.2f\n",
                 pending_loans[i].id, pending_loans[i].user_id, pending_loans[i].amount);
        strcat(loan_list, loan_str);
    }

    send_message(socket, loan_list);

    char buffer[BUFFER_SIZE];
    int loan_id;
    char decision;

    send_message(socket, "Enter Loan ID to process: ");
    receive_message(socket, buffer);
    loan_id = atoi(buffer);

    send_message(socket, "Approve (A) or Reject (R) the loan? ");
    receive_message(socket, buffer);
    trim_whitespace(buffer);
    decision = buffer[0];

    if (process_loan(loan_id, employee_id, decision == 'A' || decision == 'a'))
    {
        send_message(socket, "Loan processed successfully.\n");
    }
    else
    {
        send_message(socket, "Failed to process loan.\n");
    }

    free(pending_loans);
}

void view_customer_transactions(int employee_id, int socket)
{
    char buffer[BUFFER_SIZE];
    int user_id;

    send_message(socket, "Enter customer's User ID: ");
    receive_message(socket, buffer);
    user_id = atoi(buffer);

    Account *account = find_account_by_user_id(user_id);
    if (account == NULL)
    {
        send_message(socket, "Customer account not found.\n");
        return;
    }

    Transaction *transactions;
    int count = get_transactions_by_account_id(account->id, &transactions);

    if (count == 0)
    {
        send_message(socket, "No transactions found for this customer.\n");
        return;
    }

    char history[BUFFER_SIZE * 10] = "Transaction History:\n";
    char transaction_str[BUFFER_SIZE];

    for (int i = 0; i < count; i++)
    {
        snprintf(transaction_str, BUFFER_SIZE, "%s Rs%.2f on %s\n",
                 transactions[i].type == DEPOSIT ? "Deposit" : transactions[i].type == WITHDRAW ? "Withdrawal"
                                                                                                : "Transfer",
                 transactions[i].amount,
                 ctime(&transactions[i].timestamp));
        strcat(history, transaction_str);
    }

    send_message(socket, history);
    free(transactions);
}


void handle_employee_menu(int employee_id, int socket)
{
    char buffer[BUFFER_SIZE];
    int choice;

    while (1)
    {
        send_message(socket, "\nEmployee Menu:\n"
                             "1. Add New Customer\n"
                             "2. Modify Customer Details\n"
                             "3. Process Loan Applications\n"
                             "4. View Customer Transactions\n"
                             "5. Change Password\n"
                             "6. Logout\n"
                             "Enter your choice: ");

        receive_message(socket, buffer);
        choice = atoi(buffer);

        switch (choice)
        {
        case 1:
            add_new_customer(employee_id, socket);
            break;
        case 2:
            modify_customer_details(employee_id, socket);
            break;
        case 3:
            process_loan_applications(employee_id, socket);
            break;
        case 4:
            view_customer_transactions(employee_id, socket);
            break;
        case 5:
            change_password(employee_id, socket);
            break;
        case 6:
            logout(employee_id, socket);
            return;
        default:
            send_message(socket, "Invalid choice. Please try again.\n");
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