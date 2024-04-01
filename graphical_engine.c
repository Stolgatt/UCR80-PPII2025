#include "graphical_engine.h"

void CALCULER_BOUNDING_BOX(OBJET3D* objet) {
	float min_x, min_y, min_z;
	float max_x, max_y, max_z;
	min_x = min_y = min_z = FLT_MAX;
	max_x = max_y = max_z = -FLT_MAX;
	for (int i=0; i<objet->N_POINTS; ++i) {
		min_x = (objet->tableau_points[i].x <= min_x) ? objet->tableau_points[i].x : min_x;
		min_y = (objet->tableau_points[i].y <= min_y) ? objet->tableau_points[i].y : min_y;
		min_z = (objet->tableau_points[i].z <= min_z) ? objet->tableau_points[i].z : min_z;
		max_x = (objet->tableau_points[i].x >= max_x) ? objet->tableau_points[i].x : max_x;
		max_y = (objet->tableau_points[i].y >= max_y) ? objet->tableau_points[i].y : max_y;
		max_z = (objet->tableau_points[i].z >= max_z) ? objet->tableau_points[i].z : max_z;
	}
	objet->enveloppe_convexe[0].x = min_x; objet->enveloppe_convexe[0].y = min_y; objet->enveloppe_convexe[0].z = min_z;
	objet->enveloppe_convexe[1].x = min_x; objet->enveloppe_convexe[1].y = max_y; objet->enveloppe_convexe[1].z = min_z;
	objet->enveloppe_convexe[2].x = max_x; objet->enveloppe_convexe[2].y = max_y; objet->enveloppe_convexe[2].z = min_z;
	objet->enveloppe_convexe[3].x = max_x; objet->enveloppe_convexe[3].y = min_y; objet->enveloppe_convexe[3].z = min_z;
	objet->enveloppe_convexe[4].x = min_x; objet->enveloppe_convexe[4].y = min_y; objet->enveloppe_convexe[4].z = max_z;
	objet->enveloppe_convexe[5].x = min_x; objet->enveloppe_convexe[5].y = max_y; objet->enveloppe_convexe[5].z = max_z;
	objet->enveloppe_convexe[6].x = max_x; objet->enveloppe_convexe[6].y = max_y; objet->enveloppe_convexe[6].z = max_z;
	objet->enveloppe_convexe[7].x = max_x; objet->enveloppe_convexe[7].y = min_y; objet->enveloppe_convexe[7].z = max_z;
}

void CALCULER_FOV(CAMERA* cam, int largeur_ecran, int hauteur_ecran, float FOV_horizontal, float distance_cam_ecran) {
	cam->distance_cam_ecran = distance_cam_ecran;
	cam->largeur_ecran = largeur_ecran;
	cam->hauteur_ecran = hauteur_ecran;
	float tmp = tanf(FOV_horizontal/2.0)*distance_cam_ecran;
	cam->limite_gauche_fov = -tmp;
	cam->limite_droite_fov = tmp;
	tmp *= ( (float) hauteur_ecran )/( (float) largeur_ecran);
	cam->limite_haute_fov = tmp;
	cam->limite_basse_fov = -tmp;
}

