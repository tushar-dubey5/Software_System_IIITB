#include <stdio.h>
#include <string.h>
#include "../include/login.h"
#include "../include/utils.h"
#include<sys/types.h>
#include<time.h>
#include<stdlib.h>
struct User user_database[] = {
    {"admin", "adminpass", 4},    // Administrator
    {"employee", "employeepass", 2}, // Bank Employee
    {"customer", "customerpass", 1, 1000.0}  // Customer
};


void logout(struct User *user) {
    // Clear user session
    memset(user->username, 0, sizeof(user->username)); // Clear username
    memset(user->password, 0, sizeof(user->password)); // Clear password
    user->balance = 0; // Optionally reset balance or other user info
    user->role = -1; // Set role to an invalid state (e.g., -1)

    printf("You have been logged out successfully.\n");
}
// Function to view account balance
void view_balance(struct User *user) {
    // Open the user database for reading
    FILE *file = fopen("data/users.dat", "rb");
    if (file == NULL) {
        printf("Error opening user database.\n");
        return;
    }

    struct User tempUser;
    int found = 0;

    // Read each user from the database
    while (fread(&tempUser, sizeof(struct User), 1, file)) {
        if (strcmp(user->username, tempUser.username) == 0) {
            // User found, display balance
            printf("Current balance: $%.2f\n", tempUser.balance);
            found = 1;
            break;
        }
    }

    fclose(file); // Close the file

    if (!found) {
        printf("User not found in the database.\n");
    }
}

// Function to deposit money
void deposit_money(struct User *user, double money) {
    double amount = money; // Use double for more precision
    if (amount > 0) {
        // Update the user's balance
        FILE *file = fopen("data/users.dat", "r+b"); // Open the user database for reading and writing
        if (file == NULL) {
            printf("Error opening user database.\n");
            return;
        }

        struct User tempUser;
        int found = 0; // To check if the user is found

        while (fread(&tempUser, sizeof(struct User), 1, file)) {
            if (strcmp(user->username, tempUser.username) == 0) {
                tempUser.balance += amount; // Update balance
                fseek(file, -sizeof(struct User), SEEK_CUR); // Move the file pointer back
                fwrite(&tempUser, sizeof(struct User), 1, file); // Write updated user back to file
                printf("Deposited: $%.2f\n", amount);
                printf("New balance: $%.2f\n", tempUser.balance);
                found = 1; // Mark user as found
                break;
            }
        }

        fclose(file); // Close the user database

        if (!found) {
            printf("User not found in the database.\n");
            return;
        }

        // Log the transaction
        struct Transaction transaction;
        strcpy(transaction.username, user->username);
        transaction.amount = amount;
        strcpy(transaction.type, "Deposit");

        // For simplicity, using a fixed date format
        time_t now = time(NULL);
        strftime(transaction.date, sizeof(transaction.date), "%Y-%m-%d", localtime(&now));

        // Open the transactions file for appending
        FILE *transFile = fopen("data/transactions.dat", "ab");
        if (transFile != NULL) {
            fwrite(&transaction, sizeof(struct Transaction), 1, transFile); // Log transaction
            fclose(transFile);
        } else {
            printf("Error logging transaction.\n");
        }
    } else {
        printf("Deposit amount must be positive.\n");
    }
}


