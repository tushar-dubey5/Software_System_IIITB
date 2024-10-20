#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <stdbool.h>
#include <time.h>
#include "account.h"

#define MAX_TRANSACTIONS 10000

typedef struct {
    int id;
    int account_id;
    TransactionType type;
    double amount;
    time_t timestamp;
} Transaction;


bool initialize_transaction_system();
bool record_transaction(int account_id, TransactionType type, double amount);
int get_transactions_by_account_id(int account_id, Transaction** transactions);
void close_transaction_system();

#endif