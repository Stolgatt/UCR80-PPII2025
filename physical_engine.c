#include "physical_engine.h"

void Afficher_Monde_Physique(MONDE_PHYSIQUE* monde) {
	// affichage monde
	AFFICHAGE_CAMERA(&monde->cam,&monde->scene);
	// timer
	char timerText[22];
    snprintf(timerText, sizeof(timerText), "%02lld:%02lld:%02lld", monde->timer/60000, (monde->timer % 60000)/1000, (monde->timer % 1000)/10 );
	SDL_Surface* tmp_surf = TTF_RenderText_Solid(monde->police, timerText, monde->timer_color);
	SDL_Texture* tmp_text = SDL_CreateTextureFromSurface(monde->cam.renderer,tmp_surf);
	SDL_Rect tmp_rect;
	tmp_rect.x = monde->timer_position.x;
	tmp_rect.y = monde->timer_position.y;
	SDL_QueryTexture(tmp_text,NULL,NULL,&tmp_rect.w,&tmp_rect.h);
	SDL_RenderCopy(monde->cam.renderer,tmp_text,NULL,&tmp_rect);
	SDL_FreeSurface(tmp_surf);
	SDL_DestroyTexture(tmp_text);
	// affichage minimap
	SDL_RenderCopy(monde->cam.renderer,monde->minimap,NULL,&monde->minimap_rect);
}

