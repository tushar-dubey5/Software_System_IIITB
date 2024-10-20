#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "user.h"
#include "feedback.h"

void view_account_balance(int user_id, int socket);
void deposit_money(int user_id, int socket);
void withdraw_money(int user_id, int socket);
void transfer_funds(int user_id, int socket);
void apply_for_loan(int user_id, int socket);
void view_transaction_history(int user_id, int socket);
void change_password(int user_id, int socket);
void handle_customer_menu(int user_id, int socket);

#endif