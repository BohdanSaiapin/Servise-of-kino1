#ifndef TICKET_H
#define TICKET_H
#define MAX_NAME_LEN    50
#define TICKETS_FILE    "data/tickets.txt"

typedef struct {
int  id;
int  screeningId;
char customerName[MAX_NAME_LEN];
} Ticket;
int  getNextTicketId(void);
int  bookTicket(int screeningId, const char *customerName);
int  cancelTicket(int ticketId);
void listTickets(void);
void listTicketsByScreening(int screeningId);
void listMyTickets(const char *customerName);

#endif 
