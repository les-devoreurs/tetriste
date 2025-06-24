#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include "game.h"
#include "renderer.h"
#include "input.h"
#include "menu.h"
#include <SDL2/SDL_ttf.h>
#include "scores.h"
#include "bot.h"

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

    bool global_quit = false;
    
    // Initialisation du générateur de nombres aléatoires
    srand(time(NULL));

    // Initialiser le visuel du jeu
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    int screen_width = init_renderer(&window, &renderer);

    while (!global_quit) {
        // resize to menu window
        screen_width = resize_window(window ,0);
        
        int selected_option = menu(window, renderer); // 0 : Le mode solo, 1 : Le mode vs bot
        if (selected_option < 0 || selected_option == 3) { // Quitter
            global_quit = true;
            break;
        }
        // à revoir
        else if (selected_option == 2) {
            ScoreEntry scores[MAX_SCORES];
            int count = load_scores("src/scores.txt", scores, MAX_SCORES);
            sort_scores(scores, count);
            display_scores(renderer, font, scores, count);
            continue;
        }
        // à revoir
        
        // resize window
        screen_width = resize_window(window ,selected_option);

        GameState statePlayer;
        init_game(&statePlayer);
        SDL_Rect viewport_player = {0, 0,  screen_width, SCREEN_HEIGHT};
        int lines_cleared_player = 0;
        if (selected_option == 1)
        {
            viewport_player = (SDL_Rect) {0, 0,  screen_width/2, SCREEN_HEIGHT};
        }

        GameState stateBot;
        SDL_Rect viewport_bot;
        int lines_cleared_bot = 0;
        if (selected_option == 1) {
            init_game(&stateBot);
            stateBot.drop_speed = 300;
            viewport_bot = (SDL_Rect) {screen_width/2, 0,  screen_width/2, SCREEN_HEIGHT};
        }

        bool return_to_menu = false;
        SDL_Event event;
        Tetromino best_move = {3, 3, 3, 3};
        while (!global_quit && !statePlayer.game_over && !stateBot.game_over && !return_to_menu) {
            handle_input(&event, &global_quit, &statePlayer, &return_to_menu); // Gestion des entrées clavier
            
            Uint32 now_player = SDL_GetTicks();
            if (now_player - statePlayer.last_drop_time > statePlayer.drop_speed) {
                int temp_lines_cleared_player = update_game(&statePlayer);  // Mise à jour de l'état du jeu

                if(selected_option == 1 &&  temp_lines_cleared_player != -1) {
                    lines_cleared_player += temp_lines_cleared_player;
                    add_garbage_lines(&statePlayer, lines_cleared_bot);     // Ajoute une ligne dans la grille pour chaque ligne finit par le bot
                    lines_cleared_bot = 0;
                }
                statePlayer.last_drop_time = now_player;
            }
            
            if (selected_option == 1) {
                Uint32 now_bot = SDL_GetTicks();
                if (now_bot - stateBot.last_drop_time > stateBot.drop_speed) {
                    best_move = find_best_move(&stateBot);
                    apply_move_bot(&stateBot, &best_move);
                    
                    int temp_lines_cleared_bot = update_game(&stateBot);                // Mise à jour de l'état du jeu
                    if (temp_lines_cleared_bot != -1) {
                        lines_cleared_bot += temp_lines_cleared_bot;
                        add_garbage_lines(&stateBot, lines_cleared_player);     // Ajoute une ligne dans la grille pour chaque ligne finit par le joueur 
                        best_move.y -= lines_cleared_player;
                        lines_cleared_player = 0;
                    }
                    stateBot.last_drop_time = now_bot;
                }
            }
            
            SDL_RenderClear(renderer);
            draw_on_renderer(renderer, &statePlayer, font, viewport_player);  // Rendu graphique joueur
            if (selected_option == 1)
            {
                draw_on_renderer(renderer, &stateBot, font, viewport_bot);        // Rendu graphique bot
            }

            // Afficher le résultat
            SDL_RenderPresent(renderer);
            SDL_Delay(16); // ~60 FPS
        }

        // Game Over
        if (selected_option == 1 && statePlayer.game_over) {
            printf("Game Over! You lose against the bot!\n");
            SDL_Delay(2000); // Attente avant de quitter
        }
        else if (selected_option == 1 && stateBot.game_over) {
            printf("Win! You win against the bot!\n");
            SDL_Delay(2000); // Attente avant de quitter
        }
        else if (selected_option == 0) {
            printf("Game Over! Score:%u\n", statePlayer.score);
        }

        /*
        if (state.game_over) {
            char name[MAX_NAME_LENGTH] = "Joueur"; //score mais pas encore implémenter
            save_score("src/scores.txt", name, state.score);
            SDL_Delay(2000);
            
        }*/

        //revenir au menu si le bool return_to_menu true, sinon on boucle (ou quitte)

    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    TTF_CloseFont(font);
    return 0;
}