// les buffers doivent faire largeur_ecran*hauteur_ecran PLUS UN
void AFFICHAGE_CAMERA(const CAMERA* cam, const OBJET3D* objets, float* zbuffer, unsigned int* buffer) {
	// calculer la base de la caméra
	QUATERNION tmp_q;
	float tmp_f = 1.0;
	VECTEUR3D cam_i,cam_j,cam_k;
	INVERSER_QUATERNION_M(cam->rotation, tmp_q);
	CALCUL_BASE_3D(&tmp_q, &tmp_f, &cam_i, &cam_j, &cam_k);
	// précalcul pour le culling des objets
	VECTEUR2D gauche = {-cam->distance_cam_ecran,cam->limite_gauche_fov};
	VECTEUR2D droite = {cam->distance_cam_ecran,-cam->limite_droite_fov};
	VECTEUR2D haut = {-cam->limite_haute_fov,cam->distance_cam_ecran};
	VECTEUR2D bas = {cam->limite_basse_fov,-cam->distance_cam_ecran};
	// précalcul pour la projection sur l'écran
	float coef_x = cam->largeur_ecran / (cam->limite_droite_fov - cam->limite_gauche_fov);
	float coef_y = cam->hauteur_ecran / (cam->limite_haute_fov - cam->limite_basse_fov);
	float offset_x = -coef_x*cam->limite_gauche_fov;
	float offset_y = -coef_y*cam->limite_basse_fov;
	coef_x *= cam->distance_cam_ecran;
	coef_y *= cam->distance_cam_ecran;
	int N_TOTAL = cam->largeur_ecran*cam->hauteur_ecran;

	// boucle + initialisation de variables
	VECTEUR3D I,J,K,OFFSET,tmp_v;
	int compteur_gauche, compteur_droite, compteur_haut, compteur_bas, compteur_derrière;
	float d_carre_min;
	int x,y,indice,visible;
	while (objets != NULL) {
		// combiner base+coordonées de l'os avec celles de la caméra
		tmp_v.x = objets->os->position_finale.x - cam->position.x;
		tmp_v.y = objets->os->position_finale.y - cam->position.y;
		tmp_v.z = objets->os->position_finale.z - cam->position.z;
		APPLIQUER_BASE_3D_M(tmp_v,OFFSET, cam_i,cam_j,cam_k);
		APPLIQUER_BASE_3D_M(objets->os->i_final,I, cam_i,cam_j,cam_k);
		APPLIQUER_BASE_3D_M(objets->os->j_final,J, cam_i,cam_j,cam_k);
		APPLIQUER_BASE_3D_M(objets->os->k_final,K, cam_i,cam_j,cam_k);
		// appliquer ^ à l'enveloppe convexe de l'objet et checker la position des pts par rapport au frustrum + calculer la distance minimale au carré
		compteur_gauche = compteur_droite = compteur_haut = compteur_bas = compteur_derrière = 0;
		d_carre_min = FLT_MAX;
#define CHECKER_PT_ENVELOPPE(i) { APPLIQUER_BASE_3D_M(objets->enveloppe_convexe[(i)],tmp_v, I,J,K); tmp_v.x += OFFSET.x; tmp_v.y += OFFSET.y; tmp_v.z += OFFSET.z; compteur_derrière += (tmp_v.y <= cam->distance_cam_ecran) ? 1 : 0; compteur_gauche += (tmp_v.x*gauche.x + tmp_v.y*gauche.y >= 0) ? 1 : 0; compteur_droite += (tmp_v.x*droite.x + tmp_v.y*droite.y >= 0) ? 1 : 0; compteur_haut += (tmp_v.y*haut.x + tmp_v.z*haut.y >= 0) ? 1 : 0; compteur_bas += (tmp_v.y*bas.x + tmp_v.z*bas.y >= 0) ? 1 : 0; d_carre_min = (PDT_SCALAIRE_3D_M(tmp_v,tmp_v) <= d_carre_min) ? PDT_SCALAIRE_3D_M(tmp_v,tmp_v) : d_carre_min ; }
		CHECKER_PT_ENVELOPPE(0);
		CHECKER_PT_ENVELOPPE(1);
		CHECKER_PT_ENVELOPPE(2);
		CHECKER_PT_ENVELOPPE(3);
		CHECKER_PT_ENVELOPPE(4);
		CHECKER_PT_ENVELOPPE(5);
		CHECKER_PT_ENVELOPPE(6);
		CHECKER_PT_ENVELOPPE(7);
		// checker si objet affichable à l'aide des compteurs précédents
		if (compteur_derrière < 8 && compteur_droite < 8 && compteur_gauche < 8 && compteur_bas < 8 && compteur_haut < 8) {
			// A FAIRE : système de LOD utilisant d_carre_min
			// boucle for sur les pts (dépendante de la distance ^ (LOD))
			for (int i=0; i<objets->N_POINTS; ++i) {
				// appliquer transformation PLUS projection ET calculer distance au carré
				APPLIQUER_BASE_3D_M( objets->tableau_points[i],tmp_v, I,J,K);
				tmp_v.x += OFFSET.x; tmp_v.y += OFFSET.y; tmp_v.z += OFFSET.z;
				x = coef_x*tmp_v.x/tmp_v.y + offset_x;
				y = coef_y*tmp_v.z/tmp_v.y + offset_y;
				tmp_f = PDT_SCALAIRE_3D_M(tmp_v,tmp_v);
				// sauvegarder le résultat dans le zbuffer si possible
				indice = ( 0 <= x && x < cam->largeur_ecran && 0 <= y && y < cam->hauteur_ecran && tmp_v.y >= cam->distance_cam_ecran) ? cam->largeur_ecran*y + x : N_TOTAL;
				visible = (zbuffer[indice] > tmp_f);
				zbuffer[indice] = visible ? tmp_f : zbuffer[indice];
				buffer[indice] = visible ? objets->tableau_couleurs[i] : buffer[indice];
			}
		}

		objets = objets->next;
	}
}