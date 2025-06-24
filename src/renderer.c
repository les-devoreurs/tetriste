#include "renderer.h"
#include "grid.h"
#include "tetromino.h"
#include "game.h"

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL_ttf.h>

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

SDL_Renderer* temp_renderer = NULL;  

void render_text(SDL_Renderer* renderer, const char* text, int x, int y, SDL_Color color, TTF_Font* font) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect dest = { x, y, surface->w, surface->h };
    SDL_RenderCopy(renderer, texture, NULL, &dest);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}


void init_renderer(SDL_Window** window, SDL_Renderer** renderer)
{
    *window = SDL_CreateWindow("Tetriste", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    
    if (!*window){
        printf("Erreur création fenêtre : %s\n", SDL_GetError());
        exit(1);
    }
    
    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    
    if (!*renderer) {
        printf("Erreur création renderer : %s\n", SDL_GetError());
        exit(1);
    }

    temp_renderer = *renderer; 
}

void clean_renderer(SDL_Window* window, SDL_Renderer* renderer){

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

}

void draw_on_renderer(SDL_Renderer* renderer, GameState* state, TTF_Font* font){

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); //fond noir
    SDL_RenderClear(renderer);

    Tetromino* current_piece = &state->current_piece;

    // Dessiner la grille
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {

            // Dessiner chaque bloc de la grille
            if (state->grid[y][x] != 0) {
                SDL_Rect block = {x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};

                SDL_SetRenderDrawColor(renderer,COLORS[state->grid[y][x]].r, COLORS[state->grid[y][x]].g, COLORS[state->grid[y][x]].b, 255);
                SDL_RenderFillRect(renderer, &block);
                
                // Dessiner un contour plus clair pour chaque bloc
                SDL_SetRenderDrawColor(renderer, COLORS[state->grid[y][x]].r + 40, COLORS[state->grid[y][x]].g + 40, COLORS[state->grid[y][x]].b + 40, 255);
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
                    int color_index = current_piece->type+1;
                    
                    SDL_SetRenderDrawColor(renderer, COLORS[color_index].r, COLORS[color_index].g, COLORS[color_index].b, 255);
                    SDL_RenderFillRect(renderer, &block);
                    
                    // Contour plus clair
                    SDL_SetRenderDrawColor(renderer, COLORS[color_index].r + 40,COLORS[color_index].g + 40,COLORS[color_index].b + 40, 255);
                    SDL_RenderDrawRect(renderer, &block);
                }
            }
        }
    }
    char buffer[64];
    SDL_Color white = {255, 255, 255, 255};

    int separator_x = GRID_WIDTH * BLOCK_SIZE;  // à droite de la grille

    int text_x = separator_x + 20;  // 20px droite de la ligne verticale

    // texte
    sprintf(buffer, "Score: %d", state->score);
    render_text(renderer, buffer, text_x, 50, white, font);
    
    sprintf(buffer, "Niveau: %d", state->level);
    render_text(renderer, buffer, text_x, 100, white, font);
    
    sprintf(buffer, "Lignes: %d", state->lines_cleared);
    render_text(renderer, buffer, text_x, 150, white, font);
    

    //ligne verticale fine (1 pixel)
    SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
    SDL_RenderDrawLine(renderer, separator_x, 0, separator_x, SCREEN_HEIGHT);
    
    //zone d'info
    SDL_Rect info_area = {separator_x + 1, 0, SCREEN_WIDTH - (separator_x + 1), SCREEN_HEIGHT};
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_RenderFillRect(renderer, &info_area);
    
    SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
    SDL_RenderDrawLine(renderer, separator_x, 0, separator_x, SCREEN_HEIGHT);
    
    SDL_RenderPresent(renderer);
}
