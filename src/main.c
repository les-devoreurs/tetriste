#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include "game.h"
#include "renderer.h"
#include "input.h"
#include "menu.h"
#include <SDL2/SDL_ttf.h>
#include "scores.h"

int main(int argc, char** argv) {

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        return 1;
    }

    //init font (potentiellement faire un fichier dédié à la gestion de font)
    if (TTF_Init() != 0) {
        printf("Erreur TTF_Init: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    //ouvrir font
    TTF_Font* font = TTF_OpenFont("src/Tetris.ttf", 24);
    if (!font) {
        printf("Erreur chargement police : %s\n", TTF_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 600, SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    bool global_quit = false;

    while (!global_quit) {
        int selected_option = menu(window, renderer);
        if (selected_option < 0 || selected_option == 3) { // Quitter
            global_quit = true;
            break;
        }


        if (selected_option == 2) {
            ScoreEntry scores[MAX_SCORES];
            int count = load_scores("src/scores.txt", scores, MAX_SCORES);
            sort_scores(scores, count);
            display_scores(renderer, font, scores, count);
            continue;
        }
        

        srand(time(NULL));
        GameState state;
        init_game(&state);

        bool quit_game = false;
        bool return_to_menu = false;
        SDL_Event event;

        while (!quit_game && !state.game_over && !return_to_menu) {
            handle_input(&event, &quit_game, &state, &return_to_menu); //ajout de l'echap
            update_game(&state);
            draw_on_renderer(renderer, &state, font);
            SDL_Delay(16);
        }

        if (state.game_over) {
            char name[MAX_NAME_LENGTH] = "Joueur"; //score mais pas encore implémenter
            save_score("src/scores.txt", name, state.score);
            SDL_Delay(2000);
            
        }

        //revenir au menu si le bool return_to_menu true, sinon on boucle (ou quitte)
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    TTF_CloseFont(font);
    return 0;
}
