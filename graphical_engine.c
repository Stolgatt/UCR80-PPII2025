#include "graphical_engine.h"

struct PRECALCULS {
	VECTEUR3D i;
	VECTEUR3D j;
	VECTEUR3D k;
	VECTEUR3D cam_pos;

	float min_l;
	float max_l;
	float min_h;
	float max_h;

	float xcoef;
	float ycoef;
};
typedef struct PRECALCULS PRECALCULS;

inline void TRI_INSERTION(Z_SPRITE* tableau, unsigned int debut, unsigned int fin) {

	for (unsigned int i=debut+1; i<fin; ++i) {
		unsigned int j;
		Z_SPRITE tmp;
		j=debut;
		while (tableau[i].Z <= tableau[j].Z && j<i) j++;
		for (; j<i; ++j) {
			tmp = tableau[j];
			tableau[j] = tableau[i];
			tableau[i] = tmp;
		}
	}

}

inline void hopla(Z_SPRITE* tableau, unsigned int i, unsigned int j) {
	Z_SPRITE tmp = tableau[i];
	tableau[i] = tableau[j];
	tableau[j] = tmp;
}

void TRI_RAPIDE(Z_SPRITE* tableau, unsigned int debut, unsigned int fin) {

	if (fin-debut < 20)
		TRI_INSERTION(tableau,debut,fin);
	else {
		hopla(tableau,(fin/debut + debut*fin + (unsigned int)(tableau))%(fin-debut) + debut,fin-1);
		unsigned int index_bas = debut;
		unsigned int index_haut = fin-2;
		while (index_bas < index_haut) {
			if (tableau[index_bas].Z >= tableau[fin-1].Z) index_bas++;
			else {
				while (index_bas < index_haut) {
					if (tableau[index_haut].Z <= tableau[fin-1].Z) index_haut--;
					else {
						hopla(tableau,index_bas,index_haut);
						break;
					}
				}
			}
		}
		if (tableau[index_bas].Z >= tableau[fin-1].Z) {
			hopla(tableau,index_bas+1,fin-1);
			TRI_RAPIDE(tableau,debut,index_bas+1);
			TRI_RAPIDE(tableau,index_bas+1,fin);
		}
		else {
			hopla(tableau,index_bas,fin-1);
			TRI_RAPIDE(tableau,debut,index_bas);
			TRI_RAPIDE(tableau,index_bas,fin);
		}

	}

}

