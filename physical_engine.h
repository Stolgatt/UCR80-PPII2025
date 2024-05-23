#ifndef PHYSICAL_ENGINE
#define PHYSICAL_ENGINE

#include <stdio.h>
#include <stdlib.h>
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

	// pour aisément pouvoir chaîner les voitures
	struct voiture* voiture_suivante;
	struct voiture* voiture_precedente;

};
typedef struct VOITURE VOITURE;

struct CASE_GRILLE {

	VOITURE* liste_voitures;
	unsigned int nb_segments;
	SEGMENT2D** tableau_ptr_segments; // un tableau de pointeurs (vers des segments)
	VECTEUR2D** tableau_ptr_points; // un tableau de pointeurs (vers des origines de segments)

};
typedef struct CASE_GRILLE CASE_GRILLE;

struct MONDE_PHYSIQUE {

	// les engins rutilants
	VOITURE joueur;
	VOITURE* adversaires;

	// partitionnement de l'espace de jeu en une grille
	unsigned int nb_lignes;
	unsigned int nb_colonnes;
	float l;
	float h;
	// la grille en question (stocké sous la forme d'une succession de lignes : grille[ligne][colonne] = grille[ligne*nb_colonnes + colonne])
	CASE_GRILLE* grille;

	// la partie graphique
	CAMERA cam;
	SCENE scene;

};
typedef struct MONDE_PHYSIQUE MONDE_PHYSIQUE;

enum { UP = 0, DOWN, LEFT, RIGHT, Z, Q, S, D, A, E, W, X, O, K, L, M};

// TODO : définir deux fonctions inline, une enlevant les références d'une voiture dans la grille, et l'autre les ajoutant (en fct de voiture.position et voiture.min/max_x/y)

inline short int Test_Collision_Voitures(VOITURE* v1, VOITURE* v2, VECTEUR2D* direction) { // play ultrakill
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

void Charger_Monde_Physique(MONDE_PHYSIQUE* monde); // à modifier potentiellement (rajouter une structure qui contient les informations relatives à un niveau et la passer en argument)
void Calculer_Monde_Physique(MONDE_PHYSIQUE* monde, const short int* INPUT, const float dt);

SEGMENT2D CREA_SEGMENT_2D(int x1, int x2, int y1, int y2);
VECTEUR2D Calcul_collisions(VECTEUR2D direction_voulue,const float* rayon, const VECTEUR2D* position_disque);

#endif