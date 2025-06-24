// scores.h
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>


#ifndef SCORES_H
#define SCORES_H

#define MAX_NAME_LENGTH 20
#define MAX_SCORES 10

typedef struct {
    char name[MAX_NAME_LENGTH];
    int score;
} ScoreEntry;

void save_score(const char* filename, const char* name, int score);
int load_scores(const char* filename, ScoreEntry* scores, int max);
void sort_scores(ScoreEntry* scores, int count);
void display_scores(SDL_Renderer* renderer, TTF_Font* font, ScoreEntry* scores, int count);

#endif
