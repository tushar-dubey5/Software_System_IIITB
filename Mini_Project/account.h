#ifndef ACCOUNT_H
#define ACCOUNT_H
#include <stdbool.h>

#define MAX_ACCOUNTS 1000

typedef struct {
    int id;
    int user_id;
    double balance;
    int is_active;
} Account;

typedef enum {
    DEPOSIT,
    WITHDRAW,
    TRANSFER
} TransactionType;

bool initialize_account_system();
Account* find_account_by_user_id(int user_id);
Account* find_account_by_id(int account_id);
bool update_account_balance(int account_id, double amount, TransactionType type);
bool transfer_between_accounts(int from_account_id, int to_account_id, double amount);
void close_account_system();
int set_account_status(int account_id, int status);
Account* create_account(int user_id); 

#endif