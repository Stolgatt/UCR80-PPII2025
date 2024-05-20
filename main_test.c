#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "engine_common.h"

#define TX 1920
#define TY 1080
#define NTEST 100

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
	int i,j,k,color;
	long long before;

	// SDL_RenderDrawPoints
	SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER);
	SDL_Window* window = SDL_CreateWindow("Performance Comparison", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, TX,TY, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_NONE);

	/*srand(SDL_GetTicks());
	SDL_Point* points = calloc(sizeof(SDL_Point),TX*TY);
	before = SDL_GetTicks();
	for (k=0;k<NTEST;++k) {
		SDL_SetRenderDrawColor(renderer,rand()%256,rand()%256,rand()%256,255);
		for (i=0;i<TX;++i)
			for (j=0;j<TY;++j)
				{ points[i+TX*j].x = i; points[i+TX*j].y = j; }
		SDL_RenderDrawPoints(renderer, points, TX*TY);
		SDL_RenderPresent(renderer);
	}
	printf("SDL_RenderDrawPoints, only one color: %d times: %ldms\n",NTEST,SDL_GetTicks()-before);
	free(points);*/

	SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
	SDL_Quit();

	// SDL_RenderDrawPoint
	SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER);
	window = SDL_CreateWindow("Performance Comparison", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, TX,TY, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_NONE);

	/*srand(SDL_GetTicks());
	before = SDL_GetTicks();
	color = 0;
	for (k=0;k<NTEST;++k) {
		for (i=0;i<TX;++i)
			for (j=0;j<TY;++j)
				{ SDL_SetRenderDrawColor(renderer,color = (color+1)%256,color,color,255); SDL_RenderDrawPoint(renderer, i, j); }
		SDL_RenderPresent(renderer);
	}
	printf("SDL_RenderDrawPoint: %d times: %ldms\n",NTEST,SDL_GetTicks()-before);*/

	SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
	SDL_Quit();

	// Full Software Rendering!!

	SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER);
	window = SDL_CreateWindow("Performance Comparison", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, TX,TY, SDL_WINDOW_SHOWN);
	SDL_Surface* screen = SDL_GetWindowSurface(window);

	srand(SDL_GetTicks());
	before = SDL_GetTicks();
	color = 0;
	int screen_pitch;
	Uint8* screen_pixels;
	for (k=0;k<NTEST;++k) {
		SDL_LockSurface(screen);
		screen_pitch = screen->pitch;
		screen_pixels = (Uint8 *)screen->pixels;
		for (j=0;j<TY;++j)
			for (i=0;i<TX;++i)
				*((Uint32 *) (screen_pixels + j*screen_pitch + (i<<2))) = ((color=((color+1)%256))<<16)+(color<<8)+color;
		SDL_UnlockSurface(screen);
		SDL_UpdateWindowSurface(window);
	}
	printf("direct surface access + SDL_UpdateWindowSurface: %d times: %ldms\n",NTEST,SDL_GetTicks()-before);

    SDL_DestroyWindow(window);
	SDL_Quit();

	// Software rendering + final output with textures and renderers

	SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER);
	window = SDL_CreateWindow("Performance Comparison", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, TX,TY, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Texture* targetTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, TX, TY);

    srand(SDL_GetTicks());
	before = SDL_GetTicks();
	int pitch;
	Uint8* pixels;
	color = 0;
    for (k=0;k<NTEST;k++) {
    	SDL_LockTexture(targetTexture, NULL, &pixels, &pitch);
	    for (j=0; j<TY; ++j) {
	    	for (i=0; i<TX; ++i) {
	            *((Uint32*) (pixels + j*pitch + (i<<2))) = ((color=((color+1)%256))<<24)+(color<<16)+(color<<8);
	        }
	    }
	    SDL_UnlockTexture(targetTexture);
    	SDL_RenderCopy(renderer, targetTexture, NULL, NULL);
    	SDL_RenderPresent(renderer);
    }
    printf("direct texture access + SDL_RenderCopy and SDL_RenderPresent: %d times: %ldms\n",NTEST,SDL_GetTicks()-before);

    SDL_DestroyTexture(targetTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    // démo avec les hitbox

	SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER);
	window = SDL_CreateWindow("graphical engine test", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, TX,TY, SDL_WINDOW_SHOWN);
	// SDL_Surface* screen = SDL_GetWindowSurface(window);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_NONE);

	const int N_disques = 3;
	VECTEUR2D tab_centres[] = {{150,150},{10,10},{150,100}};
	float tab_rayons[] = {100,2,50};

	const int N_segments = 2;
	VECTEUR2D tab_pos_seg[] = { {30,35}, {300,300} };
	SEGMENT2D tab_seg[] = { {{cos(M_PI/4),sin(M_PI/4)},60}, {{1,0},10} };

	float RAYON = 20;
	VECTEUR2D POSITION = {50,50};

	const int N_COTES = 5;
	const float LONGUEUR = sqrtf( powf(1-cosf(2*M_PI/N_COTES), 2) + powf(sinf(2*M_PI/N_COTES),2) ); // calcul simplifiable xD mais bonne grosse flemme hein
	const float PIXELS = 60;
	OS2D tab_os[N_COTES+1];
	for (int i=1; i<=N_COTES; ++i) {
		tab_os[i-1].enfants = tab_os+i;
		tab_os[i-1].nombre_enfants = 1;

		tab_os[i].position.x = LONGUEUR;
		tab_os[i].position.y = 0;
		tab_os[i].echelle = 1;
		tab_os[i].theta = 2*M_PI/N_COTES;
		CALCUL_BASE_OS2D_M(tab_os+i);
	}
	tab_os[0].echelle = PIXELS;
	tab_os[0].position.x = 400;
	tab_os[0].position.y = 300;
	tab_os[0].theta = 0;
	CALCUL_BASE_OS2D_M(tab_os);

	tab_os[N_COTES].nombre_enfants = 0;
	CALCUL_SQUELETTE_2D(tab_os);


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

		for (int i=1; i<=N_COTES; ++i) {
			SDL_RenderDrawLine(renderer,tab_os[i].position_finale.x,tab_os[i].position_finale.y,  tab_os[i].position_finale.x + tab_os[i].i_final.x*LONGUEUR, tab_os[i].position_finale.y + tab_os[i].i_final.y*LONGUEUR);
		}

		SDL_RenderPresent(renderer);
		SDL_Delay(16);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	exit(0);

	exit(0);
}
