#include "bot.h"
#include "grid.h"
#include "game.h"
#include <limits.h>


int evaluate_position(GameState* state) {
    int score = 0;
    int lines_cleared = 0;
    int holes = 0;
    int max_height = 0;
    
    // Calcul du nombre de lignes complètes
    for (int y = 0; y < GRID_HEIGHT; y++) {
        int full = 1;
        for (int x = 0; x < GRID_WIDTH; x++) {
            if (state->grid[y][x] == 0) {
                full = 0;
                break;
            }
        }
        if (full) lines_cleared++;
    }

    // Calcul du nombre de trous et de la hauteur maximale
    for (int x = 0; x < GRID_WIDTH; x++) {
        int block_found = 0;
        for (int y = 0; y < GRID_HEIGHT; y++) {
            if (state->grid[y][x]) {
                block_found = 1;
                if (GRID_HEIGHT - y > max_height) max_height = GRID_HEIGHT - y;
            } else if (block_found) {
                holes++;
            }
        }
    }

    // Calcul du score (pondération des critères)
    score += lines_cleared * 100;
    score -= holes * 50;
    score -= max_height * 10;

    return score;
}

void preview_place_piece(GameState* state, int type, int rotation, int x, int y) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            int block = TETROMINOS[type][rotation][i][j];
            
            if (block != 0) {
                int grid_x = state->current_piece.x + j;
                int grid_y = state->current_piece.y + i;
                
                if (grid_y >= 0 && grid_y < GRID_HEIGHT && grid_x >= 0 && grid_x < GRID_WIDTH) {
                    // +1 car 0 est réservé pour les cases vides
                    //state->grid[grid_y][grid_x] = TETROMINOS[state->current_piece.type][state->current_piece.rotation][i][j];
                    state->grid[grid_y][grid_x] = state->current_piece.type +1;
                }
            }
        }
    }
}

void copy_game_state(const GameState* src, GameState* copy) {
    
    copy->current_piece = src->current_piece;
    copy->next_piece = src->next_piece;
    copy->score = src->score;
    copy->level = src->level;
    copy->lines_cleared = src->lines_cleared;
    copy->game_over = src->game_over;
    copy->last_drop_time = src->last_drop_time;
    copy->drop_speed = src->drop_speed;

    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            copy->grid[y][x] = src->grid[y][x];
        }
    }
}

void move_piece(GameState* state, int rotation, int x, int y) {
    state->current_piece.rotation = rotation;
    state->current_piece.x = x;
    state->current_piece.y = y;
}

BotMove find_best_move(GameState* state) {

    
    BotMove best_move = {0, 0, 0, false};
    int best_score = INT_MIN;

    // Parcours des rotations possibles
    for (int rotation = 0; rotation < 4; rotation++) {
        // Parcours des positions horizontales possibles
        for (int x = -2; x < GRID_WIDTH; x++) {
            int y = 0;
            
            GameState preview;
            copy_game_state(state, &preview);
            // Trouver la position y où la pièce s'arrête
            while (!check_collision(&preview, x, y, preview.current_piece.type, rotation)) {
                y++;
            }
            y--;

            if (y < 0) continue; // Position invalide

            // Placer la pièce sur la grille temporaire dans la position (rotation, x, y)
            move_piece(&preview, rotation, x, y);
            place_piece(&preview);
            
            // Évaluer la position
            int score = evaluate_position(&preview);
            
            if (score > best_score) {
                best_score = score;
                best_move.x = x;
                best_move.y = y;
                best_move.rotation = rotation;
            }
        }
    }
    
    return best_move;
}

void apply_move_bot(GameState* state, BotMove* best_movement) {

    if (state->current_piece.rotation != best_movement->rotation)
    {
        rotate_tetromino(state);
    }

    if (state->current_piece.x != best_movement->x) {
        if (state->current_piece.x > best_movement->x) {
            move_tetromino(state, -1, 0);
        }
        else {
            move_tetromino(state, 1, 0);
        } 
    }

    if (state->current_piece.y != best_movement->y)
    {
        move_tetromino(state, 0, 1);
    }


    printf("y .. %u : %u \n", state->current_piece.y, best_movement->y);
    printf("x .. %u : %u \n", state->current_piece.x, best_movement->x);
    printf("r .. %u : %u \n", state->current_piece.rotation, best_movement->rotation);
    if (state->current_piece.y == best_movement->y
        && state->current_piece.x == best_movement->x
        && state->current_piece.rotation == best_movement->rotation
    ) {
        best_movement->done = true;
    }
}