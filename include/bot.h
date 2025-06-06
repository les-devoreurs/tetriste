#ifndef __BOT_TETRISTE_H__
#define __BOT_TETRISTE_H__

#include "grid.h"
#include "tetromino.h"

typedef struct {    
    int x;
    int y;      
    int rotation;  
    bool done;  
} BotMove;

int evaluate_position(GameState* state);
void preview_place_piece(GameState* state, int type, int rotation, int x, int y);
void copy_game_state(const GameState* src, GameState* copy);
void move_piece(GameState* state, int rotation, int x, int y);
BotMove find_best_move(GameState* state);
void apply_move_bot(GameState* state, BotMove* best_movement);


#endif /* end of  __BOT_H__ */
