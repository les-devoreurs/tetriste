#include "grid.h"
#include "game.h"

int clear_lines(GameState* state) {

    int lines_cleared = 0;

    for (int y = GRID_HEIGHT - 1; y >= 0; y--) {
        bool line_full = true;

        for (int x = 0; x < GRID_WIDTH; x++) {
            if (state->grid[y][x] == 0) {
                line_full = false;
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
            y++;

            // Remplir la ligne du haut avec des 0 
            for(int x = 0 ; x < GRID_WIDTH ; x++){
                state->grid[0][x] = 0;
            }

            // Changement du score
            state->score += 100;
            state->lines_cleared++;
            lines_cleared++;
            
            // Changement de niveau si besoin
            if(state->lines_cleared % 5 == 0){
                state->level++;
                //printf(" passe \n");
                if (state->drop_speed <= 100) {
                    state->drop_speed = 100;
                }
                else {
                    state->drop_speed = state->drop_speed - 100;
                }
                state->lines_cleared = 0;
            }
        
        }
    }

    return lines_cleared;
}

void add_garbage_lines(GameState* state, int number_lines) {
    for (int i = 0; i < number_lines; i++) {
        // Décaler toutes les lignes vers le haut
        for (int y = 0; y < GRID_HEIGHT - 1; y++) {
            for (int x = 0; x < GRID_WIDTH; x++) {
                state->grid[y][x] = state->grid[y + 1][x];
            }
        }

        // Ajouter une nouvelle ligne en bas avec un trou aléatoire
        int hole_x = rand() % GRID_WIDTH;
        for (int x = 0; x < GRID_WIDTH; x++) {
            state->grid[GRID_HEIGHT - 1][x] = (x == hole_x) ? 0 : 9;
        }
    }
}
