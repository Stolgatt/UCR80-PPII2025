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

	SDL_Surface* bmp_test = SDL_LoadBMP("sample.bmp");
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, bmp_test);
	SDL_FreeSurface(bmp_test);
	SDL_Rect src;
	src.x = 0;
	src.y = 0;
	src.w = 1920;
	src.h = 1280;
	SDL_Rect dst;
	dst.x = TX/2 - 25;
	dst.y = TY/2 - 25;
	dst.w = 50;
	dst.h = 50;
	const double angle = 90;
	SDL_Point center;
	center.x = 100;
	center.y = 100;

	/*CAMERA cam;
	cam.N_MAX = 10000;
	cam.tableau_z = calloc(10000,sizeof(Z_SPRITE));

	cam.position.x = 0.;
	cam.position.y = 0.;
	cam.position.z = 0.;
	cam.longitude = 0.;
	cam.latitude = 0.;
	cam.roulis = 0.;
	cam.ecran_final.x = 0;
	cam.ecran_final.y = 0;
	cam.ecran_final.w = TX;
	cam.ecran_final.h = TY;
	cam.distance_ecran = 50.;
	cam.min_largeur = -25.;
	cam.max_largeur = +25.;
	cam.min_hauteur = ;
	cam.

	SCENE scene;*/

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

		SDL_RenderClear(renderer);
		//AFFICHAGE_CAMERA(&cam, &scene);
		SDL_RenderCopyEx(renderer,texture,&src,&dst,0,NULL,SDL_FLIP_NONE);
		SDL_RenderCopyEx(renderer,texture,&src,&dst,angle,&center,SDL_FLIP_NONE);
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

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	exit(0);
}