// File: employee.h
#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include "user.h"
#include "account.h"
#include "loan.h"

// Function prototypes
void add_new_customer(int employee_id, int socket);
void modify_customer_details(int employee_id, int socket);
void process_loan_applications(int employee_id, int socket);
void view_customer_transactions(int employee_id, int socket);
void handle_employee_menu(int employee_id, int socket);

#endif