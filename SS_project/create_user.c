#include <stdio.h>
#include <string.h>
#include "../include/login.h"

void create_user_database() {
    FILE *file = fopen("data/users.dat", "wb");
    if (file == NULL) {
        printf("Error creating user database file.\n");
        return;
    }

    struct User users[] = {
        {"admin", "adminpass", ADMIN_ROLE, 0.0},
        {"employee", "employeepass", EMPLOYEE_ROLE, 0.0},
        {"customer", "customerpass", CUSTOMER_ROLE, 1000.0}
    };

    fwrite(users, sizeof(struct User), sizeof(users) / sizeof(users[0]), file);
    fclose(file);
    printf("User database created successfully.\n");
}

int main() {
    create_user_database();
    return 0;
}
