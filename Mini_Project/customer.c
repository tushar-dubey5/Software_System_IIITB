#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/file.h>
#include <sys/socket.h>
#include "customer.h"
#include "transaction.h"
#include "loan.h"
#include "user.h"
#include <ctype.h>
#include "feedback.h"
#include "logout.h"

#define BUFFER_SIZE 1024

static void send_message(int socket, const char *message);
static void receive_message(int socket, char *buffer);

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


void add_customer_feedback(int user_id, int socket)
{
    char buffer[BUFFER_SIZE];
    char comment[MAX_COMMENT_LENGTH];
    int rating;

    send_message(socket, "Enter your rating (1-5): ");
    receive_message(socket, buffer);
    rating = atoi(buffer);

    if (rating < 1 || rating > 5)
    {
        send_message(socket, "Error: Invalid rating. Please enter a value between 1 and 5.\n");
        return;
    }

    send_message(socket, "Enter your comment: ");
    receive_message(socket, comment);
    trim_whitespace(comment);

    Feedback feedback;
    feedback.user_id = user_id;
    feedback.rating = rating;
    strncpy(feedback.comment, comment, MAX_COMMENT_LENGTH - 1);
    feedback.comment[MAX_COMMENT_LENGTH - 1] = '\0';

    if (add_feedback(&feedback))
    {
        send_message(socket, "Thank you for your feedback!\n");
    }
    else
    {
        send_message(socket, "Error: Unable to store feedback. Please try again later.\n");
    }
}

void view_account_balance(int user_id, int socket)
{
    Account *account = find_account_by_user_id(user_id);
    if (account == NULL)
    {
        send_message(socket, "Error: Account not found.\n");
        return;
    }

    if (account->is_active != 1)
    {
        send_message(socket, "Error: Your account is currently inactive. Contact support for assistance.\n");
        return;
    }

    char balance_msg[BUFFER_SIZE];
    snprintf(balance_msg, BUFFER_SIZE, "Your current balance is: Rs%.2f\n", account->balance);
    send_message(socket, balance_msg);
}

void deposit_money(int user_id, int socket)
{
    Account *account = find_account_by_user_id(user_id);
    if (account == NULL)
    {
        send_message(socket, "Error: Account not found.\n");
        return;
    }
    if (account->is_active != 1)
    {
        send_message(socket, "Error: Your account is currently inactive. Contact support for assistance.\n");
        return;
    }
    char buffer[BUFFER_SIZE];
    double amount;

    send_message(socket, "Enter the amount to deposit: Rs");
    receive_message(socket, buffer);
    amount = atof(buffer);
    trim_whitespace(buffer);

    if (amount <= 0)
    {
        send_message(socket, "Error: Invalid amount. Please enter a positive number.\n");
        return;
    }

    if (update_account_balance(account->id, amount, DEPOSIT))
    {
        char success_msg[BUFFER_SIZE];
        snprintf(success_msg, BUFFER_SIZE, "Successfully deposited Rs%.2f. New balance: Rs%.2f\n", amount, account->balance);
        send_message(socket, success_msg);

        // Record the transaction
        record_transaction(account->id, DEPOSIT, amount);
    }
    else
    {
        send_message(socket, "Error: Failed to deposit money. Please try again later.\n");
    }
}

void withdraw_money(int user_id, int socket)
{
    Account *account = find_account_by_user_id(user_id);
    if (account == NULL)
    {
        send_message(socket, "Error: Account not found.\n");
        return;
    }
    if (account->is_active != 1)
    {
        send_message(socket, "Error: Your account is currently inactive. Contact support for assistance.\n");
        return;
    }
    char buffer[BUFFER_SIZE];
    double amount;

    send_message(socket, "Enter the amount to withdraw: Rs");
    receive_message(socket, buffer);
    amount = atof(buffer);
    trim_whitespace(buffer);

    if (amount <= 0)
    {
        send_message(socket, "Error: Invalid amount. Please enter a positive number.\n");
        return;
    }

    if (amount > account->balance)
    {
        send_message(socket, "Error: Insufficient funds.\n");
        return;
    }

    if (update_account_balance(account->id, amount, WITHDRAW))
    {
        char success_msg[BUFFER_SIZE];
        snprintf(success_msg, BUFFER_SIZE, "Successfully withdrew Rs%.2f. New balance: Rs%.2f\n", amount, account->balance);
        send_message(socket, success_msg);

        // Record the transaction
        record_transaction(account->id, WITHDRAW, amount);
    }
    else
    {
        send_message(socket, "Error: Failed to withdraw money. Please try again later.\n");
    }
}

void transfer_funds(int user_id, int socket)
{
    Account *from_account = find_account_by_user_id(user_id);
    if (from_account == NULL)
    {
        send_message(socket, "Error: Your account not found.\n");
        return;
    }
    if (from_account->is_active != 1)
    {
        send_message(socket, "Error: Your account is currently inactive. Contact support for assistance.\n");
        return;
    }
    char buffer[BUFFER_SIZE];
    int to_account_id;
    double amount;

    send_message(socket, "Enter the recipient's account ID: ");
    receive_message(socket, buffer);
    trim_whitespace(buffer);
    to_account_id = atoi(buffer);

    Account *to_account = find_account_by_id(to_account_id);
    if (to_account == NULL)
    {
        send_message(socket, "Error: Recipient's account not found.\n");
        return;
    }

    send_message(socket, "Enter the amount to transfer: Rs");
    receive_message(socket, buffer);
    trim_whitespace(buffer);
    amount = atof(buffer);

    if (amount <= 0)
    {
        send_message(socket, "Error: Invalid amount. Please enter a positive number.\n");
        return;
    }

    if (amount > from_account->balance)
    {
        send_message(socket, "Error: Insufficient funds.\n");
        return;
    }

    if (transfer_between_accounts(from_account->id, to_account_id, amount))
    {
        char success_msg[BUFFER_SIZE];
        snprintf(success_msg, BUFFER_SIZE, "Successfully transferred Rs%.2f to account %d. Your new balance: Rs%.2f\n",
                 amount, to_account_id, from_account->balance);
        send_message(socket, success_msg);

        // Record the transactions
        record_transaction(from_account->id, TRANSFER, -amount);
        record_transaction(to_account_id, TRANSFER, amount);
    }
    else
    {
        send_message(socket, "Error: Failed to transfer funds. Please try again later.\n");
    }
}

