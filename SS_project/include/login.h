#ifndef LOGIN_H
#define LOGIN_H
#define CUSTOMER_ROLE 1
#define EMPLOYEE_ROLE 2
#define MANAGER_ROLE 3
#define ADMIN_ROLE 4
#define MAX_LOAN_AMOUNT 50000.00 // Maximum loan amount for simplicity

struct Transaction {
    char username[50];
    double amount;
    char type[20]; // "Deposit", "Withdraw", etc.
    char date[20]; // Can use a simple date format (e.g., YYYY-MM-DD)
};

struct LoanApplication {
    char username[50];
    double amount;
    char date[20];
    int approved; /// 0 = not approved, 1 = approved
};
struct Loan {
    char username[50];  // Username of the customer applying for the loan
    double amount;      // Amount of the loan
    char status[20];    // Status of the loan (e.g., "Pending", "Approved", "Rejected")
    char date[11];      // Date of application in YYYY-MM-DD format
};

struct User {
    char username[50];
    char password[50];
    int role;
    double balance; 
};
// Include this at the top of login.h
void view_account_balance(struct User *user);
int login(struct User *user);
void user_login();
void view_balance(struct User *user);
void deposit_money(struct User *user, double amount);
void withdraw_money(struct User *user, double amount);
void transfer_funds(struct User *user, double amount, const char *recipient);
void view_transaction_history(struct User *user);
void add_new_customer();
void modify_customer_details();
void process_loan_application();
void view_customer_transactions();
void apply_for_loan(struct User *user);

#endif
