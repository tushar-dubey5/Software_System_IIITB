#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/file.h>
#include <errno.h>
#include <pthread.h>
#include "loan.h"
#include "user.h"

#define LOANS_FILE "loans.dat"

static Loan loans[MAX_LOANS];
static int loan_count = 0;
static pthread_mutex_t loan_mutex = PTHREAD_MUTEX_INITIALIZER;

bool initialize_loan_system() {
    pthread_mutex_lock(&loan_mutex);

    int fd = open(LOANS_FILE, O_RDWR | O_CREAT, 0644);
    if (fd == -1) {
        perror("Error opening loans file");
        pthread_mutex_unlock(&loan_mutex);
        return false;
    }

    if (flock(fd, LOCK_SH) == -1) {
        perror("Error locking loans file");
        close(fd);
        pthread_mutex_unlock(&loan_mutex);
        return false;
    }

    ssize_t bytes_read = read(fd, loans, sizeof(Loan) * MAX_LOANS);
    if (bytes_read == -1) {
        perror("Error reading loans file");
        flock(fd, LOCK_UN);
        close(fd);
        pthread_mutex_unlock(&loan_mutex);
        return false;
    }

    loan_count = bytes_read / sizeof(Loan);

    flock(fd, LOCK_UN);
    close(fd);
    pthread_mutex_unlock(&loan_mutex);
    return true;
}

static bool save_loans() {
    int fd = open(LOANS_FILE, O_WRONLY | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Error opening loans file for writing");
        return false;
    }

    if (flock(fd, LOCK_EX) == -1) {
        perror("Error locking loans file");
        close(fd);
        return false;
    }

    ssize_t bytes_written = write(fd, loans, sizeof(Loan) * loan_count);
    if (bytes_written == -1) {
        perror("Error writing to loans file");
        flock(fd, LOCK_UN);
        close(fd);
        return false;
    }

    // Ensure data is flushed to disk
    if (fsync(fd) == -1) {
        perror("Error syncing loans file");
    }

    flock(fd, LOCK_UN);
    close(fd);
    return true;
}

bool create_loan_application(int user_id, double amount) {
    pthread_mutex_lock(&loan_mutex);

    if (loan_count >= MAX_LOANS) {
        pthread_mutex_unlock(&loan_mutex);
        return false;
    }

    Loan new_loan = {
        .id = loan_count + 1,
        .user_id = user_id,
        .amount = amount,
        .status = PENDING,
        .assigned_employee_id = -1,
        .application_date = time(NULL),
        .decision_date = 0
    };

    loans[loan_count++] = new_loan;

    bool result = save_loans();
    pthread_mutex_unlock(&loan_mutex);
    return result;
}

Loan* find_loan_by_id(int loan_id) {
    pthread_mutex_lock(&loan_mutex);
    for (int i = 0; i < loan_count; i++) {
        if (loans[i].id == loan_id) {
            pthread_mutex_unlock(&loan_mutex);
            return &loans[i];
        }
    }
    pthread_mutex_unlock(&loan_mutex);
    return NULL;
}

bool update_loan_status(int loan_id, LoanStatus new_status, int assigned_employee_id) {
    pthread_mutex_lock(&loan_mutex);
    Loan* loan = find_loan_by_id(loan_id);
    if (loan == NULL) {
        pthread_mutex_unlock(&loan_mutex);
        return false;
    }

    loan->status = new_status;
    loan->assigned_employee_id = assigned_employee_id;
    loan->decision_date = time(NULL);

    bool result = save_loans();
    pthread_mutex_unlock(&loan_mutex);
    return result;
}

int get_pending_loans(Loan** result) {
    pthread_mutex_lock(&loan_mutex);

    int count = 0;
    for (int i = 0; i < loan_count; i++) {
        if (loans[i].status == PENDING) {
            count++;
        }
    }

    if (count == 0) {
        pthread_mutex_unlock(&loan_mutex);
        *result = NULL;
        return 0;
    }

    *result = malloc(sizeof(Loan) * count);
    if (*result == NULL) {
        pthread_mutex_unlock(&loan_mutex);
        return -1;
    }

    int index = 0;
    for (int i = 0; i < loan_count; i++) {
        if (loans[i].status == PENDING) {
            (*result)[index++] = loans[i];
        }
    }

    pthread_mutex_unlock(&loan_mutex);
    return count;
}

int assign_loan_to_employee(int loan_id, int assigned_employee_id) {
    // pthread_mutex_lock(&loan_mutex);

    Loan* loan = find_loan_by_id(loan_id);
    if (loan == NULL || loan->status != PENDING) {
        // pthread_mutex_unlock(&loan_mutex);
        return 0;
    }

    // Check if the loan is already assigned to an employee
    if (loan->assigned_employee_id != -1) {
        // pthread_mutex_unlock(&loan_mutex);
        return 0; // Loan already assigned
    }

    User* employee = find_user_by_id(assigned_employee_id);
    if (employee == NULL || employee->role != EMPLOYEE) {
        // pthread_mutex_unlock(&loan_mutex);
        return 0; // Invalid employee
    }

    loan->assigned_employee_id = assigned_employee_id;
    bool result = save_loans();
    // pthread_mutex_unlock(&loan_mutex);
    return result ? 1 : 0;
}

int process_loan(int loan_id, int assigned_employee_id, int approve) {
    // pthread_mutex_lock(&loan_mutex);

    Loan* loan = find_loan_by_id(loan_id);
    if (loan == NULL || loan->status != PENDING || loan->assigned_employee_id != assigned_employee_id) {
        // pthread_mutex_unlock(&loan_mutex);
        return 0;
    }

    loan->status = approve ? APPROVED : REJECTED;
    loan->decision_date = time(NULL);

    bool result = save_loans();
    // pthread_mutex_unlock(&loan_mutex);
    return result ? 1 : 0;
}

void close_loan_system() {
    pthread_mutex_destroy(&loan_mutex);
}