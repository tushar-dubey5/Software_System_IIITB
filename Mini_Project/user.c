#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/file.h>
#include <openssl/evp.h>
#include "user.h"
#include "account.h"

#define USERS_FILE "users.dat"
#define LOGGED_IN_USERS_FILE "logged_in_users.dat"
#define MAX_USERS 1000
#define MAX_HASHED_PASSWORD_LENGTH 65

static User users[MAX_USERS];
static int user_count = 0;

static void hash_password(const char* password, char* hashed_password) {
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len;
    
    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    const EVP_MD *md = EVP_sha256();
    
    EVP_DigestInit_ex(mdctx, md, NULL);
    EVP_DigestUpdate(mdctx, password, strlen(password));
    EVP_DigestFinal_ex(mdctx, hash, &hash_len);
    EVP_MD_CTX_free(mdctx);

    for (unsigned int i = 0; i < hash_len; i++) {
        sprintf(&hashed_password[i * 2], "%02x", hash[i]);
    }
    hashed_password[hash_len * 2] = '\0';
}

void save_users()
{
    FILE *file = fopen(USERS_FILE, "wb");
    if (file == NULL)
    {
        perror("Error opening users file for writing");
        return;
    }

    for (int i = 0; i < user_count; i++)
    {
        fprintf(file, "%s,%s,%d\n", users[i].username, users[i].password, users[i].role);
    }

    fclose(file);
    printf("Saved %d users to file.\n", user_count);
}

void create_initial_users()
{
    if (user_count == 0)
    {
        create_user("admin", "admin123", ADMIN);
        create_user("manager", "manager123", MANAGER);
        create_user("employee", "employee123", EMPLOYEE);
        create_user("customer", "customer123", CUSTOMER);
        int cusid = find_user_by_username("customer")->id;
        create_account(cusid);
        printf("Initial users created.\n");
        save_users();
    }
}

bool initialize_user_system() {
    FILE *file = fopen(USERS_FILE, "r");
    if (file == NULL) {
        printf("Users file not found. Creating initial users.\n");
        create_initial_users();
        return true;
    }

    char line[256];
    user_count = 0;
    while (fgets(line, sizeof(line), file) && user_count < MAX_USERS) {
        char username[MAX_USERNAME_LENGTH];
        char password[MAX_PASSWORD_LENGTH];
        int role;
        // int logged_in = 0;

        if (sscanf(line, "%[^,],%[^,],%d", username, password, &role) == 3) {
            strncpy(users[user_count].username, username, MAX_USERNAME_LENGTH - 1);
            users[user_count].username[MAX_USERNAME_LENGTH - 1] = '\0';
            strncpy(users[user_count].password, password, MAX_PASSWORD_LENGTH - 1);
            users[user_count].password[MAX_PASSWORD_LENGTH - 1] = '\0';
            users[user_count].role = (UserRole)role;
            // users[user_count].logged_in = (bool)logged_in;
            users[user_count].id = user_count + 1;
            user_count++;
            printf("Loaded user: %s (role: %d)\n", username, role);
        } else {
            printf("Failed to parse line: %s", line);
        }
    }

    fclose(file);

    FILE *logged_in_file = fopen(LOGGED_IN_USERS_FILE, "a");
    if (logged_in_file == NULL) {
        perror("Error creating logged_in_users file");
        return false;
    }
    fclose(logged_in_file);

    return true;
}

User *create_user(const char *username, const char *password, UserRole role) {
    if (user_count >= MAX_USERS) {
        fprintf(stderr, "Maximum number of users reached\n");
        return NULL;
    }

    if (username == NULL || password == NULL || strlen(username) == 0 || strlen(password) == 0) {
        fprintf(stderr, "Invalid username or password\n");
        return NULL;
    }

    if (find_user_by_username(username) != NULL) {
        fprintf(stderr, "Username already exists\n");
        return NULL;
    }

    User *new_user = &users[user_count];
    new_user->id = user_count + 1;
    strncpy(new_user->username, username, MAX_USERNAME_LENGTH - 1);
    new_user->username[MAX_USERNAME_LENGTH - 1] = '\0';
    hash_password(password, new_user->password);
    new_user->role = role;
    // new_user->logged_in = false;

    user_count++;

    save_users();
    return new_user;
}

User* find_user_by_username(const char* username) {
    printf("Searching for user: '%s' (length: %zu)\n", username, strlen(username));
    printf("Total users in system: %d\n", user_count);
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, username) == 0) {
            printf("User found: '%s'\n", username);
            return &users[i];
        }
    }
    printf("User not found: '%s'\n", username);
    return NULL;
}

bool authenticate_user(const char* username, const char* password) {
    User* user = find_user_by_username(username);
    if (user == NULL) {
        printf("User not found: %s\n", username);
        return false;
    }

    char hashed_password[MAX_PASSWORD_LENGTH];
    hash_password(password, hashed_password);

    printf("Input username: %s\n", username);
    printf("Stored hashed password: %s\n", user->password);
    printf("Input hashed password: %s\n", hashed_password);

    bool result = strcmp(user->password, hashed_password) == 0;
    printf("Authentication result: %s\n", result ? "Success" : "Failure");

    return result;
}

