#ifndef ENGINE_COMMON
#define ENGINE_COMMON

//#define SINCOSF_SYMBOL_NOT_FOUND_pk_ça_fait_ça_je_comprends_pas_en_tout_cas_voici_un_fix_dégueulasse_mais_fonctionnel

#include <math.h>

// jsp pk mais gcc trouve pas la déclaration de sincosf dans <math.h> donc pour éviter les warnings chiants je fais ça
//void sincosf(float, float*, float*);
// et au pire... :(
//#ifdef SINCOSF_SYMBOL_NOT_FOUND_pk_ça_fait_ça_je_comprends_pas_en_tout_cas_voici_un_fix_dégueulasse_mais_fonctionnel
void inline sincosf(float angle,float* sin,float* cos) {
	*sin = sinf(angle);
	*cos = cosf(angle);
}
//#endif

#define RAD2DEG(angle) ((angle)*180./M_PI)

struct VECTEUR2D {
	float x;
	float y;
};
typedef struct VECTEUR2D VECTEUR2D;

// nom+définition explicite
#define NORMALISER_VECTEUR2D_M(vi, vf, tmp_float) { (tmp_float) = 1.0/sqrtf( (vi).x*(vi).x + (vi).y*(vi).y ); (vf).x = (vi).x*(tmp_float); (vf).y = (vi).y*(tmp_float); }
inline void NORMALISER_VECTEUR2D(const VECTEUR2D* v_initial, VECTEUR2D* v_destination) {
	float tmp;
	NORMALISER_VECTEUR2D_M(*v_initial, *v_destination, tmp);
}

// idem
#define PDT_SCALAIRE_2D_M(v1,v2) ( (v1).x*(v2).x + (v1).y*(v2).y )

// noms+définitions explicites
#define APPLIQUER_BASE_2D_M(v_initial,v_resultant, i,j) { (v_resultant).x = (v_initial).x*(i).x + (v_initial).y*(j).x; (v_resultant).y = (v_initial).x*(i).y + (v_initial).y*(j).y; }
#define APPLIQUER_EN_PLACE_BASE_2D_M(vecteur, i,j,float_1) { (float_1) = (vecteur).x; (vecteur).x = (vecteur).x*(i).x + (vecteur).y*(j).x; (vecteur).y = (float_1)*(i).y + (vecteur).y*(j).y; }
inline void APPLIQUER_EN_PLACE_BASE_2D(VECTEUR2D* vecteur, const VECTEUR2D* i,const VECTEUR2D* j) {
	float tmp;
	APPLIQUER_EN_PLACE_BASE_2D_M(*vecteur, *i, *j, tmp);
}

#define APPLIQUER_BASE_INVERSE_2D_M(v_initial, v_resultant, i,j) { (v_resultant).x = PDT_SCALAIRE_2D_M(v_initial,i); (v_resultant).y = PDT_SCALAIRE_2D_M(v_initial,j); }
#define APPLIQUER_EN_PLACE_BASE_INVERSE_2D_M(v, i,j, float_1) { (float_1) = PDT_SCALAIRE_2D_M(v,i); (v).y = PDT_SCALAIRE_2D_M(v,j); (v).x = (float_1); }
inline void APPLIQUER_EN_PLACE_BASE_INVERSE_2D(VECTEUR2D* vecteur, const VECTEUR2D* i, const VECTEUR2D* j) {
	float tmp;
	APPLIQUER_EN_PLACE_BASE_INVERSE_2D_M(*vecteur, *i,*j, tmp);
}

// le "calcul d'une base 2d" se fait à partir d'une rotation et d'une échelle/de scaling (calcul de la matrice correspondant à cette transformation)
#define CALCUL_BASE_2D_M(rotation,echelle, i,j, float_ptr_1,float_ptr_2) { sincosf((rotation),(float_ptr_1),(float_ptr_2)); *(float_ptr_1) *= (echelle); *(float_ptr_2) *= (echelle); (i).x = *(float_ptr_2); (i).y = *(float_ptr_1); (j).x = -(*(float_ptr_1)); (j).y = *(float_ptr_2); }
inline void CALCUL_BASE_2D(const float* rotation, const float* echelle, VECTEUR2D* i,VECTEUR2D* j) {
	float tmp1, tmp2;
	CALCUL_BASE_2D_M(*rotation, *echelle, *i, *j, &tmp1, &tmp2);
}

