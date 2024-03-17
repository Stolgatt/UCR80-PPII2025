#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#define TX 1920
#define TY 1080
#define NTEST 100

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

	exit(0);
}