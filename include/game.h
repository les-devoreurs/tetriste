#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <stdbool.h>

#include "grid.h"
#include "tetromino.h"
#include "input.h"


typedef struct GameState{
    Tetromino next_piece;
    Tetromino current_piece;
    Tetromino stock_piece;
    bool has_already_stock;
    int grid[GRID_HEIGHT][GRID_WIDTH];
    int score;
    int level;
    int lines_cleared;
    bool game_over;
    Uint32 last_drop_time;
    int drop_speed;
} GameState;

void init_game(GameState* state);
int update_game(GameState* state);
int place_piece(GameState* state, Tetromino* tetromino );

#endif
