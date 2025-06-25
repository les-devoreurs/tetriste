#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include "grid.h"
#include "tetromino.h"
#include "game.h"
#include <SDL2/SDL_ttf.h>

extern const SDL_Color COLORS[];

#define SCREEN_WIDTH (GRID_WIDTH * BLOCK_SIZE)
#define SCREEN_HEIGHT (GRID_HEIGHT * BLOCK_SIZE)


int init_renderer(SDL_Window** window, SDL_Renderer** renderer);
void clean_renderer(SDL_Window* window, SDL_Renderer* renderer);

void draw_on_renderer(SDL_Renderer* renderer, GameState* state, TTF_Font* font, SDL_Rect viewport);
void render_text(SDL_Renderer* renderer, const char* text, int x, int y, SDL_Color color, TTF_Font* font);
int resize_window(SDL_Window* window, int gamemode);

#endif