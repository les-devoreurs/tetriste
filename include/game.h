#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <stdbool.h>


#include "grid.h"
#include "tetromino.h"
#include "input.h"


typedef struct GameState{
    int grid[GRID_HEIGHT][GRID_WIDTH];
    Tetromino current_piece;
    int score;
    int level;
    int lines_cleared;
    bool game_over;
    Uint32 last_drop_time;
    int drop_speed;
} GameState;

void init_game(GameState* state);
void update_game(GameState* state);
void place_piece(GameState* state );

#endif
