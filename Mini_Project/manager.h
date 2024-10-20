// File: manager.h
#ifndef MANAGER_H
#define MANAGER_H

#include "user.h"
#include "account.h"
#include "loan.h"

// Function prototypes
void activate_deactivate_account(int manager_id, int socket);
void assign_loan_application(int manager_id, int socket);
void review_customer_feedback(int manager_id, int socket);
void handle_manager_menu(int manager_id, int socket);

#endif