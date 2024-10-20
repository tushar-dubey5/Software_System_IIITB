#ifndef ADMIN_H
#define ADMIN_H

#include "user.h"

void handle_admin_menu(int user_id, int socket_desc);
int add_new_bank_employee(int socket_desc);
int manage_user_roles(int socket_desc);
void modify_user_details(int socket_desc);
void change_admin_password(int socket_desc, int user_id) ;

#endif 