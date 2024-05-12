#include "graphical_engine.h"

void AFFICHAGE_SPRITES() {
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

	// clear l'écran
	SDL_SetRenderTarget(cam->renderer, cam->tmp_cible);
	SDL_RenderClear(cam->renderer);

	// AFFICHAGE_SKYBOX();

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

	PLAN_HORIZONTAL* ptr = scene->tout_en_haut;
	if (0 < haut_ecran_z) {
		int debut = 0;
		int fin = 0 < bas_ecran_z ? src_rect.h : src_rect.h*(haut_ecran_z)/(haut_ecran_z - bas_ecran_z) - 1; // -1 pour se protéger d'une éventuelle division par 0
		while ((ptr != NULL) && (ptr->position.z > haut_ecran_z + cam->position.z)) {
			// AFFICHAGE_SPRITES(ptr->sprites_au_dessus);

			// préparation du parcours scanline par scanline
			float actuel_y = haut_ecran_y;
			float actuel_z = haut_ecran_z;
			float increment_y = (bas_ecran_y - haut_ecran_y)/((float)src_rect.h);
			float increment_z = (bas_ecran_z - haut_ecran_z)/((float)src_rect.h);
			float hauteur = ptr->position.z - cam->position.z;
			float precedent_y_plan = actuel_y*hauteur/actuel_z;
			VECTEUR2D centre_plan = { (max_l+min_l)*hauteur/(2.*actuel_z),  // on centre le plan sur le sommet du cône de projection (c'est pas vraiment le centre mais baleek)
									  actuel_y*hauteur/actuel_z};
			// traitement préalable de la texture du plan (gère la rotation longitudinale de la caméra, l'échelle et la rotation de la texture)
			SDL_Rect dst;
			dst.w = ptr->source.w*ptr->echelle;
			dst.h = ptr->source.h*ptr->echelle;
			VECTEUR2D v = {ptr->position.x, ptr->position.y};
			APPLIQUER_EN_PLACE_BASE_INVERSE_2D(&v,&i,&j);
			dst.x = TMP_TEXT_W/2. + v.x - centre_plan.x - dst.w/2;
			dst.y = TMP_TEXT_H - v.y + centre_plan.y - dst.h/2; // SDL fait les choses à l'envers, de haut en bas : ici on en tient compte
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
				SDL_RenderCopyEx(cam->renderer,cam->tmp_text,&src,&dst,0,NULL,SDL_FLIP_NONE);
				precedent_y_plan = actuel_y_plan;
			}

			ptr = ptr->en_dessous;
		}
	}

/*
	while ((ptr != NULL) && (ptr->position.z > cam->position.z)) {
		// AFFICHAGE_SPRITES(ptr->sprites_au_dessus);

		// affichage du plan
		ptr = ptr->en_dessous;
	}

	ptr = scene->tout_en_bas;
	TABLEAU_SPRITES* prev_sprites = scene->sprites_tout_en_bas;
	while ((ptr != NULL) && (ptr->position.z < cam->position.z)) {
		// AFFICHAGE_SPRITES(prev_sprites);
		// AFFICHAGE_PLAN_DESSOUS(ptr);
		prev_sprites = ptr->sprites_au_dessus;
		ptr = ptr->au_dessus;
	}
	// AFFICHAGE_SPRITES(prev_sprites);

	while ((ptr != NULL) && (ptr->position.z == cam->position.z)) {
		// AFFICHAGE_SPRITES(ptr->sprites_au_dessus);
		ptr = ptr->au_dessus;
	}
*/

	// rotation roulis final de src_rect/tmp_cible vers dst_rect/cible
	SDL_Rect dst_rect;
	dst_rect.x = 0;
	dst_rect.y = 0;
	dst_rect.w = 1080;
	dst_rect.h = 540;
	SDL_SetRenderTarget(cam->renderer, cam->cible);
	SDL_RenderCopyEx(cam->renderer, cam->tmp_cible, &dst_rect, &dst_rect, 0, NULL, SDL_FLIP_NONE);

}
