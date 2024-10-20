#ifndef USER_H
#define USER_H
#include<stdbool.h>

#define MAX_USERNAME_LENGTH 50
#define MAX_PASSWORD_LENGTH 65

typedef enum {
    CUSTOMER = 0,
    EMPLOYEE = 1,
    MANAGER = 2,
    ADMIN = 3
} UserRole;

typedef struct {
    int id;
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    UserRole role;
} User; 

bool initialize_user_system();
User* create_user(const char* username, const char* password, UserRole role);
User* find_user_by_username(const char* username);
bool authenticate_user(const char* username, const char* password);
void close_user_system();
int update_user(User* user);
User* find_user_by_id(int user_id);
bool update_user_password(int user_id, const char* new_password);
int get_all_users(User **users);
bool add_logged_in_user(const char *username);
bool remove_logged_in_user(const char *username);
bool is_user_logged_in(const char *username);

#endif