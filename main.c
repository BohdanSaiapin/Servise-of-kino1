#include <stdio.h>
#include "users.h"

int main()
{
int choice;
do
{
printf("\n=========================\n");
printf("     КІНОТЕАТР\n");
printf("=========================\n");
printf("1. Реєстрація\n");
printf("2. Авторизація\n");
printf("0. Вихід\n");
printf("Вибір: ");
scanf("%d",&choice);
switch(choice)
{
case 1:
registerUser();
break;
case 2:
if(loginUser())
{
userMenu();
}
break;
case 0:
printf("До побачення!\n");
break;
default:
printf("Невірний вибір!\n");
}
}
while(choice!=0);
return 0;
}