struct OS2D {
	// position de l'os dans l'espace
	VECTEUR2D position;
	// vecteurs représentant la rotation de l'os dans l'espace (voire plus, techniquement permet de représenter une transformation par n'importe quelle matrice 2x2)
	VECTEUR2D i;
	VECTEUR2D j;
	
	// rotation de l'os dans l'espace (dans le sens trigonométrique classique)
	float theta;
	// taille ("scale") de l'os, càd la longueur des vecteurs i et j
	float echelle;

	// position et orientation finales, en tenant compte des os parents
	VECTEUR2D position_finale;
	VECTEUR2D i_final;
	VECTEUR2D j_final;

	// tableau d'OS2D contenant les enfants de l'os
	struct OS2D* enfants;
	int nombre_enfants;
};
typedef struct OS2D OS2D;

// calcule la base correspondant à la rotation de theta radians puis la mise à l'échelle par un facteur echelle et stocke le résultat dans i et j (modifie i et j, utilise theta et echelle comme arguments)
#define CALCUL_BASE_OS2D_M(os_ptr) CALCUL_BASE_2D( &((os_ptr)->theta), &((os_ptr)->echelle), &((os_ptr)->i), &((os_ptr)->j) )

// fonction récursive calculant les position/i/j finaux de chaque os
void CALCUL_SQUELETTE_2D(OS2D* os_racine);

