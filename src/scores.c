// scores.c
#include "renderer.h"
#include "scores.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


void save_score(const char* filename, const char* name, int score) {
    ScoreEntry scores[MAX_SCORES];
    int count = load_scores(filename, scores, MAX_SCORES);

    bool updated = false;
    for (int i = 0; i < count; i++) {
        if (strcmp(scores[i].name, name) == 0) {
            if (score > scores[i].score) {
                scores[i].score = score;  // mise à jour si score meilleur
            }
            updated = true;
            break;
        }
    }

    // Si le nom n'existait pas, on l'ajoute
    if (!updated && count < MAX_SCORES) {
        strcpy(scores[count].name, name);
        scores[count].score = score;
        count++;
    }

    // Réécrire tout le fichier
    FILE* f = fopen(filename, "w");
    if (!f) return;
    for (int i = 0; i < count; i++) {
        fprintf(f, "%s %d\n", scores[i].name, scores[i].score);
    }
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
    render_text(renderer, "Tableau des Scores", 100, 50, white, font);

    char buffer[64];
    for (int i = 0; i < count; i++) {
        snprintf(buffer, sizeof(buffer), "%d. %s - %d", i+1, scores[i].name, scores[i].score);
        render_text(renderer, buffer, 100, 100 + i * 30, white, font);
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
    int find_player_rank(ScoreEntry* scores, int count, int player_score) {
        for (int i = 0; i < count; i++) {
            if (scores[i].score == player_score) {
                return i + 1; 
            }
        }
        return -1; 
    }

void get_player_name(char* name, int max_length) {
    printf("Entrez votre nom (max %d caractères): ", max_length - 1);
    if (fgets(name, max_length, stdin)) {
        //retour chariot
        name[strcspn(name, "\n")] = 0;
        
        //nom par défaut Joueur
        if (strlen(name) == 0) {
            strcpy(name, "Joueur");
        }
        
        //clean
        int len = strlen(name);
        while (len > 0 && isspace(name[len-1])) {
            name[--len] = 0;
        }
        
        if (len == 0) {
            strcpy(name, "Joueur");
        }
    } else {
        strcpy(name, "Joueur");
    }
}

void display_game_over_scores(SDL_Renderer* renderer, TTF_Font* font, const char* filename, 
                             const char* player_name, int player_score) {
    
    ScoreEntry scores[MAX_SCORES];
    int count = load_scores(filename, scores, MAX_SCORES);
    
    // Ajouter le score du joueur actuel temporairement pour le classement
    ScoreEntry temp_scores[MAX_SCORES + 1];
    for (int i = 0; i < count; i++) {
        temp_scores[i] = scores[i];
    }
    
    //vérifier si le score du joueur est déjà présent (évite les doublons)
    bool already_present = false;
    for (int i = 0; i < count; i++) {
        if (strcmp(scores[i].name, player_name) == 0 && scores[i].score == player_score) {
            already_present = true;
            break;
        }
    }

    if (!already_present) {
        strcpy(temp_scores[count].name, player_name);
        temp_scores[count].score = player_score;
        count++;
    }

    
    sort_scores(temp_scores, count);
    
    // Trouver le rang du joueur
    int player_rank = find_player_rank(temp_scores, count, player_score);
    
    // Effacer l'écran
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    SDL_Color white = {255, 255, 255, 255};
    SDL_Color yellow = {255, 255, 0, 255};
    SDL_Color red = {255, 100, 100, 255};
    
    const char* title = "GAME OVER - Mode Classique";
    render_text(renderer, title, 30, 30, red, font);
    
    // Score du joueur
    char player_info[128];
    if (player_rank <= 10) {
        snprintf(player_info, sizeof(player_info), "Votre score: %d (Rang #%d)", player_score, player_rank);
        render_text(renderer, player_info, 30, 70, yellow, font);
        render_text(renderer, "Bravo TOP 10!", 70, 100, yellow, font);
    } else {
        snprintf(player_info, sizeof(player_info), "Votre score: %d (Rang #%d)", player_score, player_rank);
        render_text(renderer, player_info, 30, 70, white, font);
    }
    
    // Titre du tableau
    render_text(renderer, "TOP 10", 30, 140, white, font);
    
    // Afficher le top 10
    char buffer[64];
    for (int i = 0; i < count && i < 10; i++) {
        SDL_Color color = white;
        
        // Mettre en surbrillance le score du joueur actuel s'il est dans le top 10
        if (strcmp(temp_scores[i].name, player_name) == 0 && 
            temp_scores[i].score == player_score) {
            color = yellow;
            snprintf(buffer, sizeof(buffer), "%d. %s - %d <<<", i+1, temp_scores[i].name, temp_scores[i].score);
        } else {
            snprintf(buffer, sizeof(buffer), "%d. %s - %d", i+1, temp_scores[i].name, temp_scores[i].score);
        }
        
        render_text(renderer, buffer, 50, 170 + i * 25, color, font);
    }
    
    render_text(renderer, "Espace pour continuer", 40, 450, white, font);
    
    SDL_RenderPresent(renderer);
    
    // Attendre l'input du joueur
    SDL_Event event;
    bool wait = true;
    while (wait) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                wait = false;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_SPACE) {
                    wait = false;
                }
            }
        }
        SDL_Delay(16);
    }
}

void display_duel_result(SDL_Renderer* renderer, TTF_Font* font, bool player_won) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Color color = player_won ? (SDL_Color){0, 255, 0, 255} : (SDL_Color){255, 0, 0, 255};
    const char* message = player_won ? "Vous avez GAGNE !" : "Vous avez PERDU !";

    render_text(renderer, "Resultat du Duel", 100, 100, (SDL_Color){255, 255, 255, 255}, font);
    render_text(renderer, message, 120, 180, color, font);
    render_text(renderer, "Espace pour continuer", 60, 300, (SDL_Color){255, 255, 255, 255}, font);

    SDL_RenderPresent(renderer);

    SDL_Event event;
    bool wait = true;
    while (wait) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                wait = false;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_SPACE) {
                    wait = false;
                }
            }
        }
        SDL_Delay(16);
    }
}

