#ifndef GRID_H
#define GRID_H

typedef struct GameState GameState;

//valeurs de la grille
#define GRID_HEIGHT 20
#define GRID_WIDTH 10      
#define BLOCK_SIZE 30


int clear_lines(GameState* state);
void add_garbage_lines(GameState* state, int number_lines);

#endif