void Charger_Monde_Physique(MONDE_PHYSIQUE* monde, const NIVEAU* niveau, const CONTEXTE_SDL* contexte) {

	// initialisation segments et grille
	monde->nb_colonnes = niveau->nb_colonnes;
	monde->nb_lignes = niveau->nb_lignes;
	monde->l = niveau->l;
	monde->h = niveau->h;
	monde->grille = calloc(monde->nb_colonnes*monde->nb_lignes+1,sizeof(CASE_GRILLE)); // le +1 est très important
	monde->grille_voitures = calloc(monde->nb_colonnes*monde->nb_lignes*niveau->nb_voitures,sizeof(unsigned short int));
	// premier parcours des segments pour compter le nb de segments (déterminer la taille du tableau)
	unsigned int nb_total_segments = 0;
	for (unsigned short int k=0; k<niveau->nb_tableaux; ++k) {
		for (unsigned int i=0; i<niveau->tailles_tableaux[k]; ++i) {
			// détermination bouding box du segment
			float max_x = niveau->tableaux_x[k][i] > niveau->tableaux_x[k][(i+1)%niveau->tailles_tableaux[k]] ? niveau->tableaux_x[k][i] : niveau->tableaux_x[k][(i+1)%niveau->tailles_tableaux[k]];
			float max_y = niveau->tableaux_y[k][i] > niveau->tableaux_y[k][(i+1)%niveau->tailles_tableaux[k]] ? niveau->tableaux_y[k][i] : niveau->tableaux_y[k][(i+1)%niveau->tailles_tableaux[k]];
			float min_x = niveau->tableaux_x[k][i] < niveau->tableaux_x[k][(i+1)%niveau->tailles_tableaux[k]] ? niveau->tableaux_x[k][i] : niveau->tableaux_x[k][(i+1)%niveau->tailles_tableaux[k]];
			float min_y = niveau->tableaux_y[k][i] < niveau->tableaux_y[k][(i+1)%niveau->tailles_tableaux[k]] ? niveau->tableaux_y[k][i] : niveau->tableaux_y[k][(i+1)%niveau->tailles_tableaux[k]];
			// ajout d'une unité (comptage) dans les cases de la grille intersectant cette bounding box
			for (unsigned int col=min_x*monde->nb_colonnes/monde->l; col<max_x*monde->nb_colonnes/monde->l + 1; ++col) {
				for (unsigned int lig=min_y*monde->nb_lignes/monde->h; lig<max_y*monde->nb_lignes/monde->h + 1; ++lig) {
					monde->grille[lig*monde->nb_colonnes + col].indice_debut_segments++;
					nb_total_segments++;
				}
			}
		}
	}
	// maintenant qu'on a la taille, on peut allouer
	monde->grille_pos_segments = calloc(nb_total_segments,sizeof(VECTEUR2D));
	monde->grille_segments = calloc(nb_total_segments,sizeof(SEGMENT2D));
	// on répartit les segments dans les cases
	nb_total_segments = 0;
	for (unsigned int i=0; i<=monde->nb_colonnes*monde->nb_lignes; ++i) {
		unsigned int tmp;
		tmp = monde->grille[i].indice_debut_segments;
		monde->grille[i].indice_debut_segments = nb_total_segments;
		nb_total_segments += tmp;
	}
	// deuxième parcours pour calculer les segments
	for (unsigned short int k=0; k<niveau->nb_tableaux; ++k) {
		if (niveau->sens[k] > 0)
			for (unsigned int i=0; i<niveau->tailles_tableaux[k]; ++i) {
				// détermination bouding box du segment
				float max_x = niveau->tableaux_x[k][i] > niveau->tableaux_x[k][(i+1)%niveau->tailles_tableaux[k]] ? niveau->tableaux_x[k][i] : niveau->tableaux_x[k][(i+1)%niveau->tailles_tableaux[k]];
				float max_y = niveau->tableaux_y[k][i] > niveau->tableaux_y[k][(i+1)%niveau->tailles_tableaux[k]] ? niveau->tableaux_y[k][i] : niveau->tableaux_y[k][(i+1)%niveau->tailles_tableaux[k]];
				float min_x = niveau->tableaux_x[k][i] < niveau->tableaux_x[k][(i+1)%niveau->tailles_tableaux[k]] ? niveau->tableaux_x[k][i] : niveau->tableaux_x[k][(i+1)%niveau->tailles_tableaux[k]];
				float min_y = niveau->tableaux_y[k][i] < niveau->tableaux_y[k][(i+1)%niveau->tailles_tableaux[k]] ? niveau->tableaux_y[k][i] : niveau->tableaux_y[k][(i+1)%niveau->tailles_tableaux[k]];
				// ajout du segment pour chaque case de la grille intersectant cette bounding box
				for (unsigned int col=min_x*monde->nb_colonnes/monde->l; col<max_x*monde->nb_colonnes/monde->l+1; ++col) {
					for (unsigned int lig=min_y*monde->nb_lignes/monde->h; lig<max_y*monde->nb_lignes/monde->h+1; ++lig) {
						monde->grille_pos_segments[monde->grille[lig*monde->nb_colonnes + col].indice_debut_segments + monde->grille[lig*monde->nb_colonnes + col].nb_voitures].x = niveau->tableaux_x[k][i];
						monde->grille_pos_segments[monde->grille[lig*monde->nb_colonnes + col].indice_debut_segments + monde->grille[lig*monde->nb_colonnes + col].nb_voitures].y = niveau->tableaux_y[k][i];
						monde->grille_segments[monde->grille[lig*monde->nb_colonnes + col].indice_debut_segments + monde->grille[lig*monde->nb_colonnes + col].nb_voitures] = CREA_SEGMENT_2D(niveau->tableaux_x[k][i],niveau->tableaux_x[k][(i+1)%niveau->tailles_tableaux[k]],
												niveau->tableaux_y[k][i],niveau->tableaux_y[k][(i+1)%niveau->tailles_tableaux[k]]);
						monde->grille[lig*monde->nb_colonnes + col].nb_voitures++;
					}
				}
			}
		else
			for (unsigned int i=0; i<niveau->tailles_tableaux[k]; ++i) {
				// détermination bouding box du segment
				float max_x = niveau->tableaux_x[k][i] > niveau->tableaux_x[k][(i+1)%niveau->tailles_tableaux[k]] ? niveau->tableaux_x[k][i] : niveau->tableaux_x[k][(i+1)%niveau->tailles_tableaux[k]];
				float max_y = niveau->tableaux_y[k][i] > niveau->tableaux_y[k][(i+1)%niveau->tailles_tableaux[k]] ? niveau->tableaux_y[k][i] : niveau->tableaux_y[k][(i+1)%niveau->tailles_tableaux[k]];
				float min_x = niveau->tableaux_x[k][i] < niveau->tableaux_x[k][(i+1)%niveau->tailles_tableaux[k]] ? niveau->tableaux_x[k][i] : niveau->tableaux_x[k][(i+1)%niveau->tailles_tableaux[k]];
				float min_y = niveau->tableaux_y[k][i] < niveau->tableaux_y[k][(i+1)%niveau->tailles_tableaux[k]] ? niveau->tableaux_y[k][i] : niveau->tableaux_y[k][(i+1)%niveau->tailles_tableaux[k]];
				// ajout du segment pour chaque case de la grille intersectant cette bounding box
				for (unsigned int col=min_x*monde->nb_colonnes/monde->l; col<max_x*monde->nb_colonnes/monde->l+1; ++col) {
					for (unsigned int lig=min_y*monde->nb_lignes/monde->h; lig<max_y*monde->nb_lignes/monde->h+1; ++lig) {
						monde->grille_pos_segments[monde->grille[lig*monde->nb_colonnes + col].indice_debut_segments + monde->grille[lig*monde->nb_colonnes + col].nb_voitures].x = niveau->tableaux_x[k][(i+1)%niveau->tailles_tableaux[k]];
						monde->grille_pos_segments[monde->grille[lig*monde->nb_colonnes + col].indice_debut_segments + monde->grille[lig*monde->nb_colonnes + col].nb_voitures].y = niveau->tableaux_y[k][(i+1)%niveau->tailles_tableaux[k]];
						monde->grille_segments[monde->grille[lig*monde->nb_colonnes + col].indice_debut_segments + monde->grille[lig*monde->nb_colonnes + col].nb_voitures] = CREA_SEGMENT_2D(niveau->tableaux_x[k][(i+1)%niveau->tailles_tableaux[k]],niveau->tableaux_x[k][i],
												niveau->tableaux_y[k][(i+1)%niveau->tailles_tableaux[k]],niveau->tableaux_y[k][i]);
						monde->grille[lig*monde->nb_colonnes + col].nb_voitures++;
					}
				}
			}
	}
	// on réinitialise nb_voitures qui était utilisé comme variable temporaire
	for (unsigned int i=0; i<=monde->nb_colonnes*monde->nb_lignes; ++i)
		monde->grille[i].nb_voitures = 0;

	// initialisation plans
	monde->scene.sprites_tout_en_bas = NULL;
	PLAN_HORIZONTAL** ptr_plan = &monde->scene.tout_en_bas;
	PLAN_HORIZONTAL* ptr_plan_prev = NULL;
	for (unsigned short int i=0; i<niveau->nb_sols; ++i) {
		*ptr_plan = malloc(sizeof(PLAN_HORIZONTAL));
		(*ptr_plan)->texture = contexte->tableau_textures[niveau->texture_ids_sols[i]];
		(*ptr_plan)->rotation = niveau->angles_initiaux_sols[i];
		(*ptr_plan)->echelle = niveau->echelles_sols[i];
		(*ptr_plan)->position.x = niveau->positions_initiales_sols[i].x;
		(*ptr_plan)->position.y = niveau->positions_initiales_sols[i].y;
		(*ptr_plan)->position.z = 0.;
		(*ptr_plan)->source.x = (*ptr_plan)->source.y = 0;
		SDL_QueryTexture((*ptr_plan)->texture,NULL,NULL,&(*ptr_plan)->source.w,&(*ptr_plan)->source.h);
		(*ptr_plan)->sprites_au_dessus = NULL;
		(*ptr_plan)->en_dessous = ptr_plan_prev;
		ptr_plan_prev = *ptr_plan;
		ptr_plan = &(*ptr_plan)->au_dessus;
	}
	ptr_plan_prev->au_dessus = NULL;
	monde->scene.tout_en_haut = ptr_plan_prev;
	ptr_plan_prev->sprites_au_dessus = malloc(sizeof(TABLEAU_SPRITES));
	ptr_plan_prev->sprites_au_dessus->suivant = NULL;
	ptr_plan_prev->sprites_au_dessus->N = niveau->nb_voitures + niveau->nb_decors;
	ptr_plan_prev->sprites_au_dessus->sprites = calloc(ptr_plan_prev->sprites_au_dessus->N,sizeof(SPRITE));
	unsigned int index_sprites = 0;

	// initialisation sprites
	for (unsigned int i=0; i<niveau->nb_decors; ++i,++index_sprites) {
		ptr_plan_prev->sprites_au_dessus->sprites[index_sprites].texture = contexte->tableau_textures[niveau->texture_ids_dec[i]];
		ptr_plan_prev->sprites_au_dessus->sprites[index_sprites].echelle = niveau->echelles_dec[i];
		ptr_plan_prev->sprites_au_dessus->sprites[index_sprites].position = niveau->positions_dec[i];
		// TODO gestion animation/spritesheets
		ptr_plan_prev->sprites_au_dessus->sprites[index_sprites].source.x = ptr_plan_prev->sprites_au_dessus->sprites[index_sprites].source.y = 0;
		SDL_QueryTexture(contexte->tableau_textures[niveau->texture_ids_dec[i]],NULL,NULL,&ptr_plan_prev->sprites_au_dessus->sprites[index_sprites].source.w,&ptr_plan_prev->sprites_au_dessus->sprites[index_sprites].source.h);
	}

	// initialisation voitures
	monde->nb_voitures = niveau->nb_voitures;
	monde->voitures = calloc(monde->nb_voitures,sizeof(VOITURE));
	// voitures
	for (unsigned short int i=0; i<niveau->nb_voitures; ++i) {
		monde->voitures[i].nombre_disques = 1;
		monde->voitures[i].tableau_rayons = niveau->rayons_voit+i;
		monde->voitures[i].tableau_centres = calloc(1,sizeof(VECTEUR2D));
		monde->voitures[i].min_x = monde->voitures[i].min_y = -(monde->voitures[i].max_x = monde->voitures[i].max_y = niveau->rayons_voit[i]);
		monde->voitures[i].position.x = niveau->positions_initiales_voit[i].x;
		monde->voitures[i].position.y = niveau->positions_initiales_voit[i].y;
		monde->voitures[i].angle = niveau->angles_initiaux_voit[i];
		// l'axe de départ est l'axe des y (car la caméra est centré dessus, ça facilite les choses)
		sincosf(monde->voitures[i].angle,&monde->voitures[i].vect_rotation.x,&monde->voitures[i].vect_rotation.y);
		monde->voitures[i].vect_rotation.x = -monde->voitures[i].vect_rotation.x;
		monde->voitures[i].vitesse = 0.;
		monde->voitures[i].sprite = ptr_plan_prev->sprites_au_dessus->sprites+index_sprites;
		index_sprites++;
		monde->voitures[i].sprite->texture = contexte->tableau_textures[niveau->texture_ids_voit[i]];
		monde->voitures[i].sprite->echelle = niveau->echelles_voit[i];
		monde->voitures[i].sprite->position = niveau->positions_initiales_voit[i];
		// TODO source rectangle (animation voiture à gauche voiture à droite)
		monde->voitures[i].sprite->source.x = monde->voitures[i].sprite->source.y = 0;
		SDL_QueryTexture(monde->voitures[i].sprite->texture,NULL,NULL,&monde->voitures[i].sprite->source.w,&monde->voitures[i].sprite->source.h);

		// on place la voitures dans les cases de la grille
		for (unsigned int col=(monde->voitures[i].position.x+monde->voitures[i].min_x)*monde->nb_colonnes/monde->l;
		col<(monde->voitures[i].position.x+monde->voitures[i].max_x)*monde->nb_colonnes/monde->l + 1; ++col) {
			for (unsigned int lig=(monde->voitures[i].position.y+monde->voitures[i].min_y)*monde->nb_lignes/monde->h;
			lig<(monde->voitures[i].position.y+monde->voitures[i].max_y)*monde->nb_lignes/monde->h + 1; ++lig) {
				// ajout de la voiture dans cette case
				monde->grille_voitures[(lig*monde->nb_colonnes + col)*monde->nb_voitures + monde->grille[lig*monde->nb_colonnes + col].nb_voitures] = i;
				monde->grille[lig*monde->nb_colonnes + col].nb_voitures++;
			}
		}
	}

	// initialisation camera
	monde->cam.renderer = contexte->param_cam.renderer;
	monde->cam.tmp_text = contexte->param_cam.tmp_text;
	monde->cam.tmp_cible = contexte->param_cam.tmp_cible;
	monde->cam.dimension_cible = contexte->param_cam.dimension_cible;
	monde->cam.cible = contexte->param_cam.cible;
	monde->cam.N_MAX = contexte->param_cam.N_MAX;
	monde->cam.tableau_z = calloc(contexte->param_cam.N_MAX,sizeof(Z_SPRITE));
	monde->cam.tableau_p = calloc(contexte->param_cam.N_MAX,sizeof(SPRITE_PROJETE));
	monde->cam.roulis = 0.;
	monde->cam.latitude = CAM_LAT;
	monde->cam.longitude = niveau->angles_initiaux_voit[0];
	monde->cam.position.x = monde->voitures[0].position.x-DIST_CAM_VOITURE*monde->voitures[0].vect_rotation.x;
	monde->cam.position.y = monde->voitures[0].position.y-DIST_CAM_VOITURE*monde->voitures[0].vect_rotation.y;
	monde->cam.position.z = HAUTEUR_CAMERA;
	monde->cam.offset_horizontal = monde->cam.offset_vertical = 0.;
	monde->cam.distance_ecran = DIST_CAM_ECRAN;
	monde->cam.echelle_ecran = tanf(SEMI_FOV)*DIST_CAM_ECRAN*2./contexte->param_cam.dimension_cible.w;

	// initialisation police + audio
	monde->police = contexte->police;
	monde->nb_channels = contexte->nb_channels;
	monde->tableau_sons = contexte->tableau_sons;

	// initialisation timer
	monde->timer = 0;
	monde->timer_position = contexte->timer_position;
	monde->timer_color = contexte->timer_color;

	// initialisation minimap
	monde->minimap = contexte->tableau_textures[niveau->minimap];
	monde->minimap_rect = contexte->minimap_rect;

	// initialisation checkpoint
	monde->nb_checkpoints = niveau->nb_checkpoints;
	monde->tableau_checkpoints = niveau->tableau_checkpoints;

}

