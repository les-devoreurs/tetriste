// main.c
#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include "game.h"
#include "renderer.h"
#include "input.h"
#include "bot.h"

int main(int argc, char** argv) {
    // Initialisation SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Erreur SDL_Init : %s\n", SDL_GetError());
        return 1;
    }

    // Initialisation du générateur de nombres aléatoires
    srand(time(NULL));


    // Initialiser l'état du jeu
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    init_renderer(&window, &renderer);

    GameState statePlayer;
    init_game(&statePlayer);
    SDL_Rect viewport_player = {0, 0,  SCREEN_WIDTH/2, SCREEN_HEIGHT};


    GameState stateBot;
    init_game(&stateBot);
    SDL_Rect viewport_bot = {SCREEN_WIDTH/2, 0,  SCREEN_WIDTH/2, SCREEN_HEIGHT};

    // Boucle principale du jeu
    bool quit = false;
    SDL_Event event;

    BotMove best_move = {3, 3, 3, true};
    
    while (!quit && !statePlayer.game_over) {
        handle_input(&event, &quit, &statePlayer); // Gestion des entrées clavier
        
        Uint32 now_player = SDL_GetTicks();
        if (now_player - statePlayer.last_drop_time > statePlayer.drop_speed) {
            update_game(&statePlayer); // Mise à jour de l'état du jeu
            statePlayer.last_drop_time = now_player;
        }
        
        Uint32 now_bot = SDL_GetTicks();
        if (now_bot - stateBot.last_drop_time > stateBot.drop_speed) {
            if (best_move.done) {
                printf("%u : %u \n", now_bot, best_move.done);
                best_move = find_best_move(&stateBot);
            }
            apply_move_bot(&stateBot, &best_move);
            update_game(&stateBot); // Mise à jour de l'état du jeu
            stateBot.last_drop_time = now_bot;
        }
        
        SDL_RenderClear(renderer);
        draw_on_renderer(renderer, &statePlayer, viewport_player); // Rendu graphique
        draw_on_renderer(renderer, &stateBot, viewport_bot); // Rendu graphique

        // Afficher le résultat
        SDL_RenderPresent(renderer);
        SDL_Delay(16); // ~60 FPS
    }

    // Game Over
    if (statePlayer.game_over) {
        printf("Game Over! Score: %d\n", statePlayer.score);
        SDL_Delay(2000); // Attente avant de quitter
    }

    clean_renderer(window, renderer); // Nettoyage SDL
    SDL_Quit();
    return 0;
}
