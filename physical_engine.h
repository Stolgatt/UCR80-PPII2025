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

// il faut plus d'informations que juste celles contenues dans un segment2D
struct SEGMENT_HITBOX_MAP {

	SEGMENT2D segment;
	VECTEUR2D origine;

	VECTEUR2D normale; // vecteur unitaire pointant vers l'extérieur du segment
};
typedef struct SEGMENT_HITBOX_MAP SEGMENT_HITBOX_MAP;

struct CASE_GRILLE {

	VOITURE* liste_voitures;
	SEGMENT_HITBOX_MAP* tableau_ptr_segments[]; // un tableau de pointeurs

};
typedef struct CASE_GRILLE CASE_GRILLE;

struct monde_physique {

	// les engins rutilants
	VOITURE voiture_joueur;
	VOITURE* adversaires;

	// partitionnement de l'espace de jeu en une grille
	unsigned int nb_lignes;
	unsigned int nb_colonnes;
	float largeur_totale;
	float hauteur_totale;
	// la grille en question (stocké sous la forme d'une succession de lignes : grille[ligne][colonne] = grille[ligne*nb_colonnes + colonne])
	CASE_GRILLE* grille;

	// la partie graphique
	CAMERA cam;
	SCENE scene;

};
typedef struct monde_physique monde_physique;

SEGMENT2D CREA_SEGMENT_2D(int x1, int x2, int y1, int y2);
VECTEUR2D Calcul_reflexion(VECTEUR2D direction_voulue, SEGMENT2D* segment);
VECTEUR2D Calcul_collisions(VECTEUR2D direction_voulue,const float* rayon, const VECTEUR2D* position_disque);

#endif