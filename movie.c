#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "movie.h"

static int parseMovieLine(const char *line, Movie *m) {
char buf[256];
strncpy(buf, line, sizeof(buf) - 1);
buf[sizeof(buf) - 1] = '\0';
char *idTok       = strtok(buf, "|");
char *titleTok     = strtok(NULL, "|");
char *genreTok      = strtok(NULL, "|");
char *durationTok    = strtok(NULL, "|\n");
if (!idTok || !titleTok || !genreTok || !durationTok) return 0;
m->id = atoi(idTok);
strncpy(m->title, titleTok, MAX_TITLE_LEN - 1);
m->title[MAX_TITLE_LEN - 1] = '\0';
strncpy(m->genre, genreTok, MAX_GENRE_LEN - 1);
m->genre[MAX_GENRE_LEN - 1] = '\0';
m->duration = atoi(durationTok);
return 1;
}
int getNextMovieId(void) {
FILE *fp = fopen(MOVIES_FILE, "r");
if (!fp) return 1;
char line[256];
int maxId = 0;
Movie m;
while (fgets(line, sizeof(line), fp)) {
if (parseMovieLine(line, &m) && m.id > maxId) {
maxId = m.id;
}
}
fclose(fp);
return maxId + 1;
}
int movieExists(int id) {
Movie m;
return loadMovieById(id, &m);
}
int loadMovieById(int id, Movie *out) {
FILE *fp = fopen(MOVIES_FILE, "r");
if (!fp) return 0;
char line[256];
Movie m;
int found = 0;
while (fgets(line, sizeof(line), fp)) {
if (parseMovieLine(line, &m) && m.id == id) {
*out = m;
found = 1;
break;
}
}
fclose(fp);
return found;
}
int addMovie(const char *title, const char *genre, int duration) {
FILE *fp = fopen(MOVIES_FILE, "a");
if (!fp) {
perror("addMovie: could not open movies file");
return -1;
}
int newId = getNextMovieId();
fprintf(fp, "%d|%s|%s|%d\n", newId, title, genre, duration);
fclose(fp);
return newId;
}
int removeMovie(int id) {
FILE *fp = fopen(MOVIES_FILE, "r");
if (!fp) return 0;

FILE *tmp = fopen("data/movies.tmp", "w");
if (!tmp) {
fclose(fp);
return 0;
}
char line[256];
Movie m;
int removed = 0;
while (fgets(line, sizeof(line), fp)) {
if (parseMovieLine(line, &m) && m.id == id) {
removed = 1;
continue; 
}
fputs(line, tmp);
}
fclose(fp);
fclose(tmp);
remove(MOVIES_FILE);
rename("data/movies.tmp", MOVIES_FILE);
return removed;
}
void listMovies(void) {
FILE *fp = fopen(MOVIES_FILE, "r");
if (!fp) {
printf("No movies found.\n");
return;
}
char line[256];
Movie m;
int any = 0;
printf("\n%-5s %-30s %-15s %-10s\n", "ID", "Title", "Genre", "Duration");
printf("--------------------------------------------------------------\n");
while (fgets(line, sizeof(line), fp)) {
if (parseMovieLine(line, &m)) {
printf("%-5d %-30s %-15s %-10d\n", m.id, m.title, m.genre, m.duration);
any = 1;
}
}
fclose(fp);
if (!any) {
printf("No movies found.\n");
}
}