void AFFICHAGE_SPRITES(const TABLEAU_SPRITES* sprites, const CAMERA* cam, const PRECALCULS* precalc) {

	unsigned int n = 0;

	// projection de tous les sprites
	while (sprites != NULL && n < cam->N_MAX) {
		unsigned int i_final = cam->N_MAX - n >= sprites->N ? sprites->N : cam->N_MAX - n;
		for (unsigned int i=0; i<i_final; ++i) {
			VECTEUR3D pos;
			// calcul de la position du sprite dans l'espace caméra
			pos = sprites->sprites[i].position;
			APPLIQUER_EN_PLACE_BASE_INVERSE_3D(&pos,&precalc->i,&precalc->j,&precalc->k);
			pos.x -= precalc->cam_pos.x;
			pos.y -= precalc->cam_pos.y;
			pos.z -= precalc->cam_pos.z;
			// test de visibilité 
			if (pos.y >= cam->distance_ecran
				&& (pos.x + sprites->sprites[i].echelle*sprites->sprites[i].source.w*cam->distance_ecran/(2.*pos.y))*cam->distance_ecran - pos.y*precalc->min_l >= 0.
				&& pos.y*precalc->max_l - (pos.x - sprites->sprites[i].echelle*sprites->sprites[i].source.w*cam->distance_ecran/(2.*pos.y))*cam->distance_ecran >= 0.
				&& (pos.z + sprites->sprites[i].echelle*sprites->sprites[i].source.h*cam->distance_ecran/(2.*pos.y))*cam->distance_ecran - pos.y*precalc->min_h >= 0.
				&& pos.y*precalc->max_h - (pos.z - sprites->sprites[i].echelle*sprites->sprites[i].source.h*cam->distance_ecran/(2.*pos.y))*cam->distance_ecran >= 0.)
			{
				// calcul des information nécessaires au rendercopy
				cam->tableau_z[n].index = n;
				cam->tableau_z[n].Z = pos.y;
				cam->tableau_p[n].texture = sprites->sprites[i].texture;
				cam->tableau_p[n].source = sprites->sprites[i].source;
				cam->tableau_p[n].dest.w = sprites->sprites[i].echelle*sprites->sprites[i].source.w*cam->distance_ecran/pos.y;
				cam->tableau_p[n].dest.h = sprites->sprites[i].echelle*sprites->sprites[i].source.h*cam->distance_ecran/pos.y;
				cam->tableau_p[n].dest.x = (pos.x*cam->distance_ecran/pos.y - precalc->min_l)*precalc->xcoef - cam->tableau_p[n].dest.w/2.;
				cam->tableau_p[n].dest.y = (pos.z*cam->distance_ecran/pos.y - precalc->max_h)*precalc->ycoef - cam->tableau_p[n].dest.h/2.;
				n++;
			}
		}

		sprites = sprites->suivant;
	}

	// tri de tous les Z_SPRITEs (suivant Z :)
	TRI_INSERTION(cam->tableau_z,0,n);

	// parcours des Z_SPRITEs et affichage des sprites correspondants
	for (unsigned int i=0; i<n; ++i) {
		SDL_RenderCopy(cam->renderer,cam->tableau_p[cam->tableau_z[i].index].texture,&cam->tableau_p[cam->tableau_z[i].index].source,&cam->tableau_p[cam->tableau_z[i].index].dest);
	}

}


