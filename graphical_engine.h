#ifndef GRAPHICAL_ENGINE_H
#define GRAPHICAL_ENGINE_H

#include "engine_common.h"

#include <SDL2/SDL.h>

#include <stdio.h>

struct SPRITE {

	const SDL_Texture* texture;
	float rotation;
	float echelle;
	SDL_Rect source;
	VECTEUR3D position;

};
typedef struct SPRITE SPRITE;


struct D_SPRITE {

	SPRITE* sprite;
	float D;

};
typedef struct D_SPRITE D_SPRITE;


struct TABLEAU_SPRITES {

	unsigned int N;
	SPRITE* sprites;

	struct TABLEAU_SPRITES* suivant;

};
typedef struct TABLEAU_SPRITES TABLEAU_SPRITES;


struct PLAN_HORIZONTAL {

	const SDL_Texture* texture;
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

	D_SPRITE* tableau_d;
	unsigned int N_MAX;

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

#define PREPARATION_RENDERER_POUR_RENDU(renderer,bg_R,bg_G,bg_B,bg_A) {SDL_SetRenderDrawColor(renderer,bg_R,bg_G,bg_B,bg_A); } // à compléter

void AFFICHAGE_CAMERA(const CAMERA* cam, const SCENE* scene);

#endif
