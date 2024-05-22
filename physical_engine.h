#include <stdio.h>
#include <stdlib.h>
#include "engine_common.h"

SEGMENT2D CREA_SEGMENT_2D(int x1, int x2, int y1, int y2);
VECTEUR2D Calcul_reflexion(VECTEUR2D direction_voulue, SEGMENT2D* segment);
VECTEUR2D Calcul_collisions(VECTEUR2D direction_voulue,const float* rayon, const VECTEUR2D* position_disque);
