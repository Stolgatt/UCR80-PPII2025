#ifndef PHYSICAL_ENGINE
#define PHYSICAL_ENGINE

#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "engine_common.h"
#include "graphical_engine.h"

// #define FPS_CONTROLS

struct VOITURE {

	// nombre de disques constituants la hitbox
	unsigned short int nombre_disques;
	// hitbox à proprement parler
	float* tableau_rayons;
	VECTEUR2D* tableau_centres;
	// plus petit rectangle englobant les hitbox (par rapport au centre de la voiture)
	float min_x;
	float min_y;
	float max_x;
	float max_y;

	VECTEUR2D position;
	float angle; // par rapport à l'axe y, sens trigonométrique et en radians... pas forcément dans la même direction que la vitesse
	VECTEUR2D vect_rotation; // pour faciliter les calculs (correspond à l'angle ci-dessus)
	float vitesse; // 

	SPRITE* sprite;

	// variable temporaire pour la physique
	VECTEUR2D deplacement_final;

};
typedef struct VOITURE VOITURE;

typedef struct _CHECKPOINT{
    float x;
    float y;
    float r;
} CHECKPOINT;

struct CASE_GRILLE {

	unsigned short int nb_voitures;
	unsigned int indice_debut_segments;

};
typedef struct CASE_GRILLE CASE_GRILLE;

struct MONDE_PHYSIQUE {

	// les engins rutilants (index 0 = joueur)
	short int nb_voitures;
	VOITURE* voitures; // tableau

	// partitionnement de l'espace de jeu en une grille
	unsigned int nb_lignes;
	unsigned int nb_colonnes;
	float l;
	float h;
	// la grille en question (stocké sous la forme d'une succession de lignes : grille[ligne][colonne] = grille[ligne*nb_colonnes + colonne])
	// il faut une dernière case supplémentaire avec nb_voitures mis à 0 et indice_debut_segment mis au nombre total de segments de grille_segments
	CASE_GRILLE* grille;
	unsigned short int* grille_voitures; // tableau de char[nb_voitures] : grille_voitures[ligne][colonne][index] = grille[(ligne*nb_colonnes + colonne)*nb_voitures + index]
	VECTEUR2D* grille_pos_segments; // tableau de VECTEUR2D
	SEGMENT2D* grille_segments; // tableau de SEGMENT2D

	// la partie graphique
	CAMERA cam;
	SCENE scene; 
	TTF_Font* police;
	SDL_Rect minimap_rect;
	SDL_Point timer_position;
	SDL_Color timer_color;

	// la partie audio
	unsigned short int nb_channels;
	Mix_Chunk** tableau_sons;

	// la partie timer
	unsigned long long timer;

	// la partie minimap
	SDL_Texture* minimap;

	// partie checkpoints
	unsigned int nb_checkpoints;
	const CHECKPOINT* tableau_checkpoints;

};
typedef struct MONDE_PHYSIQUE MONDE_PHYSIQUE;

// defines liés au fonctionnement de la caméra par rapport au joueur
#define CAM_LAT (-M_PI/8.)
#define DIST_CAM_VOITURE 50.
#define HAUTEUR_CAMERA 35.
#define SEMI_FOV (M_PI/4.)
#define DIST_CAM_ECRAN 5.

// contient toutes les infos nécessaires à l'initialisation d'un niveau
struct NIVEAU {

	// voitures (la première est celle du joueur)
	unsigned short int nb_voitures;
	// tableau des positions initiales
	VECTEUR3D* positions_initiales_voit;
	// tableau des rotations initiales
	float* angles_initiaux_voit;
	// tableau des rayons (tailles)
	float* rayons_voit;
	// tableau des textures des sprites (avec échelles)
	float* echelles_voit;
	unsigned short int* texture_ids_voit;

	// sols (le premier est le plus profond)
	unsigned short int nb_sols;
	VECTEUR2D* positions_initiales_sols;
	float* angles_initiaux_sols;
	float* echelles_sols;
	unsigned short int* texture_ids_sols;

