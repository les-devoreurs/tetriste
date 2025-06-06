#include <SDL2/SDL.h>
#include "input.h"
#include "game.h"  


// Fonction pour gérer les entrées du joueur
void handle_input(SDL_Event* event, bool* quit, GameState* state) {

    //printf("handle_input: current x=%d y=%d\n", state->current_piece.x, state->current_piece.y);
    
    // Boucle sur les événements clavier
    while (SDL_PollEvent(event)) {

        switch (event->type) {
            case SDL_QUIT:          // Si l'utilisateur ferme la fenêtre
                *quit = true;
                break;
            
            case SDL_KEYDOWN:       // Si une touche est pressée
                switch (event->key.keysym.sym) {
                    case SDLK_LEFT: // Déplacement vers la gauche
                        move_tetromino(state, -1, 0);
                        break;

                    case SDLK_RIGHT: // Déplacement vers la droite
                        move_tetromino(state, 1, 0);
                        break;

                    case SDLK_DOWN: // Descente de la pièce
                        move_tetromino(state, 0, 1);
                        break;

                    case SDLK_UP: // Rotation de la pièce
                        rotate_tetromino(state);
                        break;

                    case SDLK_SPACE: // Hard drop
                        hard_drop(state);
                        break;

                    case SDLK_ESCAPE: // Quitter le jeu
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
}
