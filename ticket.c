#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ticket.h"
#include "screening.h"

static int parseTicketLine(const char *line, Ticket *t) {
char buf[256];
strncpy(buf, line, sizeof(buf) - 1);
buf[sizeof(buf) - 1] = '\0';
char *idTok        = strtok(buf, "|");
char *screeningTok  = strtok(NULL, "|");
char *nameTok         = strtok(NULL, "|\n");
if (!idTok || !screeningTok || !nameTok) return 0;
t->id = atoi(idTok);
t->screeningId = atoi(screeningTok);
strncpy(t->customerName, nameTok, MAX_NAME_LEN - 1);
t->customerName[MAX_NAME_LEN - 1] = '\0';
return 1;
}
int getNextTicketId(void) {
FILE *fp = fopen(TICKETS_FILE, "r");
if (!fp) return 1;
char line[256];
int maxId = 0;
Ticket t;
while (fgets(line, sizeof(line), fp)) {
if (parseTicketLine(line, &t) && t.id > maxId) {
maxId = t.id;
}
}
fclose(fp);
return maxId + 1;
}

int bookTicket(int screeningId, const char *customerName) {
if (!decrementSeat(screeningId)) {
return -1;
}
FILE *fp = fopen(TICKETS_FILE, "a");
if (!fp) {
perror("bookTicket: could not open tickets file");
incrementSeat(screeningId); /* roll back the seat we just took */
return -1;
}
int newId = getNextTicketId();
fprintf(fp, "%d|%d|%s\n", newId, screeningId, customerName);
fclose(fp);
return newId;
}
int cancelTicket(int ticketId) {
FILE *fp = fopen(TICKETS_FILE, "r");
if (!fp) return 0;
FILE *tmp = fopen("data/tickets.tmp", "w");
if (!tmp) {
fclose(fp);
return 0;
}
char line[256];
Ticket t;
int cancelledScreening = -1;
while (fgets(line, sizeof(line), fp)) {
if (parseTicketLine(line, &t) && t.id == ticketId) {
cancelledScreening = t.screeningId;
continue;
}
fputs(line, tmp);
    }
fclose(fp);
fclose(tmp);
remove(TICKETS_FILE);
rename("data/tickets.tmp", TICKETS_FILE);
if (cancelledScreening != -1) {
incrementSeat(cancelledScreening);
return 1;
}
return 0;
}
void listTickets(void) {
FILE *fp = fopen(TICKETS_FILE, "r");
if (!fp) {
printf("No tickets found.\n");
return;
}
char line[256];
Ticket t;
int any = 0;
printf("\n%-5s %-12s %-20s\n", "ID", "ScreeningID", "Customer");
printf("--------------------------------------------\n");
while (fgets(line, sizeof(line), fp)) {
    if (parseTicketLine(line, &t)) {
printf("%-5d %-12d %-20s\n", t.id, t.screeningId, t.customerName);
any = 1;
}
}
fclose(fp);
if (!any) printf("No tickets found.\n");
}
void listTicketsByScreening(int screeningId) {
FILE *fp = fopen(TICKETS_FILE, "r");
if (!fp) {
printf("No tickets found.\n");
return;
}
char line[256];
Ticket t;
int any = 0;
printf("\n%-5s %-20s\n", "ID", "Customer");
printf("------------------------------\n");
while (fgets(line, sizeof(line), fp)) {
if (parseTicketLine(line, &t) && t.screeningId == screeningId) {
printf("%-5d %-20s\n", t.id, t.customerName);
any = 1;
}
}
fclose(fp);
if (!any) printf("No tickets found for this screening.\n");
}
void listMyTickets(const char *customerName) {
FILE *fp = fopen(TICKETS_FILE, "r");
if (!fp) {
printf("You have no tickets yet.\n");
return;
}
char line[256];
Ticket t;
int any = 0;
printf("\n%-5s %-12s\n", "ID", "ScreeningID");
printf("------------------------\n");
while (fgets(line, sizeof(line), fp)) {
if (parseTicketLine(line, &t) && strcmp(t.customerName, customerName) == 0) {
printf("%-5d %-12d\n", t.id, t.screeningId);
any = 1;
}
}
fclose(fp);
if (!any) printf("You have no tickets yet.\n");
}
