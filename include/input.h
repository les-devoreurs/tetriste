#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "game.h"

// ajout d'un bool return to menu
void handle_input(SDL_Event* event, bool* quit, GameState* state, bool* return_to_menu);

#endif