// Function to withdraw money
void withdraw_money(struct User *user, double money) {
    double amount = money;
    

    if (amount > 0) {
        // Update the user's balance
        FILE *file = fopen("data/users.dat", "r+b");
        if (file == NULL) {
            printf("Error opening user database.\n");
            return;
        }

        struct User tempUser;
        int found = 0;

        while (fread(&tempUser, sizeof(struct User), 1, file)) {
            if (strcmp(user->username, tempUser.username) == 0) {
                if (tempUser.balance >= amount) {
                    tempUser.balance -= amount; // Deduct amount
                    fseek(file, -sizeof(struct User), SEEK_CUR);
                    fwrite(&tempUser, sizeof(struct User), 1, file);
                    printf("Withdrew: $%.2f\n", amount);
                    printf("New balance: $%.2f\n", tempUser.balance);
                    found = 1; // User found
                } else {
                    printf("Insufficient balance.\n");
                }
                break;
            }
        }

        fclose(file);

        if (!found) {
            printf("User not found in the database.\n");
            return;
        }

        // Log the transaction
        struct Transaction transaction;
        strcpy(transaction.username, user->username);
        transaction.amount = amount;
        strcpy(transaction.type, "Withdraw");

        // For simplicity, using a fixed date format
        time_t now = time(NULL);
        strftime(transaction.date, sizeof(transaction.date), "%Y-%m-%d", localtime(&now));

        FILE *transFile = fopen("data/transactions.dat", "ab");
        if (transFile != NULL) {
            fwrite(&transaction, sizeof(struct Transaction), 1, transFile);
            fclose(transFile);
        } else {
            printf("Error logging transaction.\n");
        }
    } else {
        printf("Withdraw amount must be positive.\n");
    }
}


// Function to transfer funds (for simplicity, we'll just simulate this)
void transfer_funds(struct User *user, double amount, const char *recipient) {
    if (amount > 0 && amount <= user->balance) {
        user->balance -= amount;
        printf("Transferred: $%.2f to %s\n", amount, recipient);
        printf("New balance: $%.2f\n", user->balance);
    } else {
        printf("Invalid transfer amount.\n");
    }
}

// Function to view transaction history (mock implementation)
void view_transaction_history(struct User *user) {
    FILE *file = fopen("data/transactions.dat", "rb");
    if (file == NULL) {
        printf("Error opening transaction database.\n");
        return;
    }

    struct Transaction transaction;
    int found = 0;

    printf("Transaction History for %s:\n", user->username);
    while (fread(&transaction, sizeof(struct Transaction), 1, file)) {
        if (strcmp(user->username, transaction.username) == 0) {
            printf("%s: %.2f (%s)\n", transaction.date, transaction.amount, transaction.type);
            found = 1;
        }
    }

    if (!found) {
        printf("No transactions found.\n");
    }

    fclose(file);
}

// Function to validate login
int login(struct User *user) {
    FILE *file = fopen("data/users.dat", "rb");
    if (file == NULL) {
        printf("Error opening user database.\n");
        return -1;
    }
    
    struct User tempUser;
    while (fread(&tempUser, sizeof(struct User), 1, file)) {
        if (strcmp(user->username, tempUser.username) == 0 && strcmp(user->password, tempUser.password) == 0) {
            user->role = tempUser.role;
            fclose(file);
            return user->role; // Return the role of the user
        }
    }

    fclose(file);
    return 0; // Login failed
}
void apply_for_loan(struct User *user) {
    struct LoanApplication loan;
    strcpy(loan.username, user->username);

   printf("Enter loan amount (max %.2f): ", MAX_LOAN_AMOUNT);
    scanf("%lf", &loan.amount);  // Changed to %lf for double input

    if (loan.amount <= 0) {
        printf("Loan amount must be positive.\n");
        return;
    }

    if (loan.amount > MAX_LOAN_AMOUNT) {
        printf("Loan amount exceeds maximum limit.\n");
        return;
    }

    loan.approved = 0; // Set to pending (0) initially

    // Get the current date
    time_t now = time(NULL);
    strftime(loan.date, sizeof(loan.date), "%Y-%m-%d", localtime(&now));

    // Open the loan database for appending
    FILE *file = fopen("data/loans.dat", "ab");
    if (file == NULL) {
        printf("Error opening loan database.\n");
        return;
    }

    // Write the new loan application to the file
    fwrite(&loan, sizeof(struct LoanApplication), 1, file);
    fclose(file); // Close the file

    printf("Loan application submitted successfully.\n");
}

