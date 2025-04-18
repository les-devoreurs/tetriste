#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include "grid.h"
#include "tetromino.h"

extern const SDL_Color COLORS[];

#define BLOCK_SIZE 30
#define SCREEN_WIDTH (GRID_WIDTH * BLOCK_SIZE)
#define SCREEN_HEIGHT (GRID_HEIGHT * BLOCK_SIZE)


void init_renderer(SDL_Window** window, SDL_Renderer** renderer);
void clean_renderer(SDL_Window* window, SDL_Renderer* renderer);
void draw_on_renderer(SDL_Renderer* renderer);

#endif