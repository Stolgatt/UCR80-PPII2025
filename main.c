#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <math.h>

#include "engine_common.h"
#include "graphical_engine.h"

#define TX 640
#define TY 480

void generer_ligne(VECTEUR3D* tab, const VECTEUR3D* position, const VECTEUR3D* direction, int N) {
	for (int i=0; i<N; ++i) {
		tab[i].x = position->x + i*direction->x;
		tab[i].y = position->y + i*direction->y;
		tab[i].z = position->z + i*direction->z;
	}
}

void generer_parallelogramme(VECTEUR3D* tab, const VECTEUR3D* position, const VECTEUR3D* direction, int N, const VECTEUR3D* direction2, int M) {
	VECTEUR3D actuel;
	for (int i=0; i<N; ++i) {
		actuel.x = position->x + i*direction->x;
		actuel.y = position->y + i*direction->y;
		actuel.z = position->z + i*direction->z;
		generer_ligne(tab+M*i, &actuel, direction2, M);
	}
}

int main() {

	SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER);
	SDL_Window* window = SDL_CreateWindow("graphical engine test", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, TX,TY, SDL_WINDOW_SHOWN);
	SDL_Surface* screen = SDL_GetWindowSurface(window);

	// setup de la caméra + buffers
	CAMERA CAM;
	CALCULER_FOV(&CAM, TX,TY, 2*M_PI/3, 0.5);
	CAM.position.x = 0.0;
	CAM.position.y = -2.0;
	CAM.position.z = 0.0;
	CAM.rotation.w = 1.0; CAM.rotation.x = CAM.rotation.y = CAM.rotation.z = 0.0;
	float* zbuffer = calloc(TX*TY+1,sizeof(float));
	unsigned int* buffer = calloc(TX*TY+1,sizeof(unsigned int));

	// setup de l'objet et de l'os
	OS3D pos_rot_objet;
	pos_rot_objet.nombre_enfants = 0;
	pos_rot_objet.enfants = NULL;
	OBJET3D objet;
	objet.os = &pos_rot_objet;
	objet.next = NULL;

	// génération des points de l'objet
	VECTEUR3D position = {-0.5,0.0,0.5};
	VECTEUR3D direction = {0.01,0.0,0.0};
	VECTEUR3D direction2 = {0.0,0.0,-0.01};
	const int N = fabs(1./direction.x);
	const int M = fabs(1./direction2.z);
	objet.tableau_points = calloc(N*M,sizeof(VECTEUR3D));
	generer_parallelogramme(objet.tableau_points, &position, &direction, N, &direction2, M);
	objet.N_POINTS = N * M;
	// génération de la couleur des pts de l'objet
	srand(SDL_GetTicks());
	objet.tableau_couleurs = calloc(N*M, sizeof(unsigned int));
	for (int i=0; i<objet.N_POINTS; ++i) {
		int tmp = (i<objet.N_POINTS/2) ? 128 : 255;
		objet.tableau_couleurs[i] = (tmp<<16)+(tmp<<8)+(255-tmp);
	}

	// placement de l'objet (via os)
	pos_rot_objet.echelle = 1.0;
	pos_rot_objet.q.w = 1.0; pos_rot_objet.q.x = pos_rot_objet.q.y = pos_rot_objet.q.z = 0.0;
	pos_rot_objet.position.x = 0.0;
	pos_rot_objet.position.y = 0.0;
	pos_rot_objet.position.z = 0.0;
	QUATERNION chgmt_rot;
	VECTEUR3D v_rot;
	v_rot.x = 0.0;
	v_rot.y = 0.0;
	v_rot.z = 1.0;
	CALCUL_QUATERNION(0.1, &v_rot, &chgmt_rot);
	CALCUL_BASE_OS3D_M(&pos_rot_objet);
	CALCUL_SQUELETTE_3D(&pos_rot_objet);

	// calcul de l'enveloppe convexe de l'objet
	CALCULER_BOUNDING_BOX(&objet);

	enum {UP=0,DOWN=1,LEFT=2,RIGHT=3};
	int INPUT[4] = {0};
	SDL_Event EVENT;
	int loop = 1;
	int screen_pitch;
	Uint8* screen_pixels;
	float tmp = 0.0, tmp2 = 0.0;
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

		if (INPUT[UP])
			pos_rot_objet.position.y += 0.05;
		if (INPUT[DOWN])
			pos_rot_objet.position.y -= 0.05;
		if (INPUT[LEFT])
			pos_rot_objet.position.x -= 0.05;
		if (INPUT[RIGHT])
			pos_rot_objet.position.x += 0.05;
		v_rot.x = 1.0;
		v_rot.y = 0.0;
		v_rot.z = 0.0;
		CALCUL_QUATERNION(tmp = tmp+0.01, &v_rot, &(pos_rot_objet.q));
		//v_rot.x = 0.0;
		//v_rot.y = 0.0;
		//v_rot.z = 1.0;
		//CALCUL_QUATERNION(tmp2 = tmp2+0.1, &v_rot, &chgmt_rot);
		//MULT_QUATERNIONS_EN_PLACE_DROITE(&chgmt_rot, &(pos_rot_objet.q));
		//NORMALISER_QUATERNION(&(pos_rot_objet.q),&(pos_rot_objet.q));

		CALCUL_BASE_OS3D_M(&pos_rot_objet);
		CALCUL_SQUELETTE_3D(&pos_rot_objet);
		AFFICHAGE_CAMERA(&CAM, &objet, zbuffer, buffer);
		SDL_LockSurface(screen);
		screen_pitch = screen->pitch;
		screen_pixels = (Uint8 *)screen->pixels;
		for (int j=0;j<TY;++j) {
			for (int i=0;i<TX;++i) {
				*((Uint32 *) (screen_pixels + j*screen_pitch + (i<<2))) = buffer[(TY-j-1)*TX + i];
				zbuffer[(TY-j-1)*TX + i] = FLT_MAX;
				buffer[(TY-j-1)*TX + i] = 0;
			}
		}
		SDL_UnlockSurface(screen);
		SDL_UpdateWindowSurface(window);
		SDL_Delay(16);
	}

	SDL_DestroyWindow(window);
	SDL_Quit();
	exit(0);
}