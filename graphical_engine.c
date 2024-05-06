#include "graphical_engine.h"

void AFFICHAGE_PLAN_DESSUS() {



}


void AFFICHAGE_PLAN_DESSOUS() {
}


void AFFICHAGE_SPRITES() {
}


void AFFICHAGE_CAMERA(const CAMERA* cam, const SCENE* scene) {

	// déterminer la zone du plan de projection camera qu'il faut rendre
	VECTEUR2D i,j;
	float echelle = 1.;
	CALCUL_BASE_2D(&(cam->roulis),&echelle,&i,&j);
	float min_l,max_l,min_h,max_h;
	VECTEUR2D v;

	v.x = cam->offset_horizontal;
	v.y = cam->offset_vertical;
	APPLIQUER_EN_PLACE_BASE_2D(&v,&i,&j);
	min_l = max_l = v.x;
	min_h = max_h = v.y;

	v.x = cam->offset_horizontal + cam->ecran_final.w/cam->echelle_ecran;
	v.y = cam->offset_vertical;
	APPLIQUER_EN_PLACE_BASE_2D(&v,&i,&j);
	min_l = (v.x < min_l) ? v.x : min_l; max_l = (v.x > max_l) ? v.x : max_l;
	min_h = (v.y < min_h) ? v.y : min_h; max_h = (v.y > max_h) ? v.y : max_h;

	v.x = cam->offset_horizontal;
	v.y = cam->offset_vertical + cam->ecran_final.h/cam->echelle_ecran;
	APPLIQUER_EN_PLACE_BASE_2D(&v,&i,&j);
	min_l = (v.x < min_l) ? v.x : min_l; max_l = (v.x > max_l) ? v.x : max_l;
	min_h = (v.y < min_h) ? v.y : min_h; max_h = (v.y > max_h) ? v.y : max_h;

	v.x = cam->offset_horizontal + cam->ecran_final.w/cam->echelle_ecran;
	v.y = cam->offset_vertical + cam->ecran_final.h/cam->echelle_ecran;
	APPLIQUER_EN_PLACE_BASE_2D(&v,&i,&j);
	min_l = (v.x < min_l) ? v.x : min_l; max_l = (v.x > max_l) ? v.x : max_l;
	min_h = (v.y < min_h) ? v.y : min_h; max_h = (v.y > max_h) ? v.y : max_h;

	// AFFICHAGE_SKYBOX();

	PLAN_HORIZONTAL* ptr = scene->tout_en_haut;
	while ((ptr != NULL) && (ptr->position.z > cam->position.z)) {
		// AFFICHAGE_SPRITES(ptr->sprites_au_dessus);
		// AFFICHAGE_PLAN_DESSUS(ptr);
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

	// rotation roulis final

}
