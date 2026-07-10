#include <stdio.h>
#include <string.h>
#include "users.h"

char currentUser[30];
void registerUser()
{
FILE *file=fopen("users.txt","a");
User user;
printf("\nРеєстрація\n");
printf("Логін: ");
scanf("%s",user.login);
printf("Пароль: ");
scanf("%s",user.password);
fprintf(file,"%s %s\n",user.login,user.password);
fclose(file);
printf("Користувача зареєстровано!\n");
}
int loginUser()
{
FILE *file=fopen("users.txt","r");
User user;
char login[30];
char password[30];
printf("\n=== Авторизація ===\n");
printf("Логін: ");
scanf("%s",login);
printf("Пароль: ");
scanf("%s",password);
while(fscanf(file,"%s %s",user.login,user.password)!=EOF)
{
if(strcmp(login,user.login)==0 && strcmp(password,user.password)==0)
{
strcpy(currentUser,login);
fclose(file);
printf("Вхід успішний!\n");
return 1;
}
}
fclose(file);
printf("Невірний логін або пароль!\n");
return 0;
}
void userMenu()
{
    int choice;

    do
    {
        printf("\n===== МЕНЮ КОРИСТУВАЧА =====\n");
        printf("1. Переглянути фільми\n");
        printf("2. Переглянути сеанси\n");
        printf("3. Купити квиток\n");
        printf("4. Мої квитки\n");
        printf("5. Скасувати бронювання\n");
        printf("0. Вийти\n");

        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
                showMovies();
                break;

            case 2:
                showSessions();
                break;

            case 3:
                buyTicket();
                break;

            case 4:
                showMyTickets();
                break;

            case 5:
                cancelTicket();
                break;
        }

    } while(choice != 0);
}
