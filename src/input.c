#include <SDL2/SDL.h>
#include "input.h"
#include "game.h"  


// Fonction pour gérer les entrées du joueur
void handle_input(SDL_Event* event, bool* quit, GameState* state, bool* return_to_menu) {

    while (SDL_PollEvent(event)) {
        switch (event->type) {
            case SDL_QUIT:
                *quit = true;
                break;

            case SDL_KEYDOWN:
                switch (event->key.keysym.sym) {
                    case SDLK_LEFT:
                        move_tetromino(state, -1, 0);
                        break;

                    case SDLK_RIGHT:
                        move_tetromino(state, 1, 0);
                        break;

                    case SDLK_DOWN:
                        move_tetromino(state, 0, 1);
                        break;

                    case SDLK_UP:
                        rotate_tetromino(state);
                        break;

                    case SDLK_SPACE:
                        hard_drop(state);
                        break;


                    case SDLK_ESCAPE:
                        *return_to_menu = true;  // <--- ⬅️ Revenir au menu

                    case SDLK_s: // Stock
                        stock_tetromino(state);
                        break;

                    default:
                        break;
                }
                break;

            default:
                break;
        }
    }
}
