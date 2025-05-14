#include "renderer.h"
#include "grid.h"
#include "tetromino.h"
#include "game.h"

#include <stdio.h>
#include <stdlib.h>

// Couleurs des tetrominos (R,G,B)
const SDL_Color COLORS[] = {
    {0, 0, 0},       // 0 - Noir (vide)
    {0, 255, 255},   // 1 - Cyan (I)
    {0, 0, 255},     // 2 - Bleu (J)
    {255, 165, 0},   // 3 - Orange (L)
    {255, 255, 0},   // 4 - Jaune (O)
    {0, 255, 0},     // 5 - Vert (S)
    {128, 0, 128},   // 6 - Violet (T)
    {255, 0, 0}      // 7 - Rouge (Z)
};

SDL_Renderer* temp_renderer = NULL;  // stocké globalement ici pour draw()

void init_renderer(SDL_Window** window, SDL_Renderer** renderer)
{
    *window = SDL_CreateWindow("Tetriste", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                               SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!*window) {
        printf("Erreur création fenêtre : %s\n", SDL_GetError());
        exit(1);
    }
    
    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (!*renderer) {
        printf("Erreur création renderer : %s\n", SDL_GetError());
        exit(1);
    }

    temp_renderer = *renderer; // On le garde en global pour draw()
    
}

void clean_renderer(SDL_Window* window, SDL_Renderer* renderer){

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

}

void draw_on_renderer(SDL_Renderer* renderer,GameState* state){

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); //fond noir
    SDL_RenderClear(renderer);

    Tetromino* current_piece = &state->current_piece;

    printf("DEBUG: renderer = %p, state = %p\n", renderer, state);
printf("DEBUG: current_piece type = %d, x = %d, y = %d, rot = %d\n", 
        current_piece->type, current_piece->x, current_piece->y, current_piece->rotation);

    // Dessiner la grille
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            // Dessiner chaque bloc de la grille
            if (state->grid[y][x] != 0) {
                SDL_Rect block = {x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
                SDL_SetRenderDrawColor(renderer, 
                                        COLORS[state->grid[y][x]].r, 
                                       COLORS[state->grid[y][x]].g, 
                                       COLORS[state->grid[y][x]].b, 
                                       255);
                SDL_RenderFillRect(renderer, &block);
                
                // Dessiner un contour plus clair pour chaque bloc
                SDL_SetRenderDrawColor(renderer, 
                                       COLORS[state->grid[y][x]].r + 40, 
                                       COLORS[state->grid[y][x]].g + 40, 
                                       COLORS[state->grid[y][x]].b + 40, 
                                       255);
                SDL_RenderDrawRect(renderer, &block);
            }
        }
    }
    
    // Dessiner la pièce actuelle
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (TETROMINOS[current_piece->type][current_piece->rotation][i][j] != 0) {
                int x = (current_piece->x + j) * BLOCK_SIZE;
                int y = (current_piece->y + i) * BLOCK_SIZE;
                
                if (current_piece->y + i >= 0) {  // Ne pas dessiner au-dessus de l'écran
                    SDL_Rect block = {x, y, BLOCK_SIZE, BLOCK_SIZE};
                    int color_index = TETROMINOS[current_piece->type][current_piece->rotation][i][j];
                    
                    SDL_SetRenderDrawColor(renderer, 
                                        COLORS[color_index].r, 
                                        COLORS[color_index].g, 
                                        COLORS[color_index].b, 
                                        255);
                    SDL_RenderFillRect(renderer, &block);
                    
                    // Contour plus clair
                    SDL_SetRenderDrawColor(renderer, 
                                        COLORS[color_index].r + 40, 
                                        COLORS[color_index].g + 40, 
                                        COLORS[color_index].b + 40, 
                                        255);
                    SDL_RenderDrawRect(renderer, &block);
                }
            }
        }
    }
    
    // Afficher le résultat
    SDL_RenderPresent(renderer);
}
