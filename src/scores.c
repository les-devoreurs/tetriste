// scores.c
#include "renderer.h"
#include "scores.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


void save_score(const char* filename, const char* name, int score) {
    FILE* f = fopen(filename, "a");
    if (f == NULL) return;
    fprintf(f, "%s %d\n", name, score);
    fclose(f);
}

int load_scores(const char* filename, ScoreEntry* scores, int max) {
    FILE* f = fopen(filename, "r");
    if (!f) return 0;

    int count = 0;
    while (count < max && fscanf(f, "%19s %d", scores[count].name, &scores[count].score) == 2) {
        count++;
    }

    fclose(f);
    return count;
}

int compare_scores(const void* a, const void* b) {
    ScoreEntry* s1 = (ScoreEntry*)a;
    ScoreEntry* s2 = (ScoreEntry*)b;
    return s2->score - s1->score; // tri décroissant
}

void sort_scores(ScoreEntry* scores, int count) {
    qsort(scores, count, sizeof(ScoreEntry), compare_scores);
}

void display_scores(SDL_Renderer* renderer, TTF_Font* font, ScoreEntry* scores, int count) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Color white = {255, 255, 255, 255};
    render_text(renderer, "Tableau des Scores", 180, 50, white, font);

    char buffer[64];
    for (int i = 0; i < count; i++) {
        snprintf(buffer, sizeof(buffer), "%d. %s - %d", i+1, scores[i].name, scores[i].score);
        render_text(renderer, buffer, 200, 100 + i * 30, white, font);
    }

    SDL_RenderPresent(renderer);

    SDL_Event event;
    bool wait = true;
    while (wait) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                wait = false;
            } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                wait = false;
            }
        }
    }
}
