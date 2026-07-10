#ifndef AUTH_H
#define AUTH_H

#define MAX_USERNAME_LEN   50
#define USERS_FILE         "data/users.txt"

#define DEFAULT_ADMIN_USERNAME  "admin"
#define DEFAULT_ADMIN_PASSWORD  "admin123"

typedef enum{
    ROLE_ADMIN = 0,
    ROLE_CUSTOMER = 1
} Role;

typedef struct{
    int  id;
    char username[MAX_USERNAME_LEN];
    unsigned long passwordHash;
    Role role;
} User;
void seedAdminAccount(void);

int  usernameExists(const char *username);

int  registerUser(const char *username, const char *password);

int  loginUser(const char *username, const char *password, User *out);

#endif
