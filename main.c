#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <math.h>

#include "graphical_engine.h"


#define TX 640
#define TY 480



void DrawCircle(SDL_Renderer* renderer, int32_t centreX, int32_t centreY, int32_t radius)
{
const int32_t diameter = (radius * 2);

int32_t x = (radius - 1);
int32_t y = 0;
int32_t tx = 1;
int32_t ty = 1;
int32_t error = (tx - diameter);

while (x >= y)
{
// Each of the following renders an octant of the circle
SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

  if (error <= 0)
  {
  	++y;
  	error += ty;
  	ty += 2;
  }

  if (error > 0)
  {
  	--x;
  	tx += 2;
  	error += (tx - diameter);
  }

}
}


int main() {

	SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER);
	SDL_Window* window = SDL_CreateWindow("graphical engine test", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, TX,TY, SDL_WINDOW_SHOWN);
	// SDL_Surface* screen = SDL_GetWindowSurface(window);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_NONE);

	const int N_disques = 3;
	VERTEX2D tab_centres[] = {{150,150},{10,10},{150,100}};
	float tab_rayons[] = {100,2,50};

	const int N_segments = 2;
	VERTEX2D tab_pos_seg[] = { {30,35}, {300,300} };
	SEGMENT2D tab_seg[] = { {{cos(M_PI/4),sin(M_PI/4)},60}, {{1,0},10} };

	float RAYON = 20;
	VERTEX2D POSITION = {50,50};

	enum {UP=0,DOWN=1,LEFT=2,RIGHT=3};
	int INPUT[4] = {0};
	SDL_Event EVENT;
	int loop = 1;
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

        if (INPUT[UP] && !INPUT[DOWN]) {
        	POSITION.y -= 1;
        }
        else if (INPUT[DOWN] && !INPUT[UP]) {
        	POSITION.y += 1;
        }
        if (INPUT[LEFT] && !INPUT[RIGHT]) {
        	POSITION.x -= 1;
        }
        else if (INPUT[RIGHT] && !INPUT[LEFT]) {
        	POSITION.x += 1;
        }

		SDL_SetRenderDrawColor(renderer,0,0,0,255);
		SDL_RenderClear(renderer);
		for (int i=0; i<N_disques; ++i) {
			if (INTERSECTION_DISQUES(POSITION.x,POSITION.y,RAYON, tab_centres[i].x,tab_centres[i].y,tab_rayons[i])) {
				SDL_SetRenderDrawColor(renderer,255,255,255,255);
				DrawCircle(renderer, tab_centres[i].x, tab_centres[i].y, tab_rayons[i]);
			}
			else {
				SDL_SetRenderDrawColor(renderer,255,0,0,255);
				DrawCircle(renderer, tab_centres[i].x, tab_centres[i].y, tab_rayons[i]);
			}
		}
		for (int i=0; i<N_segments; ++i) {
			if (INTERSECTION_SEGMENT2D(tab_seg+i, tab_pos_seg+i, &RAYON, &POSITION)) {
				SDL_SetRenderDrawColor(renderer,255,255,255,255);
				SDL_RenderDrawLine(renderer,tab_pos_seg[i].x,tab_pos_seg[i].y,tab_seg[i].direction.x*tab_seg[i].longueur+tab_pos_seg[i].x,tab_seg[i].direction.y*tab_seg[i].longueur+tab_pos_seg[i].y);
			}
			else {
				SDL_SetRenderDrawColor(renderer,0,255,0,255);
				SDL_RenderDrawLine(renderer,tab_pos_seg[i].x,tab_pos_seg[i].y,tab_seg[i].direction.x*tab_seg[i].longueur+tab_pos_seg[i].x,tab_seg[i].direction.y*tab_seg[i].longueur+tab_pos_seg[i].y);
			}
		}

		SDL_SetRenderDrawColor(renderer,255,255,255,255);
		DrawCircle(renderer, POSITION.x, POSITION.y, RAYON);

		SDL_RenderPresent(renderer);
		SDL_Delay(16);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	exit(0);
}