void apply_for_loan(int user_id, int socket)
{
    char buffer[BUFFER_SIZE];
    double amount;

    send_message(socket, "Enter the loan amount you wish to apply for: Rs");
    receive_message(socket, buffer);
    trim_whitespace(buffer);
    amount = atof(buffer);

    if (amount <= 0)
    {
        send_message(socket, "Error: Invalid amount. Please enter a positive number.\n");
        return;
    }

    if (create_loan_application(user_id, amount))
    {
        char success_msg[BUFFER_SIZE];
        snprintf(success_msg, BUFFER_SIZE, "Loan application for Rs%.2f for user ID:%d submitted successfully. It will be reviewed by our staff.\n", amount,user_id);
        send_message(socket, success_msg);
    }
    else
    {
        send_message(socket, "Error: Failed to submit loan application. Please try again later.\n");
    }
}

void view_transaction_history(int user_id, int socket)
{
    Account *account = find_account_by_user_id(user_id);
    if (account == NULL)
    {
        send_message(socket, "Error: Account not found.\n");
        return;
    }
    if (account->is_active != 1)
    {
        send_message(socket, "Error: Your account is currently inactive. Contact support for assistance.\n");
        return;
    }

    Transaction *transactions;
    int count = get_transactions_by_account_id(account->id, &transactions);

    if (count == 0)
    {
        send_message(socket, "No transactions found.\n");
        return;
    }

    char history[BUFFER_SIZE * 10] = "Transaction History:\n";
    char transaction_str[BUFFER_SIZE];

    for (int i = 0; i < count; i++)
    {
        snprintf(transaction_str, BUFFER_SIZE, "%s Rs%.2f on %ld\n",
                 transactions[i].type == DEPOSIT ? "Deposit" : transactions[i].type == WITHDRAW ? "Withdrawal"
                                                                                                : "Transfer",
                 transactions[i].amount,
                 (long)transactions[i].timestamp);
        strcat(history, transaction_str);
    }

    send_message(socket, history);
    free(transactions);
}

void change_password(int user_id, int socket)
{
    char old_password[MAX_PASSWORD_LENGTH];
    char new_password[MAX_PASSWORD_LENGTH];

    send_message(socket, "Enter your current password: ");
    receive_message(socket, old_password);
    trim_whitespace(old_password);

    User *user = find_user_by_id(user_id);
    if (user == NULL || !authenticate_user(user->username, old_password))
    {
        send_message(socket, "Error: Invalid current password.\n");
        return;
    }

    send_message(socket, "Enter your new password: ");
    receive_message(socket, new_password);
    trim_whitespace(new_password);

    if (update_user_password(user_id, new_password))
    {
        send_message(socket, "Password changed successfully.\n");
    }
    else
    {
        send_message(socket, "Error: Failed to change password. Please try again later.\n");
    }
}

void handle_customer_menu(int user_id, int socket)
{
    char buffer[BUFFER_SIZE];
    int choice;

    while (1)
    {
        send_message(socket, "\nCustomer Menu:\n"
                             "1. View Account Balance\n"
                             "2. Deposit Money\n"
                             "3. Withdraw Money\n"
                             "4. Transfer Funds\n"
                             "5. Apply for a Loan\n"
                             "6. View Transaction History\n"
                             "7. Change Password\n"
                             "8. Add Feedback\n"
                             "9. Logout\n"
                             "Enter your choice: ");

        receive_message(socket, buffer);
        choice = atoi(buffer);

        switch (choice)
        {
        case 1:
            view_account_balance(user_id, socket);
            break;
        case 2:
            deposit_money(user_id, socket);
            break;
        case 3:
            withdraw_money(user_id, socket);
            break;
        case 4:
            transfer_funds(user_id, socket);
            break;
        case 5:
            apply_for_loan(user_id, socket);
            break;
        case 6:
            view_transaction_history(user_id, socket);
            break;
        case 7:
            change_password(user_id, socket);
            break;
        case 8:
            add_customer_feedback(user_id, socket);
            break;
        case 9:
            logout(user_id, socket);
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

// static void send_message(int socket, const char* message) {
//     size_t len = strlen(message);
//     ssize_t sent = send(socket, message, len, 0);
//     if (sent == -1) {
//         perror("Error sending message");
//     } else {
//         printf("Sent message: %s", message); // Debug
//     }
// }

// static void receive_message(int socket, char* buffer) {
//     memset(buffer, 0, BUFFER_SIZE);
//     ssize_t valread = recv(socket, buffer, BUFFER_SIZE - 1, 0);
//     if (valread > 0) {
//         buffer[valread] = '\0';
//         char* newline = strchr(buffer, '\n');
//         if (newline) *newline = '\0';
//         char* carriage_return = strchr(buffer, '\r');
//         if (carriage_return) *carriage_return = '\0';
//         printf("Received message: %s\n", buffer); // Debug
//     } else if (valread == 0) {
//         fprintf(stderr, "Client disconnected\n");
//     } else {
//         perror("Error receiving message");
//     }
// }