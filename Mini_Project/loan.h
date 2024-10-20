#ifndef LOAN_H
#define LOAN_H

#include <stdbool.h>
#include <time.h>

#define MAX_LOANS 1000

typedef enum {
    PENDING,
    APPROVED,
    REJECTED
} LoanStatus;

typedef struct {
    int id;
    int user_id;
    double amount;
    LoanStatus status;
    int assigned_employee_id;
    time_t application_date;
    time_t decision_date;
} Loan;

// Function prototypes
bool initialize_loan_system();
bool create_loan_application(int user_id, double amount);
Loan* find_loan_by_id(int loan_id);
bool update_loan_status(int loan_id, LoanStatus new_status, int employee_id);
int get_pending_loans(Loan** result);
void close_loan_system();
int assign_loan_to_employee(int loan_id, int assigned_employee_id);
int process_loan(int loan_id, int assigned_employee_id, int approve);

#endif