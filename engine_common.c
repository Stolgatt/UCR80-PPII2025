#include "engine_common.h"

static void CALCUL_SQUELETTE_2D_FCT_AUXILIAIRE(OS2D* os) {
	for (int i=0; i<os->nombre_enfants; ++i) {
		APPLIQUER_BASE_2D_M((os->enfants)[i].position, (os->enfants)[i].position_finale, os->i_final, os->j_final);
		(os->enfants)[i].position_finale.x += os->position_finale.x;
		(os->enfants)[i].position_finale.y += os->position_finale.y;
		APPLIQUER_BASE_2D_M((os->enfants)[i].i, (os->enfants)[i].i_final, os->i_final, os->j_final);
		APPLIQUER_BASE_2D_M((os->enfants)[i].j, (os->enfants)[i].j_final, os->i_final, os->j_final);
		CALCUL_SQUELETTE_2D_FCT_AUXILIAIRE(os->enfants+i);
	}
}

void CALCUL_SQUELETTE_2D(OS2D* os_racine) {
	os_racine->position_finale = os_racine->position;
	os_racine->i_final = os_racine->i;
	os_racine->j_final = os_racine->j;
	CALCUL_SQUELETTE_2D_FCT_AUXILIAIRE(os_racine);
}




static void CALCUL_SQUELETTE_3D_FCT_AUXILIAIRE(OS3D* os) {
	for (int i=0; i<os->nombre_enfants; ++i) {
		APPLIQUER_BASE_3D_M((os->enfants)[i].position, (os->enfants)[i].position_finale, os->i_final, os->j_final, os->k_final);
		(os->enfants)[i].position_finale.x += os->position_finale.x;
		(os->enfants)[i].position_finale.y += os->position_finale.y;
		(os->enfants)[i].position_finale.z += os->position_finale.z;
		APPLIQUER_BASE_3D_M((os->enfants)[i].i, (os->enfants)[i].i_final, os->i_final, os->j_final, os->k_final);
		APPLIQUER_BASE_3D_M((os->enfants)[i].j, (os->enfants)[i].j_final, os->i_final, os->j_final, os->k_final);
		APPLIQUER_BASE_3D_M((os->enfants)[i].k, (os->enfants)[i].k_final, os->i_final, os->j_final, os->k_final);
		CALCUL_SQUELETTE_3D_FCT_AUXILIAIRE(os->enfants+i);
	}
}

void CALCUL_SQUELETTE_3D(OS3D* os_racine) {
	os_racine->position_finale = os_racine->position;
	os_racine->i_final = os_racine->i;
	os_racine->j_final = os_racine->j;
	os_racine->k_final = os_racine->k;
	CALCUL_SQUELETTE_3D_FCT_AUXILIAIRE(os_racine);
}
