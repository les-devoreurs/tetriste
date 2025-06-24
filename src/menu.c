#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

const char* menu_items[] = {"Jeu Classique", "Jeu Duel","Scores", "Quitter"};
const int MENU_ITEMS_COUNT = 4;

void render_menu(SDL_Renderer* renderer, TTF_Font* font, int selected_item) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (int i = 0; i < MENU_ITEMS_COUNT; i++) {
        SDL_Color color = (i == selected_item) ? (SDL_Color){255, 255, 255, 255} : (SDL_Color){128, 128, 128, 255};
        SDL_Surface* surface = TTF_RenderText_Solid(font, menu_items[i], color);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

        int text_width = surface->w;
        int text_height = surface->h;
        SDL_FreeSurface(surface);

        SDL_Rect dest_rect = {300 - text_width / 2, 200 + i * 50, text_width, text_height};
        SDL_RenderCopy(renderer, texture, NULL, &dest_rect);
        SDL_DestroyTexture(texture);
    }

    SDL_RenderPresent(renderer);
}

int menu(SDL_Window* window, SDL_Renderer* renderer) {
    if (TTF_Init() == -1) {
        SDL_Log("Erreur d'initialisation de TTF : %s", TTF_GetError());
        return -1;
    }

    TTF_Font* font = TTF_OpenFont("src/Tetris.ttf", 24);
    if (!font) {
        SDL_Log("Erreur de chargement de la police : %s", TTF_GetError());
        TTF_Quit();
        return -1;
    }

    bool running = true;
    int selected_item = 0;

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
                selected_item = 2; // Quitter
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_UP) {
                    selected_item = (selected_item - 1 + MENU_ITEMS_COUNT) % MENU_ITEMS_COUNT;
                } else if (event.key.keysym.sym == SDLK_DOWN) {
                    selected_item = (selected_item + 1) % MENU_ITEMS_COUNT;
                } else if (event.key.keysym.sym == SDLK_RETURN) {
                    running = false;
                }
            }
        }

        render_menu(renderer, font, selected_item);
        SDL_Delay(16); // éviter une boucle trop rapide
    }

    TTF_CloseFont(font);
    TTF_Quit();
    return selected_item;
}
