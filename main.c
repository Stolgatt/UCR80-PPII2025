#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#include "engine_common.h"
#include "graphical_engine.h"

#define TX 1080
#define TY 540

int main() {

	SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER);
	SDL_Window* window = SDL_CreateWindow("graphical engine test", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, TX,TY, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
	SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );

	SDL_Surface* bmp_test = SDL_LoadBMP("sample2.bmp");
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, bmp_test);
	SDL_FreeSurface(bmp_test);

	CAMERA cam;
	cam.N_MAX = 10000;
	cam.tableau_d = calloc(10000,sizeof(D_SPRITE));
	cam.position.x = 0.;
	cam.position.y = 0.;
	cam.position.z = -1000;
	cam.longitude = 0.;
	cam.latitude = 0.;
	cam.roulis = 0.;
	cam.renderer = renderer;
	cam.tmp_text = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA32,SDL_TEXTUREACCESS_TARGET,16000,16000);
	cam.tmp_cible = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA32,SDL_TEXTUREACCESS_TARGET,TX,TX);
	cam.cible = NULL;
	cam.dimension_cible.x = 0; cam.dimension_cible.y = 0;
	cam.dimension_cible.w = TX; cam.dimension_cible.h = TY;
	cam.echelle_ecran = 0.05;
	cam.distance_ecran = 50.;
	cam.offset_horizontal = cam.offset_vertical = 0.;

	PLAN_HORIZONTAL plan;
	plan.texture = texture;
	plan.rotation = 0.;
	plan.echelle = 1.;
	plan.position.x = plan.position.z = 0.;
	plan.position.y = 3000;
	plan.au_dessus = plan.en_dessous = plan.sprites_au_dessus = NULL;
	plan.source.x = plan.source.y = 0;
	plan.source.w = 3000; // 1920;
	plan.source.h = 2172; // 1280;

	SCENE scene;
	scene.sprites_tout_en_bas = NULL;
	scene.tout_en_bas = scene.tout_en_haut = &plan;

	enum {UP=0,DOWN=1,LEFT=2,RIGHT=3};
	int INPUT[4] = {0};
	SDL_Event EVENT;
	int loop = 1;
	int fps = 0;
	long long temps_fps = SDL_GetTicks();
	while (loop) {

		while( SDL_PollEvent(&EVENT) != 0 ) {
	        if (EVENT.type == SDL_QUIT) {
                loop = 0;
            }
            else if (EVENT.type == SDL_KEYDOWN) {
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
					default:
						break;
				}
			}
			else if (EVENT.type == SDL_KEYUP) {
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
					default:
						break;
				}
			}
		}

		if (INPUT[UP]) cam.latitude += 0.05;
		if (INPUT[DOWN]) cam.latitude -= 0.05;
		if (INPUT[LEFT]) cam.longitude += 0.05;
		if (INPUT[RIGHT]) cam.longitude -= 0.05;
		cam.latitude = cam.latitude > M_PI/2 ? M_PI/2 : cam.latitude;
		cam.latitude = cam.latitude < -M_PI/2 ? -M_PI/2 : cam.latitude;

		// printf("latitude %f\n",cam.latitude);
		SDL_RenderClear(renderer);
		AFFICHAGE_CAMERA(&cam, &scene);
		SDL_RenderPresent(renderer);

		SDL_Delay(16);
		if (SDL_GetTicks() - temps_fps >= 1000) {
			printf("FPS: %d\n",fps);
			temps_fps = SDL_GetTicks();
			fps = 0;
		}
		else {
			fps++;
		}
	}

	SDL_DestroyTexture(cam.tmp_text);
	SDL_DestroyTexture(cam.tmp_cible);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	exit(0);
}