User* find_user_by_id(int user_id) {
    for (int i = 0; i < user_count; i++) {
        if (users[i].id == user_id) {
            return &users[i];
        }
    }
    return NULL;
}

int update_user(User* user) {
    if (user == NULL) {
        return 0;
    }
    
    FILE *file = fopen(USERS_FILE, "w");
    if(file == NULL){
        perror("Error opening users file");
        return 0;
    }

    if(flock(fileno(file),LOCK_EX) == -1){
        perror("Error locking users file");
        fclose(file);
        return 0;
    }

    for(int i = 0;i<user_count;i++){
        fprintf(file,"%s,%s,%d\n", users[i].username, users[i].password,users[i].role);
    }

    if(flock(fileno(file),LOCK_UN) == -1){
        perror("Error unlocking users file");
    }

    fclose(file);
    return 1;
}


// bool update_user_password(int user_id, const char* new_password) {
//     User* user = find_user_by_id(user_id);
//     if (user == NULL) {
//         return false;
//     }

//     hash_password(new_password, user->password);

//     int fd = open(USERS_FILE, O_RDWR, 0644);
//     if (fd == -1) {
//         perror("Error opening users file");
//         return false;
//     }

//     if (flock(fd, LOCK_EX) == -1) {
//         perror("Error locking users file");
//         close(fd);
//         return false;
//     }

//     if (pwrite(fd, users, sizeof(User) * user_count, 0) == -1){
//         perror("Error writing to users file");
//         flock(fd, LOCK_UN);
//         close(fd);
//         return false;
//     }

//     if (fsync(fd) == -1) {
//         perror("Error syncing users file");
//     }

//     flock(fd, LOCK_UN);
//     close(fd);
//     return true;
// }

bool update_user_password(int user_id, const char* new_password) {
    User* user = find_user_by_id(user_id);
    if (user == NULL) {
        return false;
    }

    hash_password(new_password, user->password);

    int result = update_user(user);

    return result == 1;
}

int get_all_users(User **users_ptr){
    *users_ptr = malloc(sizeof(User)*user_count);
    if(*users_ptr == NULL){
        perror("Memory Allocation failed for users list");
        return 0;
    }

    memcpy(*users_ptr,users,sizeof(User)* user_count);
    return user_count;
}

bool add_logged_in_user(const char *username) {
    FILE *file = fopen(LOGGED_IN_USERS_FILE, "a");
    if (file == NULL) {
        perror("Error opening logged_in_users file for writing");
        return false;
    }

    if (flock(fileno(file), LOCK_EX) == -1) {
        perror("Error locking logged_in_users file");
        fclose(file);
        return false;
    }

    fprintf(file, "%s\n", username);

    if (flock(fileno(file), LOCK_UN) == -1) {
        perror("Error unlocking logged_in_users file");
    }

    fclose(file);
    return true;
}

// Check if a user is logged in
bool is_user_logged_in(const char *username) {
    FILE *file = fopen(LOGGED_IN_USERS_FILE, "r");
    if (file == NULL) {
        return false;
    }

    if (flock(fileno(file), LOCK_SH) == -1) {
        perror("Error locking logged_in_users file");
        fclose(file);
        return false;
    }

    char line[MAX_USERNAME_LENGTH];
    bool found = false;
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';
        if (strcmp(line, username) == 0) {
            found = true;
            break;
        }
    }

    if (flock(fileno(file), LOCK_UN) == -1) {
        perror("Error unlocking logged_in_users file");
    }

    fclose(file);
    return found;
}

// Remove a user from the logged-in file
bool remove_logged_in_user(const char *username) {
    FILE *file = fopen(LOGGED_IN_USERS_FILE, "r");
    if (file == NULL) {
        perror("Error opening logged_in_users file for reading");
        return false;
    }

    FILE *temp_file = fopen("temp_logged_in_users.dat", "w");
    if (temp_file == NULL) {
        perror("Error opening temporary file for writing");
        fclose(file);
        return false;
    }

    if (flock(fileno(file), LOCK_EX) == -1) {
        perror("Error locking logged_in_users file");
        fclose(file);
        fclose(temp_file);
        return false;
    }

    char line[MAX_USERNAME_LENGTH];
    bool found = false;
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';

        if (strcmp(line, username) != 0) {
            fprintf(temp_file, "%s\n", line);
        } else {
            found = true;
        }
    }

    if (flock(fileno(file), LOCK_UN) == -1) {
        perror("Error unlocking logged_in_users file");
    }

    fclose(file);
    fclose(temp_file);

    if (found) {
        if (rename("temp_logged_in_users.dat", LOGGED_IN_USERS_FILE) != 0) {
            perror("Error renaming temporary file");
            return false;
        }
    } else {
        remove("temp_logged_in_users.dat");
    }

    return found;
}
void close_user_system() {
    // pthread_mutex_destroy(&user_mutex);
}