void Decharger_Monde_Physique(MONDE_PHYSIQUE* monde) {

	// free absolument toute la structure
	for (unsigned short int i=0; i<monde->nb_voitures; ++i) { free(monde->voitures[i].tableau_centres); }
	free(monde->voitures);
	free(monde->grille);
	free(monde->grille_voitures);
	free(monde->grille_pos_segments);
	free(monde->grille_segments);
	free(monde->cam.tableau_z);
	free(monde->cam.tableau_p);
	TABLEAU_SPRITES* ptr = monde->scene.sprites_tout_en_bas;
	TABLEAU_SPRITES* tmp;
	while (ptr != NULL) { tmp = ptr->suivant; free(ptr->sprites); free(ptr); ptr = tmp; }
	PLAN_HORIZONTAL* ptr2 = monde->scene.tout_en_bas;
	PLAN_HORIZONTAL* tmp2;
	while (ptr2 != NULL) {
		ptr = ptr2->sprites_au_dessus;
		while (ptr != NULL) { tmp = ptr->suivant; free(ptr->sprites); free(ptr); ptr = tmp; }
		tmp2 = ptr2->au_dessus;
		free(ptr2);
		ptr2 = tmp2;
	}

}

long long int Calculer_Monde_Physique(MONDE_PHYSIQUE* monde, const short int* INPUT, const unsigned long long dt) {

	// gestion checkpoints

	// gestion timer
	monde->timer += dt;

	float speed_coef = INPUT[UP] ? 2.0 : 0.0;
	VECTEUR2D deplacement_qd, deplacement_zs;
	if (INPUT[LEFT])
		monde->voitures[0].angle += 0.05;
	if (INPUT[RIGHT])
		monde->voitures[0].angle -= 0.05;
	monde->voitures[0].angle = monde->voitures[0].angle < 0 ? monde->voitures[0].angle + 2*M_PI : monde->voitures[0].angle;
	monde->voitures[0].angle = monde->voitures[0].angle >= 2*M_PI ? monde->voitures[0].angle - 2*M_PI : monde->voitures[0].angle;
	sincosf(monde->voitures[0].angle,&monde->voitures[0].vect_rotation.x,&monde->voitures[0].vect_rotation.y);
	monde->voitures[0].vect_rotation.x = -monde->voitures[0].vect_rotation.x;
	monde->voitures[0].deplacement_final.x = speed_coef*monde->voitures[0].vect_rotation.x;
	monde->voitures[0].deplacement_final.y = speed_coef*monde->voitures[0].vect_rotation.y;
	// parcours des voitures et calcul du mouvement final en fct du mouvement désiré (juste en dessous) ET DES COLLISIONS
	for (unsigned short int i=0; i<monde->nb_voitures; ++i) {

		// déterminer le mouvement désiré (en fct des inputs/de l'ia/de dt)
		// monde->voitures[i].deplacement_final = ...
		if (i > 0)  { // voiture normale
			monde->voitures[i].deplacement_final.x = monde->voitures[i].deplacement_final.y = 0.;
		}

		// parcours des cases de la grille dans lesquelles la voiture se trouve
		for (unsigned int col=(monde->voitures[i].position.x+monde->voitures[i].min_x)*monde->nb_colonnes/monde->l;
		col<(monde->voitures[i].position.x+monde->voitures[i].max_x)*monde->nb_colonnes/monde->l + 1; ++col) {
			for (unsigned int lig=(monde->voitures[i].position.y+monde->voitures[i].min_y)*monde->nb_lignes/monde->h;
			lig<(monde->voitures[i].position.y+monde->voitures[i].max_y)*monde->nb_lignes/monde->h + 1; ++lig) {
				// parcours des voitures dans cette case
				for (unsigned short int j=0; j<monde->grille[lig*monde->nb_colonnes + col].nb_voitures; ++j) {
					float pdt_scalaire;
					VECTEUR2D direction_collision;
					unsigned short int k;
					k = monde->grille_voitures[(lig*monde->nb_colonnes + col)*monde->nb_voitures + j];
					// si collision
					if (k != i && Test_Collision_Voitures(monde->voitures+i,monde->voitures+k,&direction_collision)) {
						// modification du mouvement final en conséquence
						pdt_scalaire = PDT_SCALAIRE_2D_M(monde->voitures[i].deplacement_final,direction_collision);
						pdt_scalaire = pdt_scalaire < 0 ? 0 : pdt_scalaire;
						monde->voitures[i].deplacement_final.x -= pdt_scalaire*direction_collision.x;
						monde->voitures[i].deplacement_final.y -= pdt_scalaire*direction_collision.y;
					}
				}
				// parcours des segments dans cette case
				for (unsigned int j=monde->grille[lig*monde->nb_colonnes + col].indice_debut_segments; j<monde->grille[lig*monde->nb_colonnes + col + 1].indice_debut_segments; ++j) {
					float pdt_scalaire;
					VECTEUR2D direction_collision;
					// si collision
					if (Test_Collision_Voiture_Segment(monde->voitures+i, monde->grille_segments+j, monde->grille_pos_segments+j, &direction_collision)) {
						// modification du mouvement final en conséquence
						pdt_scalaire = PDT_SCALAIRE_2D_M(monde->voitures[i].deplacement_final,direction_collision);
						pdt_scalaire = pdt_scalaire < 0 ? 0 : pdt_scalaire;
						monde->voitures[i].deplacement_final.x -= pdt_scalaire*direction_collision.x;
						monde->voitures[i].deplacement_final.y -= pdt_scalaire*direction_collision.y;
					}
				}
			}
		}
	}

	// deuxième parcours des voitures pour appliquer le mouvement final et calculer les nouvelles cases de la grille ET effectuer les changements graphiques
	for (unsigned short int i=0; i<monde->nb_voitures; ++i) {

		// parcours des cases de la grille dans lesquelles la voiture se trouve
		for (unsigned int col=(monde->voitures[i].position.x+monde->voitures[i].min_x)*monde->nb_colonnes/monde->l;
		col<(monde->voitures[i].position.x+monde->voitures[i].max_x)*monde->nb_colonnes/monde->l + 1; ++col) {
			for (unsigned int lig=(monde->voitures[i].position.y+monde->voitures[i].min_y)*monde->nb_lignes/monde->h;
			lig<(monde->voitures[i].position.y+monde->voitures[i].max_y)*monde->nb_lignes/monde->h + 1; ++lig) {
				// efface la voitures de cette case
				monde->grille[lig*monde->nb_colonnes + col].nb_voitures--;
				unsigned short int k;
				for (k=0; monde->grille_voitures[(lig*monde->nb_colonnes + col)*monde->nb_voitures + k] != i; ++k);
				if (k != monde->grille[lig*monde->nb_colonnes + col].nb_voitures) {
					monde->grille_voitures[(lig*monde->nb_colonnes + col)*monde->nb_voitures + k] = monde->grille_voitures[(lig*monde->nb_colonnes + col)*monde->nb_voitures + monde->grille[lig*monde->nb_colonnes + col].nb_voitures];
				}

			}
		}
		// modification de la position
		monde->voitures[i].position.x += monde->voitures[i].deplacement_final.x;
		monde->voitures[i].position.y += monde->voitures[i].deplacement_final.y;
		// parcours des NOUVELLES CASES de la grille dans lesquelles la voiture se trouve
		for (unsigned int col=(monde->voitures[i].position.x+monde->voitures[i].min_x)*monde->nb_colonnes/monde->l;
		col<(monde->voitures[i].position.x+monde->voitures[i].max_x)*monde->nb_colonnes/monde->l + 1; ++col) {
			for (unsigned int lig=(monde->voitures[i].position.y+monde->voitures[i].min_y)*monde->nb_lignes/monde->h;
			lig<(monde->voitures[i].position.y+monde->voitures[i].max_y)*monde->nb_lignes/monde->h + 1; ++lig) {
				// ajout de la voiture dans cette case
				monde->grille_voitures[(lig*monde->nb_colonnes + col)*monde->nb_voitures + monde->grille[lig*monde->nb_colonnes + col].nb_voitures] = i;
				monde->grille[lig*monde->nb_colonnes + col].nb_voitures++;
			}
		}
		// modifications graphiques
		monde->voitures[i].sprite->position.x = monde->voitures[i].position.x;
		monde->voitures[i].sprite->position.y = monde->voitures[i].position.y;
	}


	monde->cam.position.x = monde->voitures[0].position.x-DIST_CAM_VOITURE*monde->voitures[0].vect_rotation.x;
	monde->cam.position.y = monde->voitures[0].position.y-DIST_CAM_VOITURE*monde->voitures[0].vect_rotation.y;
	monde->cam.longitude = monde->voitures[0].angle;
	// pour debugguer
#ifdef FPS_CONTROLS
	float speed_coef = 5.;
	VECTEUR2D deplacement_qd, deplacement_zs;
	if (INPUT[UP]) monde->cam.latitude += 0.05;
    if (INPUT[DOWN]) monde->cam.latitude -= 0.05;
    if (INPUT[LEFT]) monde->cam.longitude += 0.05;
    if (INPUT[RIGHT]) monde->cam.longitude -= 0.05;
    monde->cam.latitude = monde->cam.latitude > M_PI / 2 ? M_PI / 2 : monde->cam.latitude;
    monde->cam.latitude = monde->cam.latitude < -M_PI / 2 ? -M_PI / 2 : monde->cam.latitude;
	sincosf(monde->cam.longitude,&deplacement_qd.y,&deplacement_qd.x);
    deplacement_qd.x *= speed_coef;
    deplacement_qd.y *= speed_coef;
    deplacement_zs.x = -deplacement_qd.y;
    deplacement_zs.y = deplacement_qd.x;
    if (INPUT[Z]) {monde->cam.position.x += deplacement_zs.x; monde->cam.position.y += deplacement_zs.y;}
    if (INPUT[S]) {monde->cam.position.x -= deplacement_zs.x; monde->cam.position.y -= deplacement_zs.y;}
    if (INPUT[D]) {monde->cam.position.x += deplacement_qd.x; monde->cam.position.y += deplacement_qd.y;}
    if (INPUT[Q]) {monde->cam.position.x -= deplacement_qd.x; monde->cam.position.y -= deplacement_qd.y;}
    if (INPUT[E]) monde->cam.position.z += speed_coef;
    if (INPUT[A]) monde->cam.position.z -= speed_coef;
#endif

	// renvoie 0 si le jeu continue de tourner, renvoie 1 si la partie est terminée
	return 0;
}
