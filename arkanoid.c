#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define GRID_WIDTH 10
#define GRID_HEIGHT 20
#define BLOCK_SIZE 30
#define SCREEN_WIDTH (GRID_WIDTH * BLOCK_SIZE)
#define SCREEN_HEIGHT (GRID_HEIGHT * BLOCK_SIZE)

// Couleurs des tetrominos (R,G,B)
const SDL_Color COLORS[] = {
    {0, 0, 0},       // Noir (vide)
    {0, 255, 255},   // Cyan (I)
    {0, 0, 255},     // Bleu (J)
    {255, 165, 0},   // Orange (L)
    {255, 255, 0},   // Jaune (O)
    {0, 255, 0},     // Vert (S)
    {128, 0, 128},   // Violet (T)
    {255, 0, 0}      // Rouge (Z)
};

// Définition des formes de tetrominos
const int TETROMINOS[7][4][4][4] = {
    // I
    {
        {{0,0,0,0}, {1,1,1,1}, {0,0,0,0}, {0,0,0,0}}, // cette ligne représente la barre horizontale
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

// Structure pour la pièce courante
typedef struct {
    int x, y;            // Position dans la grille
    int type;           // Type de tetromino (0-6)
    int rotation;       // Rotation actuelle (0-3)
} Tetromino;

// Variables globales
SDL_Window* pWindow = NULL;
SDL_Renderer* renderer = NULL;
int grid[GRID_HEIGHT][GRID_WIDTH] = {0};  // Grille de jeu (0 = vide)
Tetromino current_piece;
bool game_over = false;
int score = 0;
int level = 1;
int lines_cleared = 0;
Uint32 last_drop_time;
int drop_speed = 1000;  // Temps en ms entre chaque descente automatique

// Prototypes de fonctions
void init();
void clean_up();
void create_new_piece();
bool check_collision(int x, int y, int type, int rotation);
void place_piece();
void draw();
void clear_lines();
void update_game();
void handle_input(SDL_Event event, bool* quit);

int main(int argc, char** argv)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Erreur SDL_Init : %s\n", SDL_GetError());
        return 1;
    }
    
    srand(time(NULL));  // Initialiser le générateur de nombres aléatoires
    init();
    
    create_new_piece();
    last_drop_time = SDL_GetTicks();
    
    bool quit = false;
    while (!quit && !game_over) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            handle_input(event, &quit);
        }
        
        update_game();
        draw();
        SDL_Delay(16);  // ~60 FPS
    }
    
    if (game_over) {
        printf("Game Over! Score: %d\n", score);
        SDL_Delay(2000);  // Attendre 2 secondes avant de quitter
    }
    
    clean_up();
    return 0;
}

void init()
{
    pWindow = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                               SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!pWindow) {
        printf("Erreur création fenêtre : %s\n", SDL_GetError());
        exit(1);
    }
    
    renderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Erreur création renderer : %s\n", SDL_GetError());
        exit(1);
    }
    
    // Initialiser la grille avec des zéros (vide)
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            grid[y][x] = 0;
        }
    }
}

void clean_up()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(pWindow);
    SDL_Quit();
}

void create_new_piece()
{
    current_piece.type = rand() % 7;
    current_piece.rotation = 0;
    current_piece.x = GRID_WIDTH / 2 - 2;
    current_piece.y = 0;
    
    // Vérifier si la nouvelle pièce peut être placée
    if (check_collision(current_piece.x, current_piece.y, current_piece.type, current_piece.rotation)) {
        game_over = true;
    }
}

bool check_collision(int x, int y, int type, int rotation)
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            // Si la cellule est occupée par la pièce
            if (TETROMINOS[type][rotation][i][j] != 0) {
                int grid_x = x + j;
                int grid_y = y + i;
                
                // Vérifier les limites de la grille
                if (grid_x < 0 || grid_x >= GRID_WIDTH || grid_y >= GRID_HEIGHT) {
                    return true;  // Collision avec les bords
                }
                
                // Vérifier la collision avec les pièces déjà placées
                if (grid_y >= 0 && grid[grid_y][grid_x] != 0) {
                    return true;  // Collision avec une pièce existante
                }
            }
        }
    }
    return false;  // Pas de collision
}

void place_piece()
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (TETROMINOS[current_piece.type][current_piece.rotation][i][j] != 0) {
                int grid_x = current_piece.x + j;
                int grid_y = current_piece.y + i;
                
                if (grid_y >= 0 && grid_y < GRID_HEIGHT && grid_x >= 0 && grid_x < GRID_WIDTH) {
                    // +1 car 0 est réservé pour les cases vides
                    grid[grid_y][grid_x] = TETROMINOS[current_piece.type][current_piece.rotation][i][j];
                }
            }
        }
    }
    
    clear_lines();
    create_new_piece();
}

