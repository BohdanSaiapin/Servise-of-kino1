#ifndef SCREENING_H
#define SCREENING_H
#define MAX_HALL_LEN        30
#define MAX_DATETIME_LEN    20
#define SCREENINGS_FILE     "data/screenings.txt"

typedef struct {
int  id;
int  movieId;
char hall[MAX_HALL_LEN];
char date[MAX_DATETIME_LEN];  
char time[MAX_DATETIME_LEN];
int  totalSeats;
int  seatsAvailable;
} Screening;

int  getNextScreeningId(void);
int  screeningExists(int id);
int  loadScreeningById(int id, Screening *out);
int  addScreening(int movieId, const char *hall, const char *date, const char *time, int totalSeats);
int  removeScreening(int id);
int  removeScreeningsByMovie(int movieId);
int  decrementSeat(int screeningId);
int  incrementSeat(int screeningId);
void listScreenings(void);
void listScreeningsByMovie(int movieId);

#endif
