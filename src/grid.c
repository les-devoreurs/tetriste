#include "grid.h"
#include "game.h"



void clear_lines(GameState* state) {

    for (int y = GRID_HEIGHT - 1; y >= 0; y--) {
        bool line_full = true;

        for (int x = 0; x < GRID_WIDTH; x++) {
            if (state->grid[y][x] == 0) {
                line_full = true;
                break;
            }
        }
        if (line_full) {
            // on efface la ligne full
            for(int x = 0 ; x < GRID_WIDTH ; x++){
                state->grid[y][x] = 0;
            }
            // décaler les lignes vers le bas
            for (int i = y; i > 0; i--) {
                for (int x = 0; x < GRID_WIDTH; x++) {
                    state->grid[i][x] = state->grid[i-1][x];
                }
            }

            // Remplir la ligne du haut avec des 0 
            for(int x = 0 ; x < GRID_WIDTH ; x++){
                state->grid[0][x] = 0;
            }

            // Changement du score
            state->score += 100;
            state->lines_cleared++;
            
            // Changement de niveau si besoin
            if(state->lines_cleared >= 10){
                state->level++;
                state->lines_cleared = 0;
            }

        
        }
    }
}


