#ifndef GRAPHICAL_ENGINE_H
#define GRAPHICAL_ENGINE_H

#include <SDL2/SDL.h>
#include "engine_common.h"

struct SPRITE {

	SDL_Texture* texture;
	// float rotation; // pas sûr que ça sera utile...
	float echelle;
	SDL_Rect source;
	VECTEUR3D position;

};
typedef struct SPRITE SPRITE;


struct Z_SPRITE {

	unsigned int index;
	float Z; // comme zizi

};
typedef struct Z_SPRITE Z_SPRITE;

struct SPRITE_PROJETE {

	SDL_Texture* texture;
	SDL_Rect source;
	SDL_Rect dest;

};
typedef struct SPRITE_PROJETE SPRITE_PROJETE;

struct TABLEAU_SPRITES {

	unsigned int N;
	SPRITE* sprites;

	struct TABLEAU_SPRITES* suivant;

};
typedef struct TABLEAU_SPRITES TABLEAU_SPRITES;


struct PLAN_HORIZONTAL {

	SDL_Texture* texture;
	float rotation;
	SDL_Rect source;
	float echelle;
	VECTEUR3D position;

	TABLEAU_SPRITES* sprites_au_dessus;

	struct PLAN_HORIZONTAL* au_dessus;
	struct PLAN_HORIZONTAL* en_dessous;

};
typedef struct PLAN_HORIZONTAL PLAN_HORIZONTAL;

struct SCENE {

	TABLEAU_SPRITES* sprites_tout_en_bas;
	PLAN_HORIZONTAL* tout_en_bas;
	PLAN_HORIZONTAL* tout_en_haut;

};
typedef struct SCENE SCENE;


struct CAMERA {

	SDL_Renderer* renderer;
	SDL_Texture* tmp_text;
	SDL_Texture* tmp_cible;
	SDL_Rect dimension_cible;
	SDL_Texture* cible;

	unsigned int N_MAX;
	Z_SPRITE* tableau_z;
	SPRITE_PROJETE* tableau_p;


	VECTEUR3D position;
	float longitude;
	float latitude;
	float roulis;

	float echelle_ecran;
	float distance_ecran;
	float offset_horizontal;
	float offset_vertical;

};
typedef struct CAMERA CAMERA;

// une structure pour initialiser une caméra
struct PARAMETRES_CAMERA {
	SDL_Renderer* renderer;
	SDL_Texture* tmp_text;
	SDL_Texture* tmp_cible;
	SDL_Rect dimension_cible;
	SDL_Texture* cible;

	unsigned int N_MAX;

	VECTEUR3D position_initiale;
	float longitude_initiale;
	float latitude_initiale;
	float roulis_initial;

	float semi_FOV_horizontal; // FOV/2, en radians, inclus dans [0;pi/2] 
	float distance_ecran_camera;
};
typedef struct PARAMETRES_CAMERA PARAMETRES_CAMERA;

void AFFICHAGE_CAMERA(const CAMERA* cam, const SCENE* scene);

#endif