void clear_lines()
{
    int lines = 0;
    
    for (int y = GRID_HEIGHT - 1; y >= 0; y--) {
        bool line_full = true;
        
        // Vérifier si la ligne est complète
        for (int x = 0; x < GRID_WIDTH; x++) {
            if (grid[y][x] == 0) {
                line_full = false;
                break;
            }
        }
        
        if (line_full) {
            lines++;
            
            // Déplacer toutes les lignes au-dessus vers le bas
            for (int y2 = y; y2 > 0; y2--) {
                for (int x = 0; x < GRID_WIDTH; x++) {
                    grid[y2][x] = grid[y2-1][x];
                }
            }
            
            // Effacer la première ligne
            for (int x = 0; x < GRID_WIDTH; x++) {
                grid[0][x] = 0;
            }
            
            // La ligne y a été supprimée et remplacée, on doit la revérifier
            y++;
        }
    }
    
    // Mettre à jour le score
    if (lines > 0) {
        // Système de score classique de Tetris (plus de points pour plus de lignes)
        int points[] = {0, 40, 100, 300, 1200};
        score += points[lines] * level;
        lines_cleared += lines;
        
        // Augmenter le niveau tous les 10 lignes
        level = 1 + (lines_cleared / 10);
        
        // Ajuster la vitesse en fonction du niveau
        drop_speed = 1000 - (level - 1) * 50;
        if (drop_speed < 100) drop_speed = 100;  // Vitesse minimale
    }
}

void draw()
{
    // Effacer l'écran (noir)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    // Dessiner la grille
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            // Dessiner chaque bloc de la grille
            if (grid[y][x] != 0) {
                SDL_Rect block = {x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
                SDL_SetRenderDrawColor(renderer, 
                                       COLORS[grid[y][x]].r, 
                                       COLORS[grid[y][x]].g, 
                                       COLORS[grid[y][x]].b, 
                                       255);
                SDL_RenderFillRect(renderer, &block);
                
                // Dessiner un contour plus clair pour chaque bloc
                SDL_SetRenderDrawColor(renderer, 
                                       COLORS[grid[y][x]].r + 40, 
                                       COLORS[grid[y][x]].g + 40, 
                                       COLORS[grid[y][x]].b + 40, 
                                       255);
                SDL_RenderDrawRect(renderer, &block);
            }
        }
    }
    
    // Dessiner la pièce actuelle
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (TETROMINOS[current_piece.type][current_piece.rotation][i][j] != 0) {
                int x = (current_piece.x + j) * BLOCK_SIZE;
                int y = (current_piece.y + i) * BLOCK_SIZE;
                
                if (current_piece.y + i >= 0) {  // Ne pas dessiner au-dessus de l'écran
                    SDL_Rect block = {x, y, BLOCK_SIZE, BLOCK_SIZE};
                    int color_index = TETROMINOS[current_piece.type][current_piece.rotation][i][j];
                    
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

void update_game()
{
    Uint32 current_time = SDL_GetTicks();
    
    // Vérifier s'il est temps de faire descendre la pièce
    if (current_time - last_drop_time > drop_speed) {
        // Essayer de déplacer la pièce vers le bas
        if (!check_collision(current_piece.x, current_piece.y + 1, current_piece.type, current_piece.rotation)) {
            current_piece.y++;
        } else {
            // Si on ne peut pas descendre, placer la pièce dans la grille
            place_piece();
        }
        last_drop_time = current_time;
    }
}

void handle_input(SDL_Event event, bool* quit)
{
    switch (event.type) {
        case SDL_QUIT:
            *quit = true;
            break;
        
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
                case SDLK_LEFT:
                    // Déplacer à gauche si possible
                    if (!check_collision(current_piece.x - 1, current_piece.y, current_piece.type, current_piece.rotation)) {
                        current_piece.x--;
                    }
                    break;
                
                case SDLK_RIGHT:
                    // Déplacer à droite si possible
                    if (!check_collision(current_piece.x + 1, current_piece.y, current_piece.type, current_piece.rotation)) {
                        current_piece.x++;
                    }
                    break;
                
                case SDLK_DOWN:
                    // Accélération vers le bas
                    if (!check_collision(current_piece.x, current_piece.y + 1, current_piece.type, current_piece.rotation)) {
                        current_piece.y++;
                        score++; // Petit bonus pour descente rapide
                    }
                    break;
                
                case SDLK_UP:
                    // Rotation
                    int new_rotation = (current_piece.rotation + 1) % 4;
                    if (!check_collision(current_piece.x, current_piece.y, current_piece.type, new_rotation)) {
                        current_piece.rotation = new_rotation;
                    }
                    break;
                
                case SDLK_SPACE:
                    // Hard drop: faire chuter la pièce instantanément
                    while (!check_collision(current_piece.x, current_piece.y + 1, current_piece.type, current_piece.rotation)) {
                        current_piece.y++;
                        score += 2; // Bonus plus important pour le hard drop
                    }
                    place_piece();
                    break;
                
                case SDLK_ESCAPE:
                    *quit = true;
                    break;
                
                default:
                    break;
            }
            break;
        
        default:
            break;
    }
}