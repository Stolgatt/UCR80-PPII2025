#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "engine_common.h"
#include "graphical_engine.h"

#define TX 1920
#define TY 1080

typedef struct {
    float x, y;
    SDL_Texture* texture;
    SDL_Rect rect;
} Car;

void show_start_screen(SDL_Renderer* renderer) { 
    SDL_Surface* start_surface = SDL_LoadBMP("Page_Acceuil.bmp");
    if (!start_surface) {
        printf("Impossible de charger la page d'acceuil: %s\n", SDL_GetError());
        return;
    }
    SDL_Texture* start_texture = SDL_CreateTextureFromSurface(renderer, start_surface);
    SDL_FreeSurface(start_surface);

    int spriteFullWidth, spriteFullHeight;
    if (SDL_QueryTexture(start_texture, NULL, NULL, &spriteFullWidth, &spriteFullHeight) != 0) {
        printf("Unable to query texture: %s\n", SDL_GetError());
        return;
    }

    int spriteWidth = spriteFullWidth / 3;
    int spriteHeight = spriteFullHeight / 2;
    int offsetX = 0;
    int offsetY = 0;

    SDL_Rect spriteRect = { .x = offsetX * spriteWidth, .y = offsetY * spriteHeight, .w = spriteWidth, .h = spriteHeight };
    SDL_Rect destRect = { .x = 0, .y = 0, .w = TX, .h = TY };

    SDL_RenderClear(renderer);
    if (SDL_RenderCopy(renderer, start_texture, &spriteRect, &destRect) != 0) {
        printf("Unable to render texture: %s\n", SDL_GetError());
    }
    SDL_RenderPresent(renderer);

    SDL_DestroyTexture(start_texture);
}

void show_level_selection(SDL_Renderer* renderer) {
    SDL_Surface* start_surface = SDL_LoadBMP("Select_Map.bmp");
    if (!start_surface) {
        printf("Impossible de charger la page de selection: %s\n", SDL_GetError());
        return;
    }
    SDL_Texture* start_texture = SDL_CreateTextureFromSurface(renderer, start_surface);
    SDL_FreeSurface(start_surface);

    int spriteFullWidth, spriteFullHeight;
    if (SDL_QueryTexture(start_texture, NULL, NULL, &spriteFullWidth, &spriteFullHeight) != 0) {
        printf("Unable to query texture: %s\n", SDL_GetError());
        return;
    }

    int spriteWidth = spriteFullWidth / 4;
    int spriteHeight = spriteFullHeight / 2;
    int offsetX = 0;
    int offsetY = 0;

    SDL_Rect spriteRect = { .x = offsetX * spriteWidth, .y = offsetY * spriteHeight, .w = spriteWidth, .h = spriteHeight };
    SDL_Rect destRect = { .x = 0, .y = 0, .w = TX, .h = TY };

    SDL_RenderClear(renderer);
    if (SDL_RenderCopy(renderer, start_texture, &spriteRect, &destRect) != 0) {
        printf("Unable to render texture: %s\n", SDL_GetError());
    }
    SDL_RenderPresent(renderer);

    SDL_DestroyTexture(start_texture);
}

