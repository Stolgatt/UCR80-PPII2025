#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "engine_common.h"
#include "graphical_engine.h"
#include "physical_engine.h"
// #include "interface_utilisateur.h"

#include "levels.h"

#define TX 1920
#define TY 1080

#define FPS 60.
#define DUREE_FRAME (1000./FPS)
#define MONTRER_FPS

#define FREQ_ECHANT 44100
#define FORMATS_AUDIO 0
#define T_AUDIO_BUFFER 256
#define NB_CHANNELS 2 // + 1 pour la musique
const float VOL_INIT_MUS = 1.;
const float VOL_INIT_CHAN[NB_CHANNELS] = {1.,0.5}; // entre 0 et 1

#define EST_DANS_CLICKZONE(chose,clickZone) ((chose).x >= (clickZone).x && (chose).y >= (clickZone).y && (chose).x <= (clickZone).w && (chose).y <= (clickZone).h)

inline void choix_page_menu(int n_colonnes, int index, SDL_Rect* source) {
    source->w = TX;
    source->h = TY;
    source->x = source->w*(index%n_colonnes);
    source->y = source->h*(index/n_colonnes);
}