	// sprites décors
	unsigned int nb_decors;
	float* echelles_dec;
	VECTEUR3D* positions_dec;
	unsigned short int* texture_ids_dec;

	// segments
	unsigned short int nb_tableaux;
	unsigned int* tailles_tableaux;
	float** tableaux_x;
	float** tableaux_y;
	short int* sens;

	// grille
	unsigned int nb_lignes;
	unsigned int nb_colonnes;
	float l;
	float h;

	// checkpoints
	unsigned int nb_checkpoints;
	CHECKPOINT* tableau_checkpoints;

	// minimap
	unsigned short int minimap;

	// skybox
	unsigned short int texture_id_skybox;
	float skybox_rotation;

};
typedef struct NIVEAU NIVEAU;

struct CONTEXTE_SDL {

	// partie graphique
	PARAMETRES_CAMERA param_cam;
	SDL_Texture** tableau_textures;
	TTF_Font* police;
	SDL_Rect minimap_rect;
	SDL_Point timer_position;
	SDL_Color timer_color;
	// partie audio
	unsigned short int nb_channels;
	Mix_Chunk** tableau_sons;

};
typedef struct CONTEXTE_SDL CONTEXTE_SDL;

enum { UP = 0, DOWN, LEFT, RIGHT, Z, Q, S, D, A, E, W, X, O, K, L, M};

#define Dans_Checkpoint(chose,checkpoint) (INCLUS_DANS_DISQUE((chose).x,(chose).y, (checkpoint).x,(checkpoint).y,(checkpoint).r))

inline short int Test_Collision_Voitures(const VOITURE* v1, const VOITURE* v2, VECTEUR2D* direction) { // play ultrakill
	for (unsigned int i=0; i<v1->nombre_disques; ++i) {
		for (unsigned int j=0; j<v2->nombre_disques; ++j) {
			VECTEUR2D centre1 = v1->tableau_centres[i];
			centre1.x += v1->position.x;
			centre1.y += v1->position.y;
			VECTEUR2D centre2 = v2->tableau_centres[i];
			centre2.x += v2->position.x;
			centre2.y += v2->position.y;
			if INTERSECTION_DISQUES(centre1.x,centre1.y,v1->tableau_rayons[i], centre2.x,centre2.y,v2->tableau_rayons[j]) {
				direction->x = centre2.x - centre1.x;
				direction->y = centre2.y - centre1.y;
				NORMALISER_VECTEUR2D(direction,direction);
				return 1;
			}
		}
	}
	return 0;
}

inline short int Test_Collision_Voiture_Segment(const VOITURE* v1, const SEGMENT2D* segment, const VECTEUR2D* pos_segment, VECTEUR2D* direction) {
	for (unsigned int i=0; i<v1->nombre_disques; ++i) {
		VECTEUR2D centre = v1->tableau_centres[i];
		centre.x += v1->position.x;
		centre.y += v1->position.y;
		if (INTERSECTION_SEGMENT2D(segment,pos_segment,v1->tableau_rayons+i,&centre)) {
			// normale = rotation de 90° SENS TRIGONOMETRIQUE (sur la gauche) du vecteur
			direction->x = -segment->direction.y;
			direction->y = segment->direction.x;
			return 1;
		}
	}
	return 0;
}

inline SEGMENT2D CREA_SEGMENT_2D(float x1, float x2, float y1, float y2) {
	SEGMENT2D res;
	res.direction.x = x2 - x1;
	res.direction.y = y2 - y1;
	float norme = sqrt(res.direction.x*res.direction.x + res.direction.y*res.direction.y);
	res.direction.x /= norme;
	res.direction.y /= norme;
	res.longueur = norme;
	return res;
}

void Charger_Monde_Physique(MONDE_PHYSIQUE* monde, const NIVEAU* niveau, const CONTEXTE_SDL* contexte);
void Afficher_Monde_Physique(MONDE_PHYSIQUE* monde);
void Decharger_Monde_Physique(MONDE_PHYSIQUE* monde);
long long int Calculer_Monde_Physique(MONDE_PHYSIQUE* monde, const short int* INPUT, const unsigned long long dt);

#endif