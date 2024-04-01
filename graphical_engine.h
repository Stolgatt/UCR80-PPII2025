#ifndef GRAPHICAL_ENGINE_H
#define GRAPHICAL_ENGINE_H

#include "engine_common.h"
#include <stddef.h>
#include <float.h>

struct CAMERA {
	int largeur_ecran;
	int hauteur_ecran;

	float limite_gauche_fov;
	float limite_droite_fov;
	float limite_haute_fov;
	float limite_basse_fov;
	float distance_cam_ecran;

	VECTEUR3D position;
	QUATERNION rotation;
};
typedef struct CAMERA CAMERA;

struct OBJET3D {
	OS3D* os;
	VECTEUR3D enveloppe_convexe[8]; // pas nécessairement un cube, juste une enveloppe convexe (un cube potentiellement déformé quoi)
	int N_POINTS;
	VECTEUR3D* tableau_points;
	// temporaire
	unsigned int* tableau_couleurs;

	struct OBJET3D* next;
};
typedef struct OBJET3D OBJET3D;

void CALCULER_BOUNDING_BOX(OBJET3D* objet);
void CALCULER_FOV(CAMERA* cam, int largeur_ecran, int hauteur_ecran, float FOV_horizontal, float distance_cam_ecran);
void AFFICHAGE_CAMERA(const CAMERA* cam, const OBJET3D* objets, float* zbuffer, unsigned int* buffer);

#endif
