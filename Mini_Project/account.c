#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/file.h>
#include <errno.h>
#include <pthread.h>
#include "account.h"

#define ACCOUNTS_FILE "accounts.dat"

static Account accounts[MAX_ACCOUNTS];
static int account_count = 0;
static pthread_mutex_t account_mutex = PTHREAD_MUTEX_INITIALIZER;

bool initialize_account_system() {
    pthread_mutex_lock(&account_mutex);

    int fd = open(ACCOUNTS_FILE, O_RDWR | O_CREAT, 0644);
    if (fd == -1) {
        perror("Error opening accounts file");
        pthread_mutex_unlock(&account_mutex);
        return false;
    }

    if (flock(fd, LOCK_SH) == -1) {
        perror("Error locking accounts file");
        close(fd);
        pthread_mutex_unlock(&account_mutex);
        return false;
    }

    ssize_t bytes_read = read(fd, accounts, sizeof(Account) * MAX_ACCOUNTS);
    if (bytes_read == -1) {
        perror("Error reading accounts file");
        flock(fd, LOCK_UN);
        close(fd);
        pthread_mutex_unlock(&account_mutex);
        return false;
    }

    account_count = bytes_read / sizeof(Account);

    flock(fd, LOCK_UN);
    close(fd);
    pthread_mutex_unlock(&account_mutex);
    return true;
}

Account* create_account(int user_id) {
    pthread_mutex_lock(&account_mutex);

    if (account_count >= MAX_ACCOUNTS) {
        pthread_mutex_unlock(&account_mutex);
        return NULL;
    }

    Account* new_account = &accounts[account_count++];
    new_account->id = account_count;
    new_account->user_id = user_id;
    new_account->balance = 0.0;
    new_account->is_active = 1;

    int fd = open(ACCOUNTS_FILE, O_WRONLY | O_CREAT, 0644);
    if (fd == -1) {
        perror("Error opening accounts file");
        pthread_mutex_unlock(&account_mutex);
        return NULL;
    }

    if (flock(fd, LOCK_EX) == -1) {
        perror("Error locking accounts file");
        close(fd);
        pthread_mutex_unlock(&account_mutex);
        return NULL;
    }

    if (write(fd, accounts, sizeof(Account) * account_count) == -1) {
        perror("Error writing to accounts file");
        flock(fd, LOCK_UN);
        close(fd);
        pthread_mutex_unlock(&account_mutex);
        return NULL;
    }

    flock(fd, LOCK_UN);
    close(fd);

    pthread_mutex_unlock(&account_mutex);
    return new_account;
}

Account* find_account_by_id(int account_id) {
    for (int i = 0; i < account_count; i++) {
        if (accounts[i].id == account_id) {
            return &accounts[i];
        }
    }
    return NULL;
}

Account* find_account_by_user_id(int user_id) {
    pthread_mutex_lock(&account_mutex);
    for (int i = 0; i < account_count; i++) {
        if (accounts[i].user_id == user_id) {
            pthread_mutex_unlock(&account_mutex);
            return &accounts[i];
        }
    }
    pthread_mutex_unlock(&account_mutex);
    return NULL;
}

bool update_account_balance(int account_id, double amount, TransactionType type) {
    pthread_mutex_lock(&account_mutex);

    Account* account = find_account_by_id(account_id);
    if (account == NULL) {
        pthread_mutex_unlock(&account_mutex);
        return false;
    }

    if (type == WITHDRAW && account->balance < amount) {
        pthread_mutex_unlock(&account_mutex);
        return false;
    }

    switch (type) {
        case DEPOSIT:
        case TRANSFER:
            account->balance += amount;
            break;
        case WITHDRAW:
            account->balance -= amount;
            break;
    }

    int fd = open(ACCOUNTS_FILE, O_RDWR, 0644);
    if (fd == -1) {
        perror("Error opening accounts file");
        pthread_mutex_unlock(&account_mutex);
        return false;
    }

    if (flock(fd, LOCK_EX) == -1) {
        perror("Error locking accounts file");
        close(fd);
        pthread_mutex_unlock(&account_mutex);
        return false;
    }

    off_t offset = (account->id - 1) * sizeof(Account);
    if (pwrite(fd, account, sizeof(Account), offset) == -1) {
        perror("Error writing to accounts file");
        flock(fd, LOCK_UN);
        close(fd);
        pthread_mutex_unlock(&account_mutex);
        return false;
    }

    flock(fd, LOCK_UN);
    close(fd);

    pthread_mutex_unlock(&account_mutex);
    return true;
}

int set_account_status(int account_id, int status) {
    pthread_mutex_lock(&account_mutex);
    Account* account = find_account_by_id(account_id);
    if (account == NULL) {
        pthread_mutex_unlock(&account_mutex);
        return 0;
    }

    account->is_active = status;

    int fd = open(ACCOUNTS_FILE, O_WRONLY, 0644);
    if (fd == -1) {
        perror("Error opening accounts file");
        pthread_mutex_unlock(&account_mutex);
        return 0;
    }

    if (flock(fd, LOCK_EX) == -1) {
        perror("Error locking accounts file");
        close(fd);
        pthread_mutex_unlock(&account_mutex);
        return 0;
    }

    if (pwrite(fd, account, sizeof(Account), (account->id - 1) * sizeof(Account)) == -1) {
        perror("Error writing to accounts file");
        flock(fd, LOCK_UN);
        close(fd);
        pthread_mutex_unlock(&account_mutex);
        return 0;
    }

    flock(fd, LOCK_UN);
    close(fd);

    pthread_mutex_unlock(&account_mutex);
    return 1;
}

