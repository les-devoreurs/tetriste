#include <SDL_stdinc.h>
#include "game.h"
#include "tetromino.h"
#include "renderer.h"

void init_game(GameState* state){

    for(int y = 0 ; y < GRID_HEIGHT ; y++){
        for(int x = 0 ; x < GRID_WIDTH ; x++){
            state->grid[y][x] = 0;
        }
    }

    state->score = 0;
    state->level = 1;
    state->lines_cleared = 0;
    state->game_over = false;
    state->drop_speed = 1000;
    state->last_drop_time = SDL_GetTicks();
    state->has_already_stock = false;

    state->next_piece.type = rand() % 7;       // types 0 à 6
    state->stock_piece.type = -1;

    new_random_tetromino(&state->current_piece, &state->next_piece);
}


int update_game(GameState* state) {

    Tetromino piece = {state->current_piece.x, state->current_piece.y + 1, state->current_piece.type, state->current_piece.rotation};

    if(!check_collision(state, &piece)){
        state->current_piece.y++;
        return -1;
    } else {
        // Si on ne peut pas descendre, placer la pièce dans la grille
        return place_piece(state, &state->current_piece);
    }
}

//Tetromino* peut être pas necessaire
int place_piece(GameState* state, Tetromino* tetromino) {

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            int block = TETROMINOS[tetromino->type][tetromino->rotation][i][j];
            
            if (block != 0) {
                int grid_x = tetromino->x + j;
                int grid_y = tetromino->y + i;
                
                if (grid_y >= 0 && grid_y < GRID_HEIGHT && grid_x >= 0 && grid_x < GRID_WIDTH) {
                    // +1 car 0 est réservé pour les cases vides
                    state->grid[grid_y][grid_x] = tetromino->type +1;

                    //if (state->drop_speed > 500) {
                    //    printf(" %u : %u \n", grid_y, grid_x);
                    //}
                    if(grid_y == 0) {
                        //printf(" %u : %u \n", grid_y, state->drop_speed);
                        state->game_over = true;
                    }
                }
            }
        }
    }
    

    new_random_tetromino(tetromino, &state->next_piece);
    state->last_drop_time = SDL_GetTicks();
    state->has_already_stock = false;
    return clear_lines(state);
}