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

void new_random_tetromino(GameState* state){
    state->current_piece.type = rand() % NUM_TETROMINOS;
    state->current_piece.rotation = 0;
    state->current_piece.x = GRID_WIDTH / 2 - 2;
    state->current_piece.y = -2;
}

bool check_collision(GameState* state, int x, int y, int type, int rotation) {

    //printf("check_collision: x=%d y=%d rot=%d -> ", x, y, rotation);

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            
            int block = TETROMINOS[type][rotation][i][j] != 0;
            
            // Si la cellule est occupée par la pièce
            if (block != 0) {
                int new_x = x + j ;
                int new_y = y + i ;
                
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
    //printf("move_tetromino: dx=%d dy=%d\n", nx, ny);

    if (!check_collision(state, state->current_piece.x + nx, state->current_piece.y + ny, state->current_piece.type, state->current_piece.rotation)) {
        state->current_piece.x += nx;
        state->current_piece.y += ny;
    }
}

void rotate_tetromino(GameState* state) {
    int next_rotation = (state->current_piece.rotation + 1) % 4;
    // Vérifier si la rotation est valide
    if (!check_collision(state, state->current_piece.x, state->current_piece.y, state->current_piece.type, next_rotation)) {
        state->current_piece.rotation = next_rotation;
    }
}

void hard_drop(GameState* state) {
    while (!check_collision(state, state->current_piece.x, state->current_piece.y + 1, state->current_piece.type, state->current_piece.rotation)) {
        state->current_piece.y += 1;
    }
}