void view_loans(struct User *user) {
    struct LoanApplication loan;
    FILE *file = fopen("data/loans.dat", "rb");
    
    if (file == NULL) {
        printf("Error opening loan database.\n");
        return;
    }

    int found = 0;
    while (fread(&loan, sizeof(struct LoanApplication), 1, file)) {
        if (strcmp(loan.username, user->username) == 0) {
            printf("Loan Amount: $%.2f, Status: %s, Date: %s\n", 
                   loan.amount, 
                   loan.approved == 0 ? "Pending" : (loan.approved == 1 ? "Approved" : "Rejected"), 
                   loan.date);
            found = 1;
        }
    }

    if (!found) {
        printf("No loans found for user: %s\n", user->username);
    }

    fclose(file); // Close the file
}

//----------Employee----------------------------//
void add_new_customer() {
    struct User newCustomer;

    printf("Enter new customer username: ");
    scanf("%s", newCustomer.username);

    printf("Enter new customer password: ");
    scanf("%s", newCustomer.password);

    newCustomer.balance = 0.0;  // Initial balance
    newCustomer.role = CUSTOMER_ROLE;

    FILE *file = fopen("data/users.dat", "ab");
    if (file == NULL) {
        printf("Error opening user database.\n");
        return;
    }

    fwrite(&newCustomer, sizeof(struct User), 1, file);
    fclose(file);

    printf("New customer added successfully!\n");
}
void modify_customer_details() {
    char username[50];
    struct User customer;
    int found = 0;

    printf("Enter the username of the customer to modify: ");
    scanf("%s", username);

    FILE *file = fopen("data/users.dat", "rb+");
    if (file == NULL) {
        printf("Error opening user database.\n");
        return;
    }

    while (fread(&customer, sizeof(struct User), 1, file)) {
        if (strcmp(customer.username, username) == 0) {
            found = 1;
            printf("Customer found. Enter new password: ");
            scanf("%s", customer.password);

            fseek(file, -sizeof(struct User), SEEK_CUR); // Move pointer back to overwrite
            fwrite(&customer, sizeof(struct User), 1, file);
            printf("Customer details updated successfully!\n");
            break;
        }
    }

    if (!found) {
        printf("Customer not found.\n");
    }

    fclose(file);
}
void process_loan_application() {
    struct LoanApplication loan;
    int loanID, found = 0;

    FILE *file = fopen("data/loans.dat", "rb+");
    if (file == NULL) {
        printf("Error opening loan database.\n");
        return;
    }

    while (fread(&loan, sizeof(struct LoanApplication), 1, file)) {
        if (loan.approved == 0) {
            printf("Loan Application for user: %s, Amount: $%.2f, Date: %s\n", loan.username, loan.amount, loan.date);
            printf("Approve (1) or Reject (0): ");
            scanf("%d", &loanID);

            loan.approved = (loanID == 1) ? 1 : -1;

            fseek(file, -sizeof(struct LoanApplication), SEEK_CUR);
            fwrite(&loan, sizeof(struct LoanApplication), 1, file);
            printf("Loan application processed.\n");
            found = 1;
        }
    }

    if (!found) {
        printf("No pending loan applications found.\n");
    }

    fclose(file);
}
void view_customer_transactions() {
    // char username[50];
    // struct Transaction transaction;
    // int found = 0;

    // printf("Enter the username of the customer to view transactions: ");
    // scanf("%s", username);

    // FILE *file = fopen("data/transactions.dat", "rb");
    // if (file == NULL) {
    //     printf("Error opening transaction database.\n");
    //     return;
    // }

    // printf("Transactions for %s:\n", username);
    // while (fread(&transaction, sizeof(struct Transaction), 1, file)) {
    //     if (strcmp(transaction.username, username) == 0) {
    //         // Ensure type is a char
    //         printf("Type: %s, Amount: $%.2f, Date: %s\n", 
    //             (transaction.type == 'D') ? "Deposit" : 
    //             (transaction.type == 'W') ? "Withdraw" : "Unknown",
    //             transaction.amount, 
    //             transaction.date);
    //         found = 1;
    //     }
    // }

    // if (!found) {
    //     printf("No transactions found for %s.\n", username);
    // }

    // fclose(file);
    printf("Will Implement soon....");
}

