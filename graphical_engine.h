#ifndef GRAPHICAL_ENGINE
#define GRAPHICAL_ENGINE

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

/*
-> ux engine : pas sûr du tout que ce soit nécessaire + chiant, donc non 
-> partitionnement de l'espace : littéralement juste un tableau de pointeurs, donc pas grand chose à faire

-> définir la base de fonctionnement du moteur physique (modèle des objets), en lien avec le partitionnement de l'espace, avec potentiellement un système de référentiel/positionnement
-> moteur de rendu : attendre de voir si 2D ou 3D ou entre deux
-> audio ?

*/

struct VERTEX2D {
	float x;
	float y;
};
typedef struct VERTEX2D VERTEX2D;

struct HITBOX2D {
	int n; // nombre de cercles
	VERTEX2D* tab_centres; // tableau contenant les centres des cercles constituant la hitbox
	float* tab_rayons; // tableau contenant les rayons des cercles constituant la hitbox
};
typedef struct HITBOX2D HITBOX2D;

struct SEGMENT2D {
	VERTEX2D direction; // vecteur de norme 1 indiquant la direction du segment
	float longueur; // longueur du segment, forcément >= 0
};
typedef struct SEGMENT2D SEGMENT2D;

// composante du point (x,y) selon la droite de direction (dx, dy) (vecteur unitaire) et passant par le point (px,py)
#define COMPOSANTE_2D(x,y, px,py,dx,dy) ( ((x)-(px))*(dx) + ((y)-(py))*(dy) )
// expression valant 1 (ou 0) suivant si les deux disques s'intersectent (ou non)
#define INTERSECTION_DISQUES(x1,y1,R1, x2,y2,R2) ( ((x1)-(x2))*((x1)-(x2)) + ((y1)-(y2))*((y1)-(y2)) <= ((R1)+(R2))*((R1)+(R2)) )
// expression valant 1 (ou 0) suivant si le point (x,y) est inclus dans le disque ((u,v),R) (ou non)
#define INCLUS_DANS_DISQUE(x,y, u,v,R) ( ((x)-(u))*((x)-(u)) + ((y)-(v))*((y)-(v)) <= R*R )
// fonction renvoyant 1 (ou 0) suivant si le disque et le segment s'intersectent (ou non)
inline int INTERSECTION_SEGMENT2D(const SEGMENT2D* segment, const VERTEX2D* position_segment, const float* rayon, const VERTEX2D* position_disque) {
	float composante = COMPOSANTE_2D(position_disque->x,position_disque->y, position_segment->x,position_segment->y,segment->direction.x,segment->direction.y);
	float x = position_segment->x + (segment->direction.x * composante);
	float y = position_segment->y + (segment->direction.y * composante);
	if (INCLUS_DANS_DISQUE(x,y, position_disque->x,position_disque->y,*rayon)) {
		if (composante <= 0) {
			if (INCLUS_DANS_DISQUE(position_segment->x,position_segment->y, position_disque->x,position_disque->y,*rayon))
				return 1;
			else
				return 0;
		}
		else if (composante >= segment->longueur) {
			x = position_segment->x + (segment->direction.x * segment->longueur);
			y = position_segment->y + (segment->direction.y * segment->longueur);
			if (INCLUS_DANS_DISQUE(x,y, position_disque->x,position_disque->y,*rayon))
				return 1;
			else
				return 0;
		}
		else
			return 1;
	}
	else
		return 0;
}



struct VERTEX3D {
	float x;
	float y;
	float z;
};
typedef struct VERTEX3D VERTEX3D;

struct HITBOX3D {
	int n;
	VERTEX3D* tab_centres;
	float* tab_rayons;
};
typedef struct HITBOX3D HITBOX3D;

struct SEGMENT3D {
	VERTEX3D direction;
	float longueur;
};
typedef struct SEGMENT3D SEGMENT3D;

#define COMPOSANTE_3D(x,y,z, px,py,pz,dx,dy,dz) ( ((x)-(px))*(dx) + ((y)-(py))*(dy) + ((z)-(pz))*(dz) )
#define INTERSECTION_BOULES(x1,y1,z1,R1, x2,y2,z2,R2) ( ((x1)-(x2))*((x1)-(x2)) + ((y1)-(y2))*((y1)-(y2)) + ((z1)-(z2))*((z1)-(z2)) <= ((R1)+(R2))*((R1)+(R2)) )
#define INCLUS_DANS_BOULE(x,y,z, u,v,w,R) ( ((x)-(u))*((x)-(u)) + ((y)-(v))*((y)-(v)) + ((z)-(w))*((z)-(w)) <= R*R )
inline int INTERSECTION_SEGMENT3D(const SEGMENT3D* segment, const VERTEX3D* position_segment, const float* rayon, const VERTEX3D* position_boule) {
	float composante = COMPOSANTE_3D(position_boule->x,position_boule->y,position_boule->z, position_segment->x,position_segment->y,position_segment->z,segment->direction.x,segment->direction.y,segment->direction.z);
	float x = position_segment->x + (segment->direction.x * composante);
	float y = position_segment->y + (segment->direction.y * composante);
	float z = position_segment->z + (segment->direction.z * composante);
	if (INCLUS_DANS_BOULE(x,y,z, position_boule->x,position_boule->y,position_boule->z,*rayon)) {
		if (composante <= 0) {
			if (INCLUS_DANS_BOULE(position_segment->x,position_segment->y,position_segment->z, position_boule->x,position_boule->y,position_boule->z,*rayon))
				return 1;
			else
				return 0;
		}
		else if (composante >= segment->longueur) {
			x = position_segment->x + (segment->direction.x * segment->longueur);
			y = position_segment->y + (segment->direction.y * segment->longueur);
			z = position_segment->z + (segment->direction.z * segment->longueur);
			if (INCLUS_DANS_BOULE(x,y,z, position_boule->x,position_boule->y,position_boule->z,*rayon))
				return 1;
			else
				return 0;
		}
		else
			return 1;
	}
	else
		return 0;
}

#endif