bool transfer_between_accounts(int from_account_id, int to_account_id, double amount) {
    pthread_mutex_lock(&account_mutex);

    Account* from_account = find_account_by_id(from_account_id);
    Account* to_account = find_account_by_id(to_account_id);

    if (from_account == NULL || to_account == NULL || from_account->balance < amount) {
        fprintf(stderr, "Error: Transfer failed between accounts %d and %d (insufficient balance or account not found).\n",
                from_account_id, to_account_id);
        pthread_mutex_unlock(&account_mutex);
        return false;
    }

    from_account->balance -= amount;
    to_account->balance += amount;

    int fd = open(ACCOUNTS_FILE, O_RDWR, 0644);
    if (fd == -1) {
        perror("Error opening accounts file");
        pthread_mutex_unlock(&account_mutex);
        return false;
    }

    if (flock(fd, LOCK_EX) == -1) {
        perror("Error locking accounts file");
        close(fd);
        pthread_mutex_unlock(&account_mutex);
        return false;
    }

    // Write the updated "from" account
    off_t offset_from = (from_account->id - 1) * sizeof(Account);
    if (pwrite(fd, from_account, sizeof(Account), offset_from) == -1) {
        perror("Error writing to accounts file (from account)");
        flock(fd, LOCK_UN);
        close(fd);
        pthread_mutex_unlock(&account_mutex);
        return false;
    }

    // Write the updated "to" account
    off_t offset_to = (to_account->id - 1) * sizeof(Account);
    if (pwrite(fd, to_account, sizeof(Account), offset_to) == -1) {
        perror("Error writing to accounts file (to account)");
        flock(fd, LOCK_UN);
        close(fd);
        pthread_mutex_unlock(&account_mutex);
        return false;
    }

    // Ensure data is flushed to disk
    if (fsync(fd) == -1) {
        perror("Error syncing accounts file");
    }

    flock(fd, LOCK_UN);
    close(fd);

    pthread_mutex_unlock(&account_mutex);
    return true;
}
// bool transfer_between_accounts(int from_account_id, int to_account_id, double amount) {
//     pthread_mutex_lock(&account_mutex);

//     Account* from_account = find_account_by_id(from_account_id);
//     Account* to_account = find_account_by_id(to_account_id);

//     if (from_account == NULL || to_account == NULL || from_account->balance < amount) {
//         fprintf(stderr, "Error: Transfer failed between accounts %d and %d (insufficient balance or account not found).\n",
//                 from_account_id, to_account_id);
//         pthread_mutex_unlock(&account_mutex);
//         return false;
//     }

//     from_account->balance -= amount;
//     to_account->balance += amount;

//     int fd = open(ACCOUNTS_FILE, O_RDWR, 0644);
//     if (fd == -1) {
//         perror("Error opening accounts file");
//         pthread_mutex_unlock(&account_mutex);
//         return false;
//     }

//     if (flock(fd, LOCK_EX) == -1) {
//         perror("Error locking accounts file");
//         close(fd);
//         pthread_mutex_unlock(&account_mutex);
//         return false;
//     }

//     // Write the updated "from" account
//     off_t offset_from = (from_account->id - 1) * sizeof(Account);
//     if (pwrite(fd, from_account, sizeof(Account), offset_from) == -1) {
//         perror("Error writing to accounts file (from account)");
//         flock(fd, LOCK_UN);
//         close(fd);
//         pthread_mutex_unlock(&account_mutex);
//         return false;
//     }

//     // Write the updated "to" account
//     off_t offset_to = (to_account->id - 1) * sizeof(Account);
//     if (pwrite(fd, to_account, sizeof(Account), offset_to) == -1) {
//         perror("Error writing to accounts file (to account)");
//         flock(fd, LOCK_UN);
//         close(fd);
//         pthread_mutex_unlock(&account_mutex);
//         return false;
//     }

//     // Ensure data is flushed to disk
//     if (fsync(fd) == -1) {
//         perror("Error syncing accounts file");
//     }

//     flock(fd, LOCK_UN);
//     close(fd);

//     pthread_mutex_unlock(&account_mutex);
//     return true;
// }
void close_account_system() {
    pthread_mutex_lock(&account_mutex);

    int fd = open(ACCOUNTS_FILE, O_WRONLY | O_CREAT, 0644);
    if (fd == -1) {
        perror("Error opening accounts file for cleanup");
        pthread_mutex_unlock(&account_mutex);
        return;
    }

    if (flock(fd, LOCK_EX) == -1) {
        perror("Error locking accounts file for cleanup");
        close(fd);
        pthread_mutex_unlock(&account_mutex);
        return;
    }

    if (write(fd, accounts, sizeof(Account) * account_count) == -1) {
        perror("Error writing accounts file during cleanup");
    }

    flock(fd, LOCK_UN);
    close(fd);

    pthread_mutex_unlock(&account_mutex);
    pthread_mutex_destroy(&account_mutex);
}