// Function to handle user login
void user_login() {
    struct User user;

    printf("Enter Username: ");
    scanf("%s", user.username);
    printf("Enter Password: ");
    scanf("%s", user.password);

    int role = login(&user);
    
    if (role > 0) {
        printf("Login successful. Role: %d\n", role);
        
           if (role == CUSTOMER_ROLE) {
            view_account_balance(&user);
            int choice;
            do {
                printf("\nCustomer Menu:\n");
                printf("1. View Balance\n");
                printf("2. Deposit Money\n");
                printf("3. Withdraw Money\n");
                printf("4. Transfer Funds\n");
                printf("5. View Transaction History\n");
                printf("6. To Apply For Loan\n");
                printf("7. To View Loan\n");
                printf("8. Logout\n");
                printf("Choose an option: ");
                scanf("%d", &choice);

                switch (choice) {
                    case 1:
                        view_balance(&user);
                        break;
                    case 2: {
                        double amount;
                         printf("Enter amount to deposit: ");
                        scanf("%lf", &amount);
                        deposit_money(&user, amount);
                        break;
                    }
                    case 3: {
                        double amount;
                        printf("Enter amount to withdraw: ");
                        scanf("%lf", &amount);
                        withdraw_money(&user, amount);
                        break;
                    }
                    case 4: {
                        double amount;
                        char recipient[50];
                        printf("Enter recipient username: ");
                        scanf("%s", recipient);
                        printf("Enter amount to transfer: ");
                        scanf("%lf", &amount);
                        transfer_funds(&user, amount, recipient);
                        break;
                    }
                    case 5:
                        view_transaction_history(&user);
                        break;
                    case 6:
                            apply_for_loan(&user);
                            break;
                    case 7:
                        view_loans(&user);
                        break;
                    case 8:
                        printf("Logging out...\n");
                        logout(&user);
                        exit(1);
                    default:
                        printf("Invalid choice. Please try again.\n");
                }
            } while (choice != 6);
        } else if (role == EMPLOYEE_ROLE) {
            int choice;
            do {
                printf("\nEmployee Menu:\n");
                printf("1. Add New Customer\n");
                printf("2. Modify Customer Details\n");
                printf("3. Process Loan Applications\n");
                printf("4. View Customer Transactions\n");
                printf("5. Logout\n");
                printf("Choose an option: ");
                scanf("%d", &choice);

                switch (choice) {
                    case 1:
                        add_new_customer();
                        break;
                    case 2:
                        modify_customer_details();
                        break;
                    case 3:
                        process_loan_application();
                        break;
                    case 4:
                        view_customer_transactions();
                        break;
                    case 5:
                        printf("Logging out...\n");
                        logout(&user);
                        exit(1);
                    default:
                        printf("Invalid choice. Please try again.\n");
                }
            } while (choice != 5);
        }
    } else {
        printf("Invalid username or password. Please try again.\n");
    }
}

void view_account_balance(struct User *user) {
    FILE *file = fopen("data/users.dat", "rb");
    if (file == NULL) {
        printf("Error opening user database.\n");
        return;
    }

    struct User tempUser;
    while (fread(&tempUser, sizeof(struct User), 1, file)) {
        if (strcmp(user->username, tempUser.username) == 0) {
            printf("Account Balance for %s: %.2f\n", tempUser.username, tempUser.balance);
            fclose(file);
            return;
        }
    }

    fclose(file);
    printf("User not found.\n");
}