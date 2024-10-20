#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/file.h>
#include <errno.h>
#include <pthread.h>
#include "transaction.h"

#define TRANSACTIONS_FILE "transactions.dat"

static Transaction transactions[MAX_TRANSACTIONS];
static int transaction_count = 0;
static pthread_mutex_t transaction_mutex = PTHREAD_MUTEX_INITIALIZER;

bool initialize_transaction_system() {
    pthread_mutex_lock(&transaction_mutex);

    int fd = open(TRANSACTIONS_FILE, O_RDWR | O_CREAT, 0644);
    if (fd == -1) {
        perror("Error opening transactions file");
        pthread_mutex_unlock(&transaction_mutex);
        return false;
    }

    if (flock(fd, LOCK_EX) == -1) {
        perror("Error locking transactions file");
        close(fd);
        pthread_mutex_unlock(&transaction_mutex);
        return false;
    }

    ssize_t bytes_read = read(fd, transactions, sizeof(Transaction) * MAX_TRANSACTIONS);
    if (bytes_read == -1) {
        perror("Error reading transactions file");
        flock(fd, LOCK_UN);
        close(fd);
        pthread_mutex_unlock(&transaction_mutex);
        return false;
    }

    transaction_count = bytes_read / sizeof(Transaction);

    flock(fd, LOCK_UN);
    close(fd);
    pthread_mutex_unlock(& transaction_mutex);
    return true;
}

// Helper function to save all transactions to disk
static bool save_transactions() {
    int fd = open(TRANSACTIONS_FILE, O_WRONLY | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Error opening transactions file for writing");
        return false;
    }

if (flock(fd, LOCK_EX) == -1) {
        perror("Error locking transactions file");
        close(fd);
        return false;
    }

    ssize_t bytes_written = write(fd, transactions, sizeof(Transaction) * transaction_count);
    if (bytes_written == -1) {
        perror("Error writing to transactions file");
        flock(fd, LOCK_UN);
        close(fd);
        return false;
    }

    if (fsync(fd) == -1) {
        perror("Error syncing transactions file");
    }

    flock(fd, LOCK_UN);
    close(fd);
    return true;
}

bool record_transaction(int account_id, TransactionType type, double amount) {
    pthread_mutex_lock(&transaction_mutex);

    if (transaction_count >= MAX_TRANSACTIONS) {
        pthread_mutex_unlock(&transaction_mutex);
        return false;
    }

    Transaction new_transaction = {
        .id = transaction_count + 1,
        .account_id = account_id,
        .type = type,
        .amount = amount,
        .timestamp = time(NULL)
    };

    transactions[transaction_count++] = new_transaction;

    bool result = save_transactions();
    pthread_mutex_unlock(&transaction_mutex);
    return result;
}

int get_transactions_by_account_id(int account_id, Transaction** result) {
    pthread_mutex_lock(&transaction_mutex);

    int count = 0;
    for (int i = 0; i < transaction_count; i++) {
        if (transactions[i].account_id == account_id) {
            count++;
        }
    }

    if (count == 0) {
        pthread_mutex_unlock(&transaction_mutex);
        *result = NULL;
        return 0;
    }

    *result = malloc(sizeof(Transaction) * count);
    if (*result == NULL) {
        pthread_mutex_unlock(&transaction_mutex);
        return -1;
    }

    int index = 0;
    for (int i = 0; i < transaction_count; i++) {
        if (transactions[i].account_id == account_id) {
            (*result)[index++] = transactions[i];
        }
    }

    pthread_mutex_unlock(&transaction_mutex);
    return count;
}

void close_transaction_system() {
    pthread_mutex_lock(&transaction_mutex);
    save_transactions();
    pthread_mutex_unlock(&transaction_mutex);
    pthread_mutex_destroy(&transaction_mutex);
}