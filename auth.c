#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "auth.h"

static unsigned long hashPassword(const char *password){
    unsigned long hash = 5381;
    int c;
    while ((c = *password++)){
        hash = ((hash << 5) + hash) + (unsigned long)c;
    }
    return hash;
}
static int parseUserLine(const char *line, User *u){
    char buf[256];
    strncpy(buf, line, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';
    char *idTok  = strtok(buf, "|");
    char *usernameTok = strtok(NULL, "|");
    char *hashTok = strtok(NULL, "|");
    char *roleTok = strtok(NULL, "|\n");
    if(!idTok || !usernameTok || !hashTok || !roleTok) return 0; u->id = atoi(idTok); strncpy(u->username, usernameTok, MAX_USERNAME_LEN - 1); u->username[MAX_USERNAME_LEN - 1] = '\0'; u->passwordHash = strtoul(hashTok, NULL, 10); u->role = (Role)atoi(roleTok);
    return 1;
}
static int getNextUserId(void){
    FILE *fp = fopen(USERS_FILE, "r");
    if (!fp) return 1;
    char line[256];
    int maxId = 0;
    User u;
    while(fgets(line, sizeof(line), fp)){
        if(parseUserLine(line, &u) && u.id > maxId){
            maxId = u.id;
        }
    }
    fclose(fp);
    return maxId + 1;
}
static int loadUserByUsername(const char *username, User *out){
    FILE *fp = fopen(USERS_FILE, "r");
    if(!fp) return 0;
    char line[256];
    User u;
    int found = 0;
    while(fgets(line, sizeof(line), fp)){
        if(parseUserLine(line, &u) && strcmp(u.username, username) == 0){
            *out = u;
            found = 1;
            break;
        }
    }
    fclose(fp);
    return found;
}
static int addUserRaw(const char *username, const char *password, Role role){
    FILE *fp = fopen(USERS_FILE, "a");
    if (!fp){
        perror("addUserRaw: could not open users file");
        return -1;
    }
    int newId = getNextUserId();
    fprintf(fp, "%d|%s|%lu|%d\n", newId, username, hashPassword(password), (int)role);
    fclose(fp);
    return newId;
}
void seedAdminAccount(void){
    if(!usernameExists(DEFAULT_ADMIN_USERNAME)){
        addUserRaw(DEFAULT_ADMIN_USERNAME, DEFAULT_ADMIN_PASSWORD, ROLE_ADMIN);
    }
}
int usernameExists(const char *username){
    User u;
    return loadUserByUsername(username, &u);
}
int registerUser(const char *username, const char *password){
    if(usernameExists(username)){
        return -1;
    }
    return addUserRaw(username, password, ROLE_CUSTOMER);
}
int loginUser(const char *username, const char *password, User *out){
    User u;
    if(!loadUserByUsername(username, &u)){
        return 0;
    }
    if(u.passwordHash != hashPassword(password)){
        return 0;
    }
    *out = u;
    return 1;
}
