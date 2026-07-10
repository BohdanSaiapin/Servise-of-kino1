#ifndef USERS_H
#define USERS_H

typedef struct
{
char login[30];
char password[30];
}User;
void registerUser();
int loginUser();
void userMenu();
#endif