void AFFICHAGE_CAMERA(const CAMERA* cam, const SCENE* scene) {

	// déterminer la zone du plan de projection camera qu'il faut rendre
	VECTEUR2D i,j;
	float echelle = 1.;
	CALCUL_BASE_2D(&(cam->roulis),&echelle,&i,&j);
	float min_l,max_l,min_h,max_h;
	VECTEUR2D v;

	v.x = cam->offset_horizontal - cam->dimension_cible.w*cam->echelle_ecran/2.;
	v.y = cam->offset_vertical - cam->dimension_cible.h*cam->echelle_ecran/2.;
	APPLIQUER_EN_PLACE_BASE_2D(&v,&i,&j);
	min_l = max_l = v.x;
	min_h = max_h = v.y;

	v.x = cam->offset_horizontal + cam->dimension_cible.w*cam->echelle_ecran/2.;
	v.y = cam->offset_vertical - cam->dimension_cible.h*cam->echelle_ecran/2.;
	APPLIQUER_EN_PLACE_BASE_2D(&v,&i,&j);
	min_l = (v.x < min_l) ? v.x : min_l; max_l = (v.x > max_l) ? v.x : max_l;
	min_h = (v.y < min_h) ? v.y : min_h; max_h = (v.y > max_h) ? v.y : max_h;

	v.x = cam->offset_horizontal - cam->dimension_cible.w*cam->echelle_ecran/2.;
	v.y = cam->offset_vertical + cam->dimension_cible.h*cam->echelle_ecran/2.;
	APPLIQUER_EN_PLACE_BASE_2D(&v,&i,&j);
	min_l = (v.x < min_l) ? v.x : min_l; max_l = (v.x > max_l) ? v.x : max_l;
	min_h = (v.y < min_h) ? v.y : min_h; max_h = (v.y > max_h) ? v.y : max_h;

	v.x = cam->offset_horizontal + cam->dimension_cible.w*cam->echelle_ecran/2.;
	v.y = cam->offset_vertical + cam->dimension_cible.h*cam->echelle_ecran/2.;
	APPLIQUER_EN_PLACE_BASE_2D(&v,&i,&j);
	min_l = (v.x < min_l) ? v.x : min_l; max_l = (v.x > max_l) ? v.x : max_l;
	min_h = (v.y < min_h) ? v.y : min_h; max_h = (v.y > max_h) ? v.y : max_h;

/*	zone du plan écran qui sera rendu (sur cam->tmp_cible)
	  +-------- max_h --------+
	  |						  |
	min_l					max_l
	  |						  |
	  +-------- min_h  -------+

	l'unité de distance est celle "du monde"
	c'est pourquoi src_rect indique (en unité écran, càd en pixels) le rectangle correspondant à cette zone rendue (sur cam->tmp_cible)
*/
	SDL_Rect src_rect;
	src_rect.x = src_rect.y = 0;
	src_rect.w = (max_l-min_l)/cam->echelle_ecran;
	src_rect.h = (max_h-min_h)/cam->echelle_ecran;

	// précalculs nécessaires pour l'affichage des plans
	int TMP_TEXT_W,TMP_TEXT_H;
	SDL_QueryTexture(cam->tmp_text,NULL,NULL,&TMP_TEXT_W,&TMP_TEXT_H);
	// calcul de la rotation longitudinnale de la caméra (pour le prétraitement des textures plans)
	CALCUL_BASE_2D(&cam->longitude,&echelle,&i,&j);
	// calcul des coordonnées des 4 sommets de la zone écran DANS LE REPERE MONDE
	VECTEUR2D i2,j2;
	CALCUL_BASE_2D(&cam->latitude,&echelle,&i2,&j2);
	float haut_ecran_y = cam->distance_ecran*i2.x + max_h*j2.x;
	float haut_ecran_z = cam->distance_ecran*i2.y + max_h*j2.y;
	float bas_ecran_y = cam->distance_ecran*i2.x + min_h*j2.x;
	float bas_ecran_z = cam->distance_ecran*i2.y + min_h*j2.y;

	// précalculs nécessaires pour le rendu des sprites
	// matrice de rotation caméra (sans le roulis)
	PRECALCULS precalc;
	QUATERNION q1,q2;
	VECTEUR3D axe = {0.,0.,1.};
	CALCUL_QUATERNION(cam->longitude,&axe,&q1);
	axe.x = 1.; axe.z = 0.;
	CALCUL_QUATERNION(cam->latitude,&axe,&q2);
	MULT_QUATERNIONS_EN_PLACE_GAUCHE(&q1,&q2);
	CALCUL_BASE_3D(&q1,&echelle,&precalc.i,&precalc.j,&precalc.k);
	// position caméra
	precalc.cam_pos = cam->position;
	APPLIQUER_EN_PLACE_BASE_INVERSE_3D(&precalc.cam_pos,&precalc.i,&precalc.j,&precalc.k);
	//  info nécessaires au test visibilité (hyperplans ;)
	precalc.min_l = min_l;
	precalc.max_l = max_l;
	precalc.min_h = min_h;
	precalc.max_h = max_h;
	precalc.xcoef = src_rect.w/(max_l - min_l);
	precalc.ycoef = src_rect.h/(min_h - max_h);

	// clear l'écran
	SDL_SetRenderTarget(cam->renderer, cam->tmp_cible);
	SDL_RenderClear(cam->renderer);

	// affichage skybox
	SDL_Rect skybox_rect_source, skybox_rect_dest;
	SDL_QueryTexture(scene->skybox,NULL,NULL,&skybox_rect_source.w,&skybox_rect_source.h);
	skybox_rect_source.w /= 2;
	skybox_rect_source.x = skybox_rect_source.w;
	float left_angle = scene->skybox_rotation - cam->longitude + atanf(min_l/cam->distance_ecran);
	float right_angle = scene->skybox_rotation - cam->longitude + atanf(max_l/cam->distance_ecran);
	skybox_rect_source.w *= (right_angle - left_angle)/(2.*M_PI);
	while (left_angle < 0) left_angle += 2.*M_PI;
	while (left_angle >= 2.*M_PI) left_angle -= 2.*M_PI;
	skybox_rect_source.x *= left_angle/(2.*M_PI);
	skybox_rect_source.y = 0;
	skybox_rect_dest.w = src_rect.w;
	skybox_rect_dest.x = 0;
	skybox_rect_dest.h = skybox_rect_source.h;
	skybox_rect_dest.y = max_h/cam->echelle_ecran - 3*cam->dimension_cible.h/5; // complètement inccorect mais suffisant si -1 <<< latitude <<< 1
	SDL_RenderCopy(cam->renderer,scene->skybox,&skybox_rect_source,&skybox_rect_dest);

	PLAN_HORIZONTAL* ptr = scene->tout_en_haut;
	if (haut_ecran_z > 0) {
		while ((ptr != NULL) && (ptr->position.z > cam->position.z)) {
			AFFICHAGE_SPRITES(ptr->sprites_au_dessus, cam, &precalc);

			// AFFICHAGE PLAN HAUT;
			float hauteur = ptr->position.z - cam->position.z;
			if (hauteur > bas_ecran_z) {
				// préparation du parcours scanline par scanline
				int fin = 0 < bas_ecran_z ? src_rect.h : src_rect.h*(haut_ecran_z)/(haut_ecran_z - bas_ecran_z) - 1; // -1 pour se protéger d'une éventuelle division par 0 (dernière scanline presque parallèle au plan)
				float actuel_z,actuel_y;
				int debut;
				if (hauteur > haut_ecran_z) { // idem, protection contre les divisions par 0 (si le plan de projection est parallèle au plan (caméra braquée vers le haut))
					actuel_z = haut_ecran_z;
					actuel_y = haut_ecran_y;
					debut = 0;
				}
				else {
					actuel_z = hauteur;
					actuel_y = (actuel_z - haut_ecran_z)*(bas_ecran_y - haut_ecran_y)/(bas_ecran_z - haut_ecran_z) + haut_ecran_y;
					debut = src_rect.h*(haut_ecran_z - actuel_z)/(haut_ecran_z - bas_ecran_z);
				}
				float increment_y = (bas_ecran_y - haut_ecran_y)/((float)src_rect.h);
				float increment_z = (bas_ecran_z - haut_ecran_z)/((float)src_rect.h);
				float precedent_y_plan = actuel_y*hauteur/actuel_z;
				VECTEUR2D centre_plan = { (max_l+min_l)*hauteur/(2.*actuel_z),  // on centre le plan sur le sommet du cône de projection (c'est pas vraiment le centre mais baleek)
										  actuel_y*hauteur/actuel_z};
				// traitement préalable de la texture du plan (gère la rotation longitudinale de la caméra, l'échelle et la rotation de la texture)
				SDL_Rect dst;
				dst.w = ptr->source.w*ptr->echelle;
				dst.h = ptr->source.h*ptr->echelle;
				VECTEUR2D v = {ptr->position.x - cam->position.x, ptr->position.y - cam->position.y};
				APPLIQUER_EN_PLACE_BASE_INVERSE_2D(&v,&i,&j);
				dst.x = TMP_TEXT_W/2. + v.x - centre_plan.x - dst.w/2.;
				dst.y = TMP_TEXT_H - v.y + centre_plan.y - dst.h/2.; // SDL fait les choses à l'envers, de haut en bas : ici on en tient compte
				SDL_SetRenderTarget(cam->renderer, cam->tmp_text);
				SDL_RenderClear(cam->renderer);
				SDL_RenderCopyEx(cam->renderer,ptr->texture,&ptr->source,&dst,RAD2DEG(cam->longitude - ptr->rotation),NULL,SDL_FLIP_NONE);

				SDL_SetRenderTarget(cam->renderer, cam->tmp_cible);
				// affichage du plan scanline par scanline
				for (int k=debut; k<fin; k++) {
					float actuel_y_plan;
					SDL_Rect src,dst;
					actuel_y += increment_y;
					actuel_z += increment_z;
					actuel_y_plan = actuel_y*(hauteur/actuel_z);
					src.y = TMP_TEXT_H - actuel_y_plan + centre_plan.y; // tient compte de l'inversion de l'axe le long de la hauteur des images dûe à SDL
					// if (src.y < 0) break; // supposé supprimer certains artefacts à l'horizon, pas de différence ni de perf ni graphique en pratique
					src.x = 0;
					src.w = TMP_TEXT_W;
					src.h = actuel_y_plan - precedent_y_plan;
					src.h = src.h > 1 ? src.h : 1; // pour éviter les lignes vides si l'arrondi n'est pas favorable
					dst.x = ((centre_plan.x - TMP_TEXT_W/2.)*actuel_z/hauteur - min_l)/cam->echelle_ecran;
					dst.w = ((centre_plan.x + TMP_TEXT_W/2.)*actuel_z/hauteur - min_l)/cam->echelle_ecran - dst.x;
					dst.y = k;
					dst.h = 1;
					SDL_RenderCopy(cam->renderer,cam->tmp_text,&src,&dst);
					precedent_y_plan = actuel_y_plan;
				}
			}

			ptr = ptr->en_dessous;
		}
	}

	ptr = scene->tout_en_bas;
	TABLEAU_SPRITES* prev_sprites = scene->sprites_tout_en_bas;
	if (bas_ecran_z < 0) {
		while ((ptr != NULL) && (ptr->position.z < cam->position.z)) {
			AFFICHAGE_SPRITES(prev_sprites, cam, &precalc);

			// AFFICHAGE PLAN BAS;
			float hauteur = ptr->position.z - cam->position.z;
			if (hauteur < haut_ecran_z) {
				// préparation du parcours scanline par scanline
				int fin = 0 > haut_ecran_z ? -1 : src_rect.h*(haut_ecran_z)/(haut_ecran_z - bas_ecran_z) + 1; // +1 pour se protéger d'une éventuelle division par 0 (dernière scanline presque parallèle au plan)
				float actuel_z,actuel_y;
				int debut;
				if (hauteur < bas_ecran_z) { // idem, protection contre les divisions par 0 (si le plan de projection est parallèle au plan (caméra braquée vers le haut))
					actuel_z = bas_ecran_z;
					actuel_y = bas_ecran_y;
					debut = src_rect.h - 1;
				}
				else {
					actuel_z = hauteur;
					actuel_y = (actuel_z - bas_ecran_z)*(haut_ecran_y - bas_ecran_y)/(haut_ecran_z - bas_ecran_z) + bas_ecran_y;
					debut = src_rect.h*(haut_ecran_z - actuel_z)/(haut_ecran_z - bas_ecran_z);
				}
				float increment_y = (haut_ecran_y - bas_ecran_y)/((float)src_rect.h);
				float increment_z = (haut_ecran_z - bas_ecran_z)/((float)src_rect.h);
				float precedent_y_plan = actuel_y*hauteur/actuel_z;
				VECTEUR2D centre_plan = { (max_l+min_l)*hauteur/(2.*actuel_z),  // on centre le plan sur le sommet du cône de projection (c'est pas vraiment le centre mais baleek)
										  actuel_y*hauteur/actuel_z};
				// traitement préalable de la texture du plan (gère la rotation longitudinale de la caméra, l'échelle et la rotation de la texture)
				SDL_Rect dst;
				dst.w = ptr->source.w*ptr->echelle;
				dst.h = ptr->source.h*ptr->echelle;
				VECTEUR2D v = {ptr->position.x - cam->position.x, ptr->position.y - cam->position.y};
				APPLIQUER_EN_PLACE_BASE_INVERSE_2D(&v,&i,&j);
				dst.x = TMP_TEXT_W/2. + v.x - centre_plan.x - dst.w/2.;
				dst.y = TMP_TEXT_H - v.y + centre_plan.y - dst.h/2.; // SDL fait les choses à l'envers, de haut en bas : ici on en tient compte
				SDL_SetRenderTarget(cam->renderer, cam->tmp_text);
				SDL_RenderClear(cam->renderer);
				SDL_RenderCopyEx(cam->renderer,ptr->texture,&ptr->source,&dst,RAD2DEG(cam->longitude - ptr->rotation),NULL,SDL_FLIP_NONE);

				SDL_SetRenderTarget(cam->renderer, cam->tmp_cible);
				// affichage du plan scanline par scanline
				for (int k=debut; k>fin; k--) {
					float actuel_y_plan;
					SDL_Rect src,dst;
					actuel_y += increment_y;
					actuel_z += increment_z;
					actuel_y_plan = actuel_y*(hauteur/actuel_z);
					src.y = TMP_TEXT_H - actuel_y_plan + centre_plan.y; // tient compte de l'inversion de l'axe le long de la hauteur des images dûe à SDL
					// if (src.y < 0) break; // supposé supprimer certains artefacts à l'horizon, pas de différence ni de perf ni graphique en pratique
					src.x = 0;
					src.w = TMP_TEXT_W;
					src.h = actuel_y_plan - precedent_y_plan;
					src.h = src.h > 1 ? src.h : 1; // pour éviter les lignes vides si l'arrondi n'est pas favorable
					dst.x = ((centre_plan.x - TMP_TEXT_W/2.)*actuel_z/hauteur - min_l)/cam->echelle_ecran;
					dst.w = ((centre_plan.x + TMP_TEXT_W/2.)*actuel_z/hauteur - min_l)/cam->echelle_ecran - dst.x;
					dst.y = k;
					dst.h = 1;
					SDL_RenderCopy(cam->renderer,cam->tmp_text,&src,&dst);
					precedent_y_plan = actuel_y_plan;
				}
			}

			prev_sprites = ptr->sprites_au_dessus;
			ptr = ptr->au_dessus;
		}
	}
	else {
		while ((ptr != NULL) && (ptr->position.z < cam->position.z)) {
			prev_sprites = ptr->sprites_au_dessus;
			ptr = ptr->au_dessus;
		}
	}
	AFFICHAGE_SPRITES(prev_sprites, cam, &precalc);

	while ((ptr != NULL) && (ptr->position.z == cam->position.z)) {
		AFFICHAGE_SPRITES(ptr->sprites_au_dessus, cam, &precalc);
		ptr = ptr->au_dessus;
	}

	// rotation roulis finale de src_rect/tmp_cible vers dst_rect/tmp_text puis copie finale sur la tmp_cible
	SDL_Rect dst_rect;
	dst_rect.x = (min_l - cam->offset_horizontal)/cam->echelle_ecran + cam->dimension_cible.w/2.;
	dst_rect.y = (cam->offset_vertical - max_h)/cam->echelle_ecran + cam->dimension_cible.h/2.;
	dst_rect.w = src_rect.w;
	dst_rect.h = src_rect.h;
	SDL_Point pt;
	pt.x = -min_l/cam->echelle_ecran;
	pt.y = max_h/cam->echelle_ecran;

	SDL_SetRenderTarget(cam->renderer,cam->tmp_text);
	SDL_RenderClear(cam->renderer);
	SDL_RenderCopyEx(cam->renderer, cam->tmp_cible, &src_rect, &dst_rect, RAD2DEG(cam->roulis), &pt, SDL_FLIP_NONE);
	SDL_SetRenderTarget(cam->renderer,cam->cible);
	SDL_RenderCopy(cam->renderer,cam->tmp_text,&cam->dimension_cible,&cam->dimension_cible);

}