int main() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("graphical engine test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, TX, TY, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
    if (!renderer) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    show_start_screen(renderer);

    SDL_Rect clickZone1 = { .x = 827, .y = 138, .w = 1115, .h = 271 };
    SDL_Rect clickZone2 = { .x = 827, .y = 813, .w = 1115, .h = 945 };
    SDL_Rect clickZone3 = { .x = 131, .y = 403, .w = 494, .h = 809 };
    SDL_Rect clickZone4 = { .x = 1644, .y = 933, .w = 1799, .h = 1004 };

    SDL_Event event;
    int start_game = 0;
    while (start_game != 2) {
        while (SDL_PollEvent(&event) && start_game == 0) {
            if (event.type == SDL_QUIT) {
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                SDL_Quit();
                return 0;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX = event.button.x;
                int mouseY = event.button.y;
                if (mouseX >= clickZone1.x && mouseX <= clickZone1.w &&
                    mouseY >= clickZone1.y && mouseY <= clickZone1.h) {
                    show_level_selection(renderer);
                    start_game = 1;
                }
                else if (mouseX >= clickZone2.x && mouseX <= clickZone2.w &&
                    mouseY >= clickZone2.y && mouseY <= clickZone2.h) {
                    show_level_selection(renderer);
                    start_game = 1;
                }
            }
        }
        while (SDL_PollEvent(&event) && start_game == 1) {
            if (event.type == SDL_QUIT) {
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                SDL_Quit();
                return 0;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX = event.button.x;
                int mouseY = event.button.y;
                if (mouseX >= clickZone3.x && mouseX <= clickZone3.w &&
                    mouseY >= clickZone3.y && mouseY <= clickZone3.h) {
                    start_game = 2;
                }
                else if (mouseX >= clickZone4.x && mouseX <= clickZone4.w &&
                    mouseY >= clickZone4.y && mouseY <= clickZone4.h) {
                    show_start_screen(renderer);
                    start_game = 0;
                }
            }
        }
    }

    SDL_Surface* bmp_test = SDL_LoadBMP("map.bmp");
    if (!bmp_test) {
        printf("Unable to load map.bmp: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, bmp_test);
    SDL_FreeSurface(bmp_test);

    SDL_Surface* car_surface = SDL_LoadBMP("car.bmp");
    
    SDL_Texture* car_texture = SDL_CreateTextureFromSurface(renderer, car_surface);
    SDL_FreeSurface(car_surface);

    CAMERA cam;
    cam.N_MAX = 10000;
    cam.tableau_z = calloc(cam.N_MAX, sizeof(Z_SPRITE));
    cam.tableau_p = calloc(cam.N_MAX, sizeof(SPRITE_PROJETE));
    cam.position.x = 0.;
    cam.position.y = 0.;
    cam.position.z = 50;
    cam.longitude = 0.;
    cam.latitude = 0.;
    cam.roulis = 0.;
    cam.renderer = renderer;
    cam.tmp_text = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, 4000, 4000);
    cam.tmp_cible = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, 1.5 * TX, 1.5 * TX);
    cam.cible = NULL;
    cam.dimension_cible.x = 0;
    cam.dimension_cible.y = 0;
    cam.dimension_cible.w = TX;
    cam.dimension_cible.h = TY;
    cam.echelle_ecran = 0.025;
    cam.distance_ecran = 10.;
    cam.offset_horizontal = cam.offset_vertical = 0.;

    PLAN_HORIZONTAL plan;
    plan.texture = texture;
    plan.rotation = 0.;
    plan.echelle = 1.;
    plan.position.x = plan.position.z = 0.;
    plan.position.y = 0;
    plan.au_dessus = plan.en_dessous = plan.sprites_au_dessus = NULL;
    plan.source.x = plan.source.y = 0;
    plan.source.w = 1600;
    plan.source.h = 3000;

    SCENE scene;
    scene.sprites_tout_en_bas = NULL;
    scene.tout_en_bas = scene.tout_en_haut = &plan;

    Car player_car;
    player_car.x = 0;
    player_car.y = 0;
    player_car.texture = car_texture;
    SDL_QueryTexture(car_texture, NULL, NULL, &player_car.rect.w, &player_car.rect.h);
    player_car.rect.x = (int)player_car.x;
    player_car.rect.y = (int)player_car.y;

    enum { UP = 0, DOWN, LEFT, RIGHT, Z, Q, S, D, A, E, W, X };
    int INPUT[12] = { 0 };
    SDL_Event EVENT;
    int loop = 1;
    int fps = 0;
    long long temps_fps = SDL_GetTicks();
    float speed = 5.0;

    while (loop) {
        while (SDL_PollEvent(&EVENT) != 0) {
            if (EVENT.type == SDL_QUIT) {
                loop = 0;
            } else if (EVENT.type == SDL_KEYDOWN) {
                switch (EVENT.key.keysym.sym) {
                    case SDLK_UP:
                        INPUT[UP] = 1;
                        break;
                    case SDLK_DOWN:
                        INPUT[DOWN] = 1;
                        break;
                    case SDLK_LEFT:
                        INPUT[LEFT] = 1;
                        break;
                    case SDLK_RIGHT:
                        INPUT[RIGHT] = 1;
                        break;
                    case SDLK_z:
                        INPUT[Z] = 1;
                        break;
                    case SDLK_q:
                        INPUT[Q] = 1;
                        break;
                    case SDLK_s:
                        INPUT[S] = 1;
                        break;
                    case SDLK_d:
                        INPUT[D] = 1;
                        break;
                    case SDLK_a:
                        INPUT[A] = 1;
                        break;
                    case SDLK_e:
                        INPUT[E] = 1;
                        break;
                    case SDLK_w:
                        INPUT[W] = 1;
                        break;
                    case SDLK_x:
                        INPUT[X] = 1;
                        break;
                    default:
                        break;
                }
            } else if (EVENT.type == SDL_KEYUP) {
                switch (EVENT.key.keysym.sym) {
                    case SDLK_UP:
                        INPUT[UP] = 0;
                        break;
                    case SDLK_DOWN:
                        INPUT[DOWN] = 0;
                        break;
                    case SDLK_LEFT:
                        INPUT[LEFT] = 0;
                        break;
                    case SDLK_RIGHT:
                        INPUT[RIGHT] = 0;
                        break;
                    case SDLK_z:
                        INPUT[Z] = 0;
                        break;
                    case SDLK_q:
                        INPUT[Q] = 0;
                        break;
                    case SDLK_s:
                        INPUT[S] = 0;
                        break;
                    case SDLK_d:
                        INPUT[D] = 0;
                        break;
                    case SDLK_a:
                        INPUT[A] = 0;
                        break;
                    case SDLK_e:
                        INPUT[E] = 0;
                        break;
                    case SDLK_w:
                        INPUT[W] = 0;
                        break;
                    case SDLK_x:
                        INPUT[X] = 0;
                        break;
                    default:
                        break;
                }
            }
        }

        if (INPUT[UP]) player_car.y -= speed;
        if (INPUT[DOWN]) player_car.y += speed;
        if (INPUT[LEFT]) player_car.x -= speed;
        if (INPUT[RIGHT]) player_car.x += speed;
        player_car.rect.x = (int)player_car.x;
        player_car.rect.y = (int)player_car.y;

        if (INPUT[UP]) cam.latitude += 0.05;
        if (INPUT[DOWN]) cam.latitude -= 0.05;
        if (INPUT[LEFT]) cam.longitude += 0.05;
        if (INPUT[RIGHT]) cam.longitude -= 0.05;
        if (INPUT[W]) plan.rotation += 0.05;
        if (INPUT[X]) plan.rotation -= 0.05;
        cam.latitude = cam.latitude > M_PI / 2 ? M_PI / 2 : cam.latitude;
        cam.latitude = cam.latitude < -M_PI / 2 ? -M_PI / 2 : cam.latitude;
        if (INPUT[Z]) cam.position.y += 5;
        if (INPUT[S]) cam.position.y -= 5;
        if (INPUT[D]) cam.position.x += 5;
        if (INPUT[Q]) cam.position.x -= 5;
        if (INPUT[E]) cam.position.z += 5;
        if (INPUT[A]) cam.position.z -= 5;

        SDL_RenderClear(renderer);
        AFFICHAGE_CAMERA(&cam, &scene);
        SDL_RenderCopy(renderer, player_car.texture, NULL, &player_car.rect);
        SDL_RenderPresent(renderer);

        SDL_Delay(16);
        if (SDL_GetTicks() - temps_fps >= 1000) {
            printf("FPS: %d\n", fps);
            temps_fps = SDL_GetTicks();
            fps = 0;
        } else {
            fps++;
        }
    }

    SDL_DestroyTexture(car_texture);
    SDL_DestroyTexture(cam.tmp_text);
    SDL_DestroyTexture(cam.tmp_cible);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
