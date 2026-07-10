#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "screening.h"
#include "movie.h"

static int parseScreeningLine(const char *line, Screening *s) {
char buf[256];
strncpy(buf, line, sizeof(buf) - 1);
buf[sizeof(buf) - 1] = '\0';
char *idTok        = strtok(buf, "|");
char *movieIdTok    = strtok(NULL, "|");
char *hallTok        = strtok(NULL, "|");
char *dateTok         = strtok(NULL, "|");
char *timeTok          = strtok(NULL, "|");
char *totalTok           = strtok(NULL, "|");
char *availTok             = strtok(NULL, "|\n");
if (!idTok || !movieIdTok || !hallTok || !dateTok || !timeTok || !totalTok || !availTok)
return 0;
s->id = atoi(idTok);
s->movieId = atoi(movieIdTok);
strncpy(s->hall, hallTok, MAX_HALL_LEN - 1);
s->hall[MAX_HALL_LEN - 1] = '\0';
strncpy(s->date, dateTok, MAX_DATETIME_LEN - 1);
s->date[MAX_DATETIME_LEN - 1] = '\0';
strncpy(s->time, timeTok, MAX_DATETIME_LEN - 1);
s->time[MAX_DATETIME_LEN - 1] = '\0';
s->totalSeats = atoi(totalTok);
s->seatsAvailable = atoi(availTok);
return 1;
}
static int writeAllScreenings(Screening *arr, int count) {
FILE *fp = fopen(SCREENINGS_FILE, "w");
if (!fp) return 0;
for (int i = 0; i < count; i++) {
fprintf(fp, "%d|%d|%s|%s|%s|%d|%d\n",
arr[i].id, arr[i].movieId, arr[i].hall, arr[i].date,
arr[i].time, arr[i].totalSeats, arr[i].seatsAvailable);
}
fclose(fp);
return 1;
}
static Screening *loadAllScreenings(int *count) {
FILE *fp = fopen(SCREENINGS_FILE, "r");
*count = 0;
if (!fp) return NULL;
int capacity = 16;
Screening *arr = malloc(capacity * sizeof(Screening));
char line[256];
Screening s;
while (fgets(line, sizeof(line), fp)) {
if (!parseScreeningLine(line, &s)) continue;
if (*count >= capacity) {
capacity *= 2;
arr = realloc(arr, capacity * sizeof(Screening));
}
arr[*count] = s;
(*count)++;
}
fclose(fp);
return arr;
}
int getNextScreeningId(void) {
int count;
Screening *arr = loadAllScreenings(&count);
int maxId = 0;
for (int i = 0; i < count; i++) {
if (arr[i].id > maxId) maxId = arr[i].id;
}
free(arr);
return maxId + 1;
}
int screeningExists(int id) {
Screening s;
return loadScreeningById(id, &s);
}
int loadScreeningById(int id, Screening *out) {
int count;
Screening *arr = loadAllScreenings(&count);
int found = 0;
for (int i = 0; i < count; i++) {
if (arr[i].id == id) {
*out = arr[i];
found = 1;
break;
}
}
free(arr);
return found;
}
int addScreening(int movieId, const char *hall, const char *date, const char *time, int totalSeats) {
if (!movieExists(movieId)) {
return -1;
}
FILE *fp = fopen(SCREENINGS_FILE, "a");
if (!fp) {
perror("addScreening: could not open screenings file");
return -1;
}
int newId = getNextScreeningId();
fprintf(fp, "%d|%d|%s|%s|%s|%d|%d\n", newId, movieId, hall, date, time, totalSeats, totalSeats);
fclose(fp);
return newId;
}
int removeScreening(int id) {
int count;
Screening *arr = loadAllScreenings(&count);
if (!arr && count == 0) return 0;
int removed = 0;
int writeIndex = 0;
for (int i = 0; i < count; i++) {
if (arr[i].id == id) {
removed = 1;
continue;
}
arr[writeIndex++] = arr[i];
}
if (removed) writeAllScreenings(arr, writeIndex);
free(arr);
return removed;
}
int removeScreeningsByMovie(int movieId) {
int count;
Screening *arr = loadAllScreenings(&count);
if (!arr && count == 0) return 0;
int removedCount = 0;
int writeIndex = 0;
for (int i = 0; i < count; i++) {
if (arr[i].movieId == movieId) {
removedCount++;
continue;
}
arr[writeIndex++] = arr[i];
}
if (removedCount > 0) writeAllScreenings(arr, writeIndex);
free(arr);
return removedCount;
}
int decrementSeat(int screeningId) {
int count;
Screening *arr = loadAllScreenings(&count);
int ok = 0;
for (int i = 0; i < count; i++) {
if (arr[i].id == screeningId) {
if (arr[i].seatsAvailable > 0) {
    arr[i].seatsAvailable--;
ok = 1;
}
break;
}
}
if (ok) writeAllScreenings(arr, count);
free(arr);
return ok;
}
int incrementSeat(int screeningId) {
int count;
Screening *arr = loadAllScreenings(&count);
int ok = 0;
for (int i = 0; i < count; i++) {
if (arr[i].id == screeningId) {
if (arr[i].seatsAvailable < arr[i].totalSeats) {
arr[i].seatsAvailable++;
ok = 1;
}
break;
}
}
if (ok) writeAllScreenings(arr, count);
free(arr);
return ok;
}
void listScreenings(void) {
int count;
Screening *arr = loadAllScreenings(&count);
printf("\n%-5s %-8s %-12s %-12s %-8s %-8s %-10s\n", "ID", "MovieID", "Hall", "Date", "Time", "Total", "Available");
printf("--------------------------------------------------------------------\n");
for (int i = 0; i < count; i++) {
printf("%-5d %-8d %-12s %-12s %-8s %-8d %-10d\n", arr[i].id, arr[i].movieId, arr[i].hall, arr[i].date, arr[i].time, arr[i].totalSeats, arr[i].seatsAvailable);
}
if (count == 0) printf("No screenings found.\n");
free(arr);
}
void listScreeningsByMovie(int movieId) {
int count;
Screening *arr = loadAllScreenings(&count);
int any = 0;
printf("\n%-5s %-12s %-12s %-8s %-8s %-10s\n", "ID", "Hall", "Date", "Time", "Total", "Available");
printf("---------------------------------------------------------\n");
for (int i = 0; i < count; i++) {
if (arr[i].movieId == movieId) {
printf("%-5d %-12s %-12s %-8s %-8d %-10d\n",
arr[i].id, arr[i].hall, arr[i].date, arr[i].time, arr[i].totalSeats, arr[i].seatsAvailable);
any = 1;
}
}
if (!any) printf("No screenings found for this movie.\n");
free(arr);
}
