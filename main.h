#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "engine_common.h"
#include "graphical_engine.h"
#include "physical_engine.h"
#include "score.h"

#include "levels.h"

// graphisme
#define TX 1920
#define TY 1080
const SDL_Color TIMER_COLOR = {255, 255, 255, 255};
const SDL_Rect MINIMAP_RECT = {TX-20-215,20,215,215};
const SDL_Color MENU_COLOR = {255, 255, 255, 255};

// fps
#define FPS 60.
#define DUREE_FRAME (1000./FPS)
#define MONTRER_FPS

// audio
#define FREQ_ECHANT 44100
#define FORMATS_AUDIO 0
#define T_AUDIO_BUFFER 256
#define NB_CHANNELS 2 // + 1 pour la musique
const float VOL_INIT_MUS = 1.;
const float VOL_INIT_CHAN[NB_CHANNELS] = {1.,0.5}; // entre 0 et 1
const short int MAX_VOLUME = 10;

// jeu
enum { ACCUEIL = 0, SELECTION, LEADERBOARD, OPTIONS, PAUSE, GAME_OVER, JEU };
enum { DEFAULT, MAP1, MAP2};

#define EST_DANS_CLICKZONE(chose,clickZone) ((chose).x >= (clickZone).x && (chose).y >= (clickZone).y && (chose).x <= (clickZone).w && (chose).y <= (clickZone).h)

inline void choix_page_menu(int n_colonnes, int index, SDL_Rect* source) {
    source->w = TX;
    source->h = TY;
    source->x = source->w*(index%n_colonnes);
    source->y = source->h*(index/n_colonnes);
}

char* NOM_MAPS[NUM_MAPS] = {"Piste Moo-Moo", "Neon Rush"};