#ifndef GRID_H
#define GRID_H

typedef struct GameState GameState;

#define GRID_WIDTH 10
#define GRID_HEIGHT 20


int clear_lines(GameState* state);
void add_garbage_lines(GameState* state, int number_lines);

#endif
