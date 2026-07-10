#ifndef MOVIE_H
#define MOVIE_H
#define MAX_TITLE_LEN   100
#define MAX_GENRE_LEN   50
#define MOVIES_FILE     "data/movies.txt"

typedef struct {
int  id;
char title[MAX_TITLE_LEN];
char genre[MAX_GENRE_LEN];
int  duration;  
} Movie;

int  getNextMovieId(void);
int  movieExists(int id);
int  loadMovieById(int id, Movie *out);
int  addMovie(const char *title, const char *genre, int duration);
int  removeMovie(int id);
void listMovies(void);

#endif 
