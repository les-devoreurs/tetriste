#include "tetromino.h"
#include "grid.h"
#include "game.h"
#include <stdlib.h>
#include <time.h>

// Définition des formes de tetrominos
const int TETROMINOS[7][4][4][4] = {
    // I
    {
        {{0,0,0,0}, {1,1,1,1}, {0,0,0,0}, {0,0,0,0}}, // représente la barre horizontale
        {{0,0,1,0}, {0,0,1,0}, {0,0,1,0}, {0,0,1,0}}, // barre verticale
        {{0,0,0,0}, {0,0,0,0}, {1,1,1,1}, {0,0,0,0}},
        {{0,1,0,0}, {0,1,0,0}, {0,1,0,0}, {0,1,0,0}}
    },
    // J
    {
        {{2,0,0,0}, {2,2,2,0}, {0,0,0,0}, {0,0,0,0}},
        {{0,2,2,0}, {0,2,0,0}, {0,2,0,0}, {0,0,0,0}},
        {{0,0,0,0}, {2,2,2,0}, {0,0,2,0}, {0,0,0,0}},
        {{0,2,0,0}, {0,2,0,0}, {2,2,0,0}, {0,0,0,0}}
    },
    // L
    {
        {{0,0,3,0}, {3,3,3,0}, {0,0,0,0}, {0,0,0,0}},
        {{0,3,0,0}, {0,3,0,0}, {0,3,3,0}, {0,0,0,0}},
        {{0,0,0,0}, {3,3,3,0}, {3,0,0,0}, {0,0,0,0}},
        {{3,3,0,0}, {0,3,0,0}, {0,3,0,0}, {0,0,0,0}}
    },
    // O
    {
        {{0,4,4,0}, {0,4,4,0}, {0,0,0,0}, {0,0,0,0}},
        {{0,4,4,0}, {0,4,4,0}, {0,0,0,0}, {0,0,0,0}},
        {{0,4,4,0}, {0,4,4,0}, {0,0,0,0}, {0,0,0,0}},
        {{0,4,4,0}, {0,4,4,0}, {0,0,0,0}, {0,0,0,0}}
    },
    // S
    {
        {{0,5,5,0}, {5,5,0,0}, {0,0,0,0}, {0,0,0,0}},
        {{0,5,0,0}, {0,5,5,0}, {0,0,5,0}, {0,0,0,0}},
        {{0,0,0,0}, {0,5,5,0}, {5,5,0,0}, {0,0,0,0}},
        {{5,0,0,0}, {5,5,0,0}, {0,5,0,0}, {0,0,0,0}}
    },
    // T
    {
        {{0,6,0,0}, {6,6,6,0}, {0,0,0,0}, {0,0,0,0}},
        {{0,6,0,0}, {0,6,6,0}, {0,6,0,0}, {0,0,0,0}},
        {{0,0,0,0}, {6,6,6,0}, {0,6,0,0}, {0,0,0,0}},
        {{0,6,0,0}, {6,6,0,0}, {0,6,0,0}, {0,0,0,0}}
    },
    // Z
    {
        {{7,7,0,0}, {0,7,7,0}, {0,0,0,0}, {0,0,0,0}},
        {{0,0,7,0}, {0,7,7,0}, {0,7,0,0}, {0,0,0,0}},
        {{0,0,0,0}, {7,7,0,0}, {0,7,7,0}, {0,0,0,0}},
        {{0,7,0,0}, {7,7,0,0}, {7,0,0,0}, {0,0,0,0}}
    }
};

void pos_start_piece(Tetromino* state) {
    state->rotation = 0;
    state->x = GRID_WIDTH / 2 - 2;
    state->y = -2;
}

void new_random_tetromino(Tetromino* current_piece, Tetromino* next_piece){
    current_piece->type = next_piece->type;
    pos_start_piece(current_piece);

    next_piece->type = rand() % NUM_TETROMINOS;
}

//Tetromino* peut être pas necessaire
bool check_collision(GameState* state, Tetromino* piece) {

    //printf("check_collision: x=%d y=%d rot=%d -> ", x, y, rotation);

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            
            int block = TETROMINOS[piece->type][piece->rotation][i][j] != 0;
            
            // Si la cellule est occupée par la pièce
            if (block != 0) {
                int new_x = piece->x + j ;
                int new_y = piece->y + i ;
                
                // Vérifier les limites de la grille
                if (new_x < 0 || new_x >= GRID_WIDTH || new_y >= GRID_HEIGHT) {
                    //printf("bord\n");
                    return true;  // Collision avec les bords
                }
                
                // Au-dessus de la grille → on ignore
                // if (new_y < 0) continue;

                // Collision avec un bloc déjà présent ?
                // if (state->grid[new_y][new_x]) {
                //     printf("bloc occupé\n");
                //     return true;
                // }
                if(new_y >= 0 && state->grid[new_y][new_x] != 0){
                    return true;
                }
            }
        }
    }
    return false;  // Pas de collision
}

void move_tetromino(GameState* state, int nx, int ny) {
    // Vérifier si le déplacement ne génère pas de collision
    
    Tetromino copy_temp;
    copy_temp.x = state->current_piece.x + nx;
    copy_temp.y = state->current_piece.y + ny;
    copy_temp.type = state->current_piece.type;
    copy_temp.rotation = state->current_piece.rotation;

    if (!check_collision(state, &copy_temp)) {
        state->current_piece.x += nx;
        state->current_piece.y += ny;
    }
}

void rotate_tetromino(GameState* state) {
    int next_rotation = (state->current_piece.rotation + 1) % 4;
    // Vérifier si la rotation est valide
    Tetromino copy_temp;
    copy_temp.x = state->current_piece.x;
    copy_temp.y = state->current_piece.y;
    copy_temp.type = state->current_piece.type;
    copy_temp.rotation = next_rotation;
    if (!check_collision(state, &copy_temp)) {
        state->current_piece.rotation = next_rotation;
    }
}

void hard_drop(GameState* state) {
    Tetromino copy_temp;
    copy_temp.x = state->current_piece.x;
    copy_temp.y = state->current_piece.y + 1;
    copy_temp.type = state->current_piece.type;
    copy_temp.rotation = state->current_piece.rotation;

    while (!check_collision(state, &copy_temp)) {
        state->current_piece.y += 1;
        
        copy_temp.y = state->current_piece.y + 1;
    }
}

void stock_tetromino(GameState* state) {
    if (!state->has_already_stock) {
        if (state->stock_piece.type == -1) {
            //si vide juste passer a la pièce suivante et stocker celle actuel
            state->stock_piece.type = state->current_piece.type;
            new_random_tetromino(&state->current_piece, &state->next_piece);
        }
        else {
            //sinon mettre le type de la pièce en stock a la place de celle en stock et celle actuel en stock
            int stock_piece_type = state->stock_piece.type;
            state->stock_piece.type = state->current_piece.type;
            state->current_piece.type = stock_piece_type;
            pos_start_piece(&state->current_piece);
        }
        state->has_already_stock = true;
    }
}
