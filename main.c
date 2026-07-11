#include <stdio.h>
#include <string.h>
#include "auth.h"
#include "admin.h"
#include "movie.h"
#include "screening.h"
#include "ticket.h"

static void readLine(char *buf, int size){
    if (fgets(buf, size, stdin) == NULL){
        buf[0] = '\0';
        return;
    }
    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n'){
        buf[len - 1] = '\0';
    }
}
static int readInt(const char *prompt){
    char buf[32];
    int value;
    while(1){
        printf("%s", prompt);
        readLine(buf, sizeof(buf));
        if (sscanf(buf, "%d", &value) == 1){
            return value;
        }
        printf("Please enter a valid number.\n");
    }
}
static void handleRegister(void){
    char username[MAX_USERNAME_LEN];
    char password[MAX_USERNAME_LEN];
    printf("\n-- Register --\n");
    printf("Choose a username: ");
    readLine(username, sizeof(username));
    printf("Choose a password: ");
    readLine(password, sizeof(password));
    if(username[0] == '\0' || password[0] == '\0'){
        printf("Username and password cannot be empty.\n");
        return;
    }
    int id = registerUser(username, password);
    if(id > 0){
        printf("Account created successfully. You can now log in.\n");
    } else{
        printf("That username is already taken. Please try again.\n");
    }
}
static void customerMenu(const User *user){
    int choice;
    do{
        printf("\n===== Customer Menu (%s) =====\n", user->username);
        printf("1. View All Movies\n");
        printf("2. View All Screenings\n");
        printf("3. View Screenings for a Movie\n");
        printf("4. Book a Ticket\n");
        printf("5. View My Tickets\n");
        printf("0. Log Out\n");
        choice = readInt("Choose an option: ");
        switch(choice){
            case 1:
                listMovies();
                break;
            case 2:
                listScreenings();
                break;
            case 3:{
                int movieId = readInt("Enter movie ID: ");
                listScreeningsByMovie(movieId);
                break;
            }
            case 4:{
                int screeningId = readInt("Enter screening ID: ");
                int ticketId = bookTicket(screeningId, user->username);
                if(ticketId > 0){
                    printf("Ticket booked! Your ticket ID is %d.\n", ticketId);
                } else{
                    printf("Booking failed (screening not found or no seats left).\n");
                }
                break;
            }
            case 5:
                listMyTickets(user->username);
                break;
            case 0:
                printf("Logging out...\n");
                break;
            default:
                printf("Invalid option, try again.\n");
        }
    } while (choice != 0);
}
static void handleLogin(void){
    char username[MAX_USERNAME_LEN];
    char password[MAX_USERNAME_LEN];
    printf("\n-- Login --\n");
    printf("Username: ");
    readLine(username, sizeof(username));
    printf("Password: ");
    readLine(password, sizeof(password));
    User loggedInUser;
    if(!loginUser(username, password, &loggedInUser)){
        printf("Invalid username or password.\n");
        return;
    }
    printf("Welcome, %s!\n", loggedInUser.username);
    if(loggedInUser.role == ROLE_ADMIN){
        adminMenu();
    } else{
        customerMenu(&loggedInUser);
    }
}
int main(void){
    seedAdminAccount();
    int choice;
    do{
        printf("\n===== Movie Service =====\n");
        printf("1. Register\n");
        printf("2. Login\n");
        printf("3. View All Movies\n");
        printf("4. View All Screenings\n");
        printf("0. Exit\n");
        choice = readInt("Choose an option: ");
        switch (choice) {
            case 1: handleRegister(); break;
            case 2: handleLogin(); break;
            case 3: listMovies(); break;
            case 4: listScreenings(); break;
            case 0: printf("Goodbye!\n"); break;
            default: printf("Invalid option, try again.\n");
        }
    } while (choice != 0);

    return 0;
}
