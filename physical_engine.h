#ifndef PHYSICAL_ENGINE
#define PHYSICAL_ENGINE

#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include "engine_common.h"
#include "graphical_engine.h"

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
	CASE_GRILLE* grille;
	unsigned short int* grille_voitures; // tableau de char[nb_voitures] : grille_voitures[ligne][colonne][index] = grille[(ligne*nb_colonnes + colonne)*nb_voitures + index]
	// il faut une dernière case supplémentaire avec nb_voitures mis à 0 et indice_debut_segment mis au nombre total de segments de grille_segments
	VECTEUR2D* grille_pos_segments; // tableau de VECTEUR2D
	SEGMENT2D* grille_segments; // tableau de SEGMENT2D

	// la partie graphique
	CAMERA cam;
	SCENE scene; 

};
typedef struct MONDE_PHYSIQUE MONDE_PHYSIQUE;

// contient toutes les infos nécessaires à l'initialisation d'un niveau
struct NIVEAU {

	// position initiale de la caméra (et du joueur)
	VECTEUR3D position_initiale;
	float longitude_initiale;
	float latitude_initiale;

	// distance au sol des sprites des voitures
	float distance_au_sol;

	// adversaires
	unsigned short int nb_adversaires;
	// tableau des positions initiales des adversaires
	VECTEUR2D* positions_initiales_adv;
	// tableau des rotations initiales des adversaires
	float* angles_initiaux_adv;
	// tableau des rayons (tailles) des adversaires
	float* rayons_adv;
	// tableau des textures des sprites
	unsigned short int* texture_ids_adv;

	// sols (le premier est le plus profond)
	unsigned short int nb_sols;
	VECTEUR2D* positions_initiales_sols;
	float* angles_initiaux_sols;
	float* echelles_sols;
	unsigned short int* texture_ids_sols;

	// sprites décors
	VECTEUR3D* position_initiales_dec;
	float* echelles_dec;
	VECTEUR3D* positions_dec;
	unsigned short int* texture_ids_dec;

	// segments
	unsigned short int nb_tableaux_segments;
	unsigned int* separations_tableaux_segments;
	int* tableau_x;
	int* tableau_y;

};
typedef struct NIVEAU NIVEAU;

enum { UP = 0, DOWN, LEFT, RIGHT, Z, Q, S, D, A, E, W, X, O, K, L, M};

inline short int Test_Collision_Voitures(const VOITURE* v1, const VOITURE* v2, VECTEUR2D* direction) { // play ultrakill
	for (unsigned int i=0; i<v1->nombre_disques; ++i) {
		for (unsigned int j=0; j<v2->nombre_disques; ++j) {
			if INTERSECTION_DISQUES(v1->tableau_centres[i].x,v1->tableau_centres[i].y,v1->tableau_rayons[i], v2->tableau_centres[j].x,v2->tableau_centres[j].y,v2->tableau_rayons[j]) {
				direction->x = v2->tableau_centres[j].x - v1->tableau_centres[i].x;
				direction->y = v2->tableau_centres[j].y - v1->tableau_centres[i].y;
				NORMALISER_VECTEUR2D(direction,direction);
				return 1;
			}
		}
	}
	return 0;
}

inline short int Test_Collision_Voiture_Segment(const VOITURE* v1, const SEGMENT2D* segment, const VECTEUR2D* pos_segment, VECTEUR2D* direction) {
	for (unsigned int i=0; i<v1->nombre_disques; ++i) {
		if (INTERSECTION_SEGMENT2D(segment,pos_segment,v1->tableau_rayons+i,v1->tableau_centres+i)) {
			// normale = rotation de 90° SENS TRIGONOMETRIQUE (sur la gauche) du vecteur
			direction->x = -segment->direction.y;
			direction->y = segment->direction.x;
			return 1;
		}
	}
	return 0;
}

void Charger_Monde_Physique(MONDE_PHYSIQUE* monde, const NIVEAU* niveau, const PARAMETRES_CAMERA* param_cam, const SDL_Texture** tableau_textures);
void Decharger_Monde_Physique(MONDE_PHYSIQUE* monde);
short int Calculer_Monde_Physique(MONDE_PHYSIQUE* monde, const short int* INPUT, const float dt);

inline VECTEUR2D Calcul_reflexion(VECTEUR2D direction_voulue, SEGMENT2D* segment) {
    VECTEUR2D vecteur_normal;
    vecteur_normal.x = -segment->direction.y;
    vecteur_normal.y = segment->direction.x;
    float produit_scalaire = direction_voulue.x*vecteur_normal.x + direction_voulue.y*vecteur_normal.y;
    VECTEUR2D reflexion;
    reflexion.x = direction_voulue.x - 2*produit_scalaire*vecteur_normal.x;
    reflexion.y = direction_voulue.y - 2*produit_scalaire*vecteur_normal.y;
    return reflexion;
}
SEGMENT2D CREA_SEGMENT_2D(int x1, int x2, int y1, int y2);
VECTEUR2D Calcul_collisions(VECTEUR2D direction_voulue,const float* rayon, const VECTEUR2D* position_disque);

#endif