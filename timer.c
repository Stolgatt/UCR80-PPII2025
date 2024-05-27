#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

int main() {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("Timer SDL2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Color textColor = { 255, 255, 255, 255 };

    // Initialiser SDL_ttf
    TTF_Init();

    // Charger la police de caractères avec une taille différente (par exemple, 36 points)
    TTF_Font* font = TTF_OpenFont("assets/police/digital-timer.ttf", 36);
    if (!font) {
        printf("Erreur: impossible de charger la police de caractères.\n");
        return 1;
    }

    // Charger la texture de la minimap
    SDL_Surface* minimapSurface = SDL_LoadBMP("assets/maps/map.bmp");
    if (!minimapSurface) {
        printf("Erreur: impossible de charger la texture de la minimap.\n");
        return 1;
    }
    SDL_Texture* minimapTexture = SDL_CreateTextureFromSurface(renderer, minimapSurface);
    SDL_FreeSurface(minimapSurface);

    int startTime = SDL_GetTicks(); // Temps au démarrage
    int elapsedTime = 0; // Temps écoulé

    SDL_Event event;
    int quit = 0;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Calculer le temps écoulé en minutes et secondes
        int minutes = elapsedTime / 60000;
        int seconds = (elapsedTime % 60000) / 1000;

        // Afficher le temps écoulé sous forme "mm:ss"
        char timerText[20];
        snprintf(timerText, sizeof(timerText), "%02d:%02d", minutes, seconds);
        SDL_Surface* surface = TTF_RenderText_Solid(font, timerText, textColor);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        SDL_Rect textRect = { 10, 10, 0, 0 };
        SDL_QueryTexture(texture, NULL, NULL, &textRect.w, &textRect.h);
        SDL_RenderCopy(renderer, texture, NULL, &textRect);
        SDL_DestroyTexture(texture);

        // Dessiner la minimap en bas à gauche de l'écran
        SDL_Rect minimapRect = { 10, 600 - 200, 200, 200 }; // Position et taille de la minimap
        SDL_RenderCopy(renderer, minimapTexture, NULL, &minimapRect);

        SDL_RenderPresent(renderer);

        // Attendre quelques millisecondes pour réduire l'utilisation du CPU
        SDL_Delay(10);

        // Mettre à jour le temps écoulé
        elapsedTime = SDL_GetTicks() - startTime;
    }

    // Libérer la police de caractères, la texture de la minimap et SDL_ttf
    TTF_CloseFont(font);
    SDL_DestroyTexture(minimapTexture);
    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
