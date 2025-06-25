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
void display_game_over_scores(SDL_Renderer* renderer, TTF_Font* font, const char* filename, 
    const char* player_name, int player_score);
int find_player_rank(ScoreEntry* scores, int count, int player_score);
void get_player_name(char* name, int max_length);
void display_duel_result(SDL_Renderer* renderer, TTF_Font* font, bool player_won);
#endif
