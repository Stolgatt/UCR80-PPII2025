#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#include "engine_common.h"
#include "graphical_engine.h"

#define TX 1920
#define TY 1080

int main() {

	SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER); // Initialisation de la SDL
	SDL_Window* window = SDL_CreateWindow("graphical engine test", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, TX,TY, SDL_WINDOW_SHOWN); // Création de la fenêtre
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE); // Création du renderer
	SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 ); // Couleur de fond noire

	SDL_Surface* bmp_test = SDL_LoadBMP("map.bmp"); // charge une image BMP depuis le fichier "map.bmp"
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, bmp_test); // crée une texture à partir de la surface BMP chargée.
	SDL_FreeSurface(bmp_test); // libère la mémoire allouée pour la surface BMP

	CAMERA cam; // Initialise une structure CAMERA avec divers paramètres, y compris la position, l'orientation, et des textures temporaires.
	cam.N_MAX = 10000;
	cam.tableau_z = calloc(cam.N_MAX,sizeof(Z_SPRITE));
	cam.tableau_p = calloc(cam.N_MAX,sizeof(SPRITE_PROJETE));
	cam.position.x = 0.;
	cam.position.y = 0.;
	cam.position.z = -50;
	cam.longitude = 0.;
	cam.latitude = 0.;
	cam.roulis = 0.;
	cam.renderer = renderer;
	cam.tmp_text = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA32,SDL_TEXTUREACCESS_TARGET,4000,4000);
	cam.tmp_cible = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA32,SDL_TEXTUREACCESS_TARGET,1.5*TX,1.5*TX);
	cam.cible = NULL;
	cam.dimension_cible.x = 0; cam.dimension_cible.y = 0;
	cam.dimension_cible.w = TX; cam.dimension_cible.h = TY;
	cam.echelle_ecran = 0.025;
	cam.distance_ecran = 10.;
	cam.offset_horizontal = cam.offset_vertical = 0.;

	PLAN_HORIZONTAL plan; // Initialise un plan horizontal avec une texture, une position, une rotation, une échelle, et des pointeurs vers les sprites au-dessus et en-dessous.
	plan.texture = texture;
	plan.rotation = 0.;
	plan.echelle = 1.;
	plan.position.x = plan.position.z = 0.;
	plan.position.y = 0;
	plan.au_dessus = plan.en_dessous = plan.sprites_au_dessus = NULL;
	plan.source.x = plan.source.y = 0;
	plan.source.w = 1600;
	plan.source.h = 3000;

	SCENE scene; // Initialise une scène avec un plan au-dessus et un plan en-dessous.	
	scene.sprites_tout_en_bas = NULL;
	scene.tout_en_bas = scene.tout_en_haut = &plan;

	enum {UP=0,DOWN=1,LEFT=2,RIGHT=3,Z,Q,S,D,A,E,W,X}; // Définit une énumération pour les touches de contrôle.
	int INPUT[12] = {0}; // Crée un tableau INPUT pour suivre l'état des touches de contrôle.
	SDL_Event EVENT;
	int loop = 1;
	int fps = 0;
	long long temps_fps = SDL_GetTicks(); // Initialise un compteur de FPS.
	while (loop) { // Boucle principale du programme.

		while( SDL_PollEvent(&EVENT) != 0 ) { // Gestion des événements.
	        if (EVENT.type == SDL_QUIT) {
                loop = 0;
            }
            else if (EVENT.type == SDL_KEYDOWN) { // Gestion des touches de contrôle.
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

		if (INPUT[UP]) cam.latitude += 0.05; // Gestion des déplacements de la caméra.
		if (INPUT[DOWN]) cam.latitude -= 0.05;
		if (INPUT[LEFT]) cam.longitude += 0.05;
		if (INPUT[RIGHT]) cam.longitude -= 0.05;
		if (INPUT[W]) plan.rotation += 0.05;
		if (INPUT[X]) plan.rotation -= 0.05;
		cam.latitude = cam.latitude > M_PI/2 ? M_PI/2 : cam.latitude;
		cam.latitude = cam.latitude < -M_PI/2 ? -M_PI/2 : cam.latitude;
		if (INPUT[Z]) cam.position.y += 0.7;
		if (INPUT[S]) cam.position.y -= 0.7;
		if (INPUT[D]) cam.position.x += 0.7;
		if (INPUT[Q]) cam.position.x -= 0.7;
		if (INPUT[E]) cam.position.z += 0.7;
		if (INPUT[A]) cam.position.z -= 0.7;

		SDL_RenderClear(renderer); // Efface le rendu précédent.
		AFFICHAGE_CAMERA(&cam, &scene); // Affiche la scène à l'écran.
		SDL_RenderPresent(renderer); // Affiche le rendu à l'écran.

		SDL_Delay(16); // Limite la fréquence de rafraîchissement à 60 FPS.
		if (SDL_GetTicks() - temps_fps >= 1000) { // Affiche les FPS toutes les secondes.
			printf("FPS: %d\n",fps);
			temps_fps = SDL_GetTicks(); 
			fps = 0;
		}
		else {
			fps++; 
		}
	}

	SDL_DestroyTexture(cam.tmp_text); // Libère la mémoire allouée pour les textures temporaires.
	SDL_DestroyTexture(cam.tmp_cible); 
	SDL_DestroyRenderer(renderer); 
	SDL_DestroyWindow(window);
	SDL_Quit(); 
	exit(0); 
}