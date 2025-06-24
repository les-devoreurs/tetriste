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

    state->current_piece.x = 3;
    state->current_piece.y = 0;
    state->current_piece.type = rand() % 7;       // types 0 à 6
    state->current_piece.rotation = 0;

    new_random_tetromino(state);
}

void update_game(GameState* state){
    Uint32 current_time = SDL_GetTicks();

    // Vérifier s'il est temps de faire descendre la pièce
    if (current_time - state->last_drop_time > state->drop_speed) {
        // Essayer de déplacer la pièce vers le bas
        // if (!check_collision(state, state->current_piece.x, state->current_piece.y + 1, state->current_piece.type, state->current_piece.rotation)) {
        //     state->current_piece.y++;
        Tetromino* piece = &state->current_piece;

        if(!check_collision(state, piece->x, piece->y+1, piece->type, piece->rotation)){
            piece->y++;
            //state->last_drop_time = current_time;
        } else {
            // Si on ne peut pas descendre, placer la pièce dans la grille
            place_piece(state);
            return;
        }
        state->last_drop_time = current_time;
    }
}

void place_piece(GameState* state){

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            int block = TETROMINOS[state->current_piece.type][state->current_piece.rotation][i][j];
            
            if (block != 0) {
                int grid_x = state->current_piece.x + j;
                int grid_y = state->current_piece.y + i;
                
                if (grid_y >= 0 && grid_y < GRID_HEIGHT && grid_x >= 0 && grid_x < GRID_WIDTH) {
                    // +1 car 0 est réservé pour les cases vides
                    //state->grid[grid_y][grid_x] = TETROMINOS[state->current_piece.type][state->current_piece.rotation][i][j];
                    state->grid[grid_y][grid_x] = state->current_piece.type +1;
                }
            }
        }
    }
    
    clear_lines(state);
    new_random_tetromino(state);

        // Vcheck la collision, si oui game over
        Tetromino* new_piece = &state->current_piece;
        if (check_collision(state, new_piece->x, new_piece->y, new_piece->type, new_piece->rotation)) {
            state->game_over = true;
        }

    state->last_drop_time = SDL_GetTicks(); 
}