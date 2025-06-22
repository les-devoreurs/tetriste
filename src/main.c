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

    int gamemode = 1; // 0: pour le mode solo 1: pour le mode vs bot

    // Initialisation du générateur de nombres aléatoires
    srand(time(NULL));

    // Initialiser l'état du jeu
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    int screen_width = init_renderer(&window, &renderer, gamemode);

    GameState statePlayer;
    init_game(&statePlayer);
    SDL_Rect viewport_player = {0, 0,  screen_width/2, SCREEN_HEIGHT};

    GameState stateBot;
    SDL_Rect viewport_bot;
    if (gamemode == 1) {
        init_game(&stateBot);
        stateBot.drop_speed = 300;
        viewport_bot = (SDL_Rect) {screen_width/2, 0,  screen_width/2, SCREEN_HEIGHT};
    }

    // Boucle principale du jeu
    bool quit = false;
    SDL_Event event;

    Tetromino best_move = {3, 3, 3, 3};
    
    while (!quit && !statePlayer.game_over && !stateBot.game_over) {
        handle_input(&event, &quit, &statePlayer); // Gestion des entrées clavier
        
        Uint32 now_player = SDL_GetTicks();
        if (now_player - statePlayer.last_drop_time > statePlayer.drop_speed) {
            int lines_cleared = update_game(&statePlayer);  // Mise à jour de l'état du jeu
            add_garbage_lines(&stateBot, lines_cleared);    // Ajoute une ligne dans la grille adversaire pour chaque ligne finit 
            best_move.y -= lines_cleared;
            statePlayer.last_drop_time = now_player;
        }
        
        if (gamemode == 1) {
            Uint32 now_bot = SDL_GetTicks();
            if (now_bot - stateBot.last_drop_time > stateBot.drop_speed) {
                best_move = find_best_move(&stateBot);
                apply_move_bot(&stateBot, &best_move);
                int lines_cleared = update_game(&stateBot);         // Mise à jour de l'état du jeu
                add_garbage_lines(&statePlayer, lines_cleared);     // Ajoute une ligne dans la grille adversaire pour chaque ligne finit
                stateBot.last_drop_time = now_bot;
            }
        }
        
        SDL_RenderClear(renderer);
        draw_on_renderer(renderer, &statePlayer, viewport_player);  // Rendu graphique joueur
        if (gamemode == 1)
        {
            draw_on_renderer(renderer, &stateBot, viewport_bot);        // Rendu graphique bot
        }

        // Afficher le résultat
        SDL_RenderPresent(renderer);
        SDL_Delay(16); // ~60 FPS
    }

    // Game Over
    if (statePlayer.game_over) {
        printf("Game Over! You lose against the bot!\n");
        SDL_Delay(2000); // Attente avant de quitter
    }
    else if (stateBot.game_over) {
        printf("Win! You win against the bot!\n");
        SDL_Delay(2000); // Attente avant de quitter
    }

    clean_renderer(window, renderer); // Nettoyage SDL
    SDL_Quit();
    return 0;
}
