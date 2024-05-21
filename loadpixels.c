#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#define TX 1920
#define TY 1080

void print_pixel_info(SDL_Surface* surface, int x, int y) {
    // Vérifie si les coordonnées sont valides
    if (x < 0 || x >= surface->w || y < 0 || y >= surface->h) {
        printf("Invalid coordinates: (%d, %d)\n", x, y);
        return;
    }

    // Vérifie si la surface est verrouillée (elle ne devrait pas l'être)
    if (SDL_MUSTLOCK(surface)) {
        if (SDL_LockSurface(surface) < 0) {
            printf("Unable to lock surface: %s\n", SDL_GetError());
            return;
        }
    }

    int pitch = surface->pitch; // Nombre d'octets par ligne
    Uint32* pixels = (Uint32*)surface->pixels; // Pointeur vers les pixels

    // Récupère la couleur du pixel à (x, y)
    Uint32 pixel = pixels[(y * pitch / 4) + x];

    // Décompose la couleur en composantes RGBA
    Uint8 r, g, b, a;
    SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a);

    // Affiche la position et la valeur RGBA du pixel
    printf("Pixel at (%d, %d): R=%d, G=%d, B=%d, A=%d\n", x, y, r, g, b, a);

    // Déverrouille la surface si nécessaire
    if (SDL_MUSTLOCK(surface)) {
        SDL_UnlockSurface(surface);
    }
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("image pointeur", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, TX, TY, SDL_WINDOW_SHOWN); // Création de la fenêtre
    if (!window) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        SDL_DestroyWindow(window);
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Surface* image_surface = SDL_LoadBMP("Select_Map.bmp");
    if (!image_surface) {
        printf("SDL_LoadBMP Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Texture* image_texture = SDL_CreateTextureFromSurface(renderer, image_surface);

    int portionWidth = image_surface->w / 4; // Largeur de la portion à afficher
    int portionHeight = image_surface->h / 2; // Hauteur de la portion à afficher
    SDL_Rect portionRect = { .x = 0, .y = 0, .w = portionWidth, .h = portionHeight }; // Rectangle pour définir la portion à afficher

    int running = 1;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                print_pixel_info(image_surface, x, y);
            }
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, image_texture, &portionRect, NULL); // Affiche uniquement la portion de l'image définie par portionRect
        SDL_RenderPresent(renderer);
    }

    SDL_FreeSurface(image_surface);
    SDL_DestroyTexture(image_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
