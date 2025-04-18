#ifndef TETROMINO_H
#define TETROMINO_H

//#include <SDL2/SDL.h>

typedef struct GameState GameState;
#include <stdbool.h>
#include <SDL2/SDL.h>

// Nombre de types de pièces
#define NUM_TETROMINOS 7

typedef struct {    
   // int shape[7][4][4][4]; // Forme du tetromino 
    int x, y;        
    int type;
    int rotation;    
} Tetromino;

// Tableau des formes de chaque pièce (type, rotation, 4x4)
extern const int TETROMINOS[NUM_TETROMINOS][4][4][4];

struct GameState;

void new_random_tetromino(GameState* state);
bool check_collision(GameState* state, int x, int y, int rotation);
void move_tetromino(GameState* state, int nx, int ny);
void rotate_tetromino(GameState* state);
void hard_drop(GameState* state);

#endif