struct SEGMENT2D {
	VECTEUR2D direction; // vecteur de norme 1 indiquant la direction du segment
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
inline int INTERSECTION_SEGMENT2D(const SEGMENT2D* segment, const VECTEUR2D* position_segment, const float* rayon, const VECTEUR2D* position_disque) {
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





struct VECTEUR3D {
	float x;
	float y;
	float z;
};
typedef struct VECTEUR3D VECTEUR3D;

#define NORMALISER_VECTEUR3D_M(vi, vf, tmp_float) { (tmp_float) = 1.0/sqrtf( (vi).x*(vi).x + (vi).y*(vi).y + (vi).z*(vi).z ); (vf).x = (vi).x*(tmp_float); (vf).y = (vi).y*(tmp_float); (vf).z = (vi).z*(tmp_float); }
inline void NORMALISER_VECTEUR3D(const VECTEUR3D* v_initial, VECTEUR3D* v_destination) {
	float tmp;
	NORMALISER_VECTEUR3D_M(*v_initial, *v_destination, tmp);
}

#define PDT_SCALAIRE_3D_M(v1,v2) ( (v1).x*(v2).x + (v1).y*(v2).y + (v1).z*(v2).z )

// vecteur de dimension 4. S'il est unitaire, représente une rotation en 3d
struct QUATERNION {
	float w;
	float x;
	float y;
	float z;
};
typedef struct QUATERNION QUATERNION;

// explicite
#define NORMALISER_QUATERNION_M(qi, qf, tmp_float) { (tmp_float) = 1.0/sqrtf( (qi).w*(qi).w + (qi).x*(qi).x + (qi).y*(qi).y + (qi).z*(qi).z ); (qf).w = (qi).w*(tmp_float); (qf).x = (qi).x*(tmp_float); (qf).y = (qi).y*(tmp_float); (qf).z = (qi).z*(tmp_float); }
inline void NORMALISER_QUATERNION(const QUATERNION* q_initial, QUATERNION* q_destination) {
	float tmp;
	NORMALISER_QUATERNION_M(*q_initial, *q_destination, tmp);
}

// multiplier 2 quaternions unitaires donne un quaternion unitaire, et revient donc à composer les rotations
// attention ! q1 * q2 != q2 * q1 (effectuer rotation 1 puis rotation 2 ne donne pas la même rotation finale que de faire rotation 2 puis rotation 1 !!!)
#define MULT_QUATERNIONS_M(qg, qd, qr) { (qr).w = (qg).w*(qd).w - (qg).x*(qd).x - (qg).y*(qd).y - (qg).z*(qd).z; (qr).x = (qg).w*(qd).x + (qg).x*(qd).w + (qg).y*(qd).z - (qg).z*(qd).y; (qr).y = (qg).w*(qd).y - (qg).x*(qd).z + (qg).y*(qd).w + (qg).z*(qd).x; (qr).z = (qg).w*(qd).z + (qg).x*(qd).y - (qg).y*(qd).x + (qg).z*(qd).w; }

// explicite (calculer le quaternion inverse = calculer la rotation inverse)
#define INVERSER_QUATERNION_M(qi,qf) { (qf).w = (qi).w; (qf).x = -(qi).x; (qf).y = -(qi).y; (qf).z = -(qi).z; }

// explicite
inline void MULT_QUATERNIONS_EN_PLACE_GAUCHE(QUATERNION* qg, const QUATERNION* qd) {
	float w = qg->w*qd->w - qg->x*qd->x - qg->y*qd->y - qg->z*qd->z;
	float x = qg->w*qd->x + qg->x*qd->w + qg->y*qd->z - qg->z*qd->y;
	float y = qg->w*qd->y - qg->x*qd->z + qg->y*qd->w + qg->z*qd->x;
	qg->z = qg->w*qd->z + qg->x*qd->y - qg->y*qd->x + qg->z*qd->w;
	qg->w = w; qg->x = x; qg->y = y;
}

inline void MULT_QUATERNIONS_EN_PLACE_DROITE(const QUATERNION* qg, QUATERNION* qd) {
	float w = qg->w*qd->w - qg->x*qd->x - qg->y*qd->y - qg->z*qd->z;
	float x = qg->w*qd->x + qg->x*qd->w + qg->y*qd->z - qg->z*qd->y;
	float y = qg->w*qd->y - qg->x*qd->z + qg->y*qd->w + qg->z*qd->x;
	qd->z = qg->w*qd->z + qg->x*qd->y - qg->y*qd->x + qg->z*qd->w;
	qd->w = w; qd->x = x; qd->y = y;
}

#define APPLIQUER_BASE_3D_M(v_initial,v_resultant, i,j,k) { (v_resultant).x = (v_initial).x*(i).x + (v_initial).y*(j).x + (v_initial).z*(k).x; (v_resultant).y = (v_initial).x*(i).y + (v_initial).y*(j).y  + (v_initial).z*(k).y; (v_resultant).z = (v_initial).x*(i).z + (v_initial).y*(j).z  + (v_initial).z*(k).z; }
#define APPLIQUER_EN_PLACE_BASE_3D_M(vecteur, i,j,k,float_1,float_2) { float_1 = (vecteur).x; float_2 = (vecteur).y; (vecteur).x = float_1*(i).x + float_2*(j).x + (vecteur).z*(k).x; (vecteur).y = float_1*(i).y + float_2*(j).y  + (vecteur).z*(k).y; (vecteur).z = float_1*(i).z + float_2*(j).z  + (vecteur).z*(k).z; }
inline void APPLIQUER_EN_PLACE_BASE_3D(VECTEUR3D* vecteur, const VECTEUR3D* i, const VECTEUR3D* j, const VECTEUR3D* k) {
	float tmp,tmp2;
	APPLIQUER_EN_PLACE_BASE_3D_M(*vecteur, *i,*j,*k, tmp,tmp2);
}

#define APPLIQUER_BASE_INVERSE_3D_M(v_initial, v_resultant, i,j,k) { (v_resultant).x = PDT_SCALAIRE_3D_M(v_initial,i); (v_resultant).y = PDT_SCALAIRE_3D_M(v_initial,j); (v_resultant).z = PDT_SCALAIRE_3D_M(v_initial,k); }
#define APPLIQUER_EN_PLACE_BASE_INVERSE_3D_M(v, i,j,k, float_1,float_2) { (float_1) = PDT_SCALAIRE_3D_M(v,i); (float_2) = PDT_SCALAIRE_3D_M(v,j); (v).z = PDT_SCALAIRE_3D_M(v,k); (v).x = (float_1); (v).y = (float_2); }
inline void APPLIQUER_EN_PLACE_BASE_INVERSE_3D(VECTEUR3D* vecteur, const VECTEUR3D* i, const VECTEUR3D* j, const VECTEUR3D* k) {
	float tmp,tmp2;
	APPLIQUER_EN_PLACE_BASE_INVERSE_3D_M(*vecteur, *i,*j,*k, tmp,tmp2);
}

// calcule le quaternion unitaire correspondant à la rotation d'axe "v_unitaire" et d'angle "angle"
inline void CALCUL_QUATERNION(float angle, const VECTEUR3D* v_unitaire, QUATERNION* q) {
	float sin_half,cos_half;
	sincosf(angle/2.0,&sin_half,&cos_half);
	q->w = cos_half;
	q->x = v_unitaire->x*sin_half;
	q->y = v_unitaire->y*sin_half;
	q->z = v_unitaire->z*sin_half;
}

// comme en 2d mais au lieu d'un simple angle (suffisant en 2d) la rotation est indiquée via un quaternion
inline void CALCUL_BASE_3D(const QUATERNION* q, const float* echelle, VECTEUR3D* i, VECTEUR3D* j, VECTEUR3D* k) {
	float ad = q->w*q->z;
	float bc = q->x*q->y;

	float bd = q->x*q->z;
	float ac = q->w*q->y;

	float ab = q->w*q->x;
	float cd = q->y*q->z;

	float bb = q->x*q->x;
	float cc = q->y*q->y;
	float dd = q->z*q->z;

	i->x = (*echelle)*( 1.-2.*(cc+dd) );
	i->y = (*echelle)*( 2.*(ad+bc) );
	i->z = (*echelle)*( 2.*(bd-ac) );
	j->x = (*echelle)*( 2.*(bc-ad) );
	j->y = (*echelle)*( 1.-2.*(bb+dd) );
	j->z = (*echelle)*( 2.*(ab+cd) );
	k->x = (*echelle)*( 2.*(ac+bd) );
	k->y = (*echelle)*( 2.*(cd-ab) );
	k->z = (*echelle)*( 1.-2.*(bb+cc) );
}

struct OS3D {
	VECTEUR3D position;
	VECTEUR3D i;
	VECTEUR3D j;
	VECTEUR3D k;

	QUATERNION q;
	float echelle;

	VECTEUR3D position_finale;
	VECTEUR3D i_final;
	VECTEUR3D j_final;
	VECTEUR3D k_final;

	struct OS3D* enfants;
	int nombre_enfants;
};
typedef struct OS3D OS3D;

#define CALCUL_BASE_OS3D_M(os_ptr) CALCUL_BASE_3D( &((os_ptr)->q), &((os_ptr)->echelle), &((os_ptr)->i), &((os_ptr)->j), &((os_ptr)->k) )

void CALCUL_SQUELETTE_3D(OS3D* os_racine);

struct SEGMENT3D {
	VECTEUR3D direction;
	float longueur;
};
typedef struct SEGMENT3D SEGMENT3D;

#define COMPOSANTE_3D(x,y,z, px,py,pz,dx,dy,dz) ( ((x)-(px))*(dx) + ((y)-(py))*(dy) + ((z)-(pz))*(dz) )
#define INTERSECTION_BOULES(x1,y1,z1,R1, x2,y2,z2,R2) ( ((x1)-(x2))*((x1)-(x2)) + ((y1)-(y2))*((y1)-(y2)) + ((z1)-(z2))*((z1)-(z2)) <= ((R1)+(R2))*((R1)+(R2)) )
#define INCLUS_DANS_BOULE(x,y,z, u,v,w,R) ( ((x)-(u))*((x)-(u)) + ((y)-(v))*((y)-(v)) + ((z)-(w))*((z)-(w)) <= R*R )
inline int INTERSECTION_SEGMENT3D(const SEGMENT3D* segment, const VECTEUR3D* position_segment, const float* rayon, const VECTEUR3D* position_boule) {
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
