// main.c
#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include "game.h"
#include "renderer.h"
#include "input.h"

int main(int argc, char** argv) {
    // Initialisation SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Erreur SDL_Init : %s\n", SDL_GetError());
        return 1;
    }

    // Initialisation du générateur de nombres aléatoires
    srand(time(NULL));


    // Initialiser l'état du jeu
    GameState state;
    init_game(&state);

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    
    init_renderer(&window, &renderer);

     

    // Boucle principale du jeu
    bool quit = false;
    SDL_Event event;

    while (!quit && !state.game_over) {
        handle_input(&event, &quit, &state); // Gestion des entrées clavier
        update_game(&state); // Mise à jour de l'état du jeu
        draw_on_renderer(renderer, &state); // Rendu graphique
        SDL_Delay(16); // ~60 FPS
    }

    // Game Over
    if (state.game_over) {
        printf("Game Over! Score: %d\n", state.score);
        SDL_Delay(2000); // Attente avant de quitter
    }

    clean_renderer(window, renderer); // Nettoyage SDL
    SDL_Quit();
    return 0;
}
