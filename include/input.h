#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "game.h"

void handle_input(SDL_Event* event, bool* quit, GameState* state);

#endif
