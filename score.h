#ifndef SCORE_H
#define SCORE_H


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include "engine_common.h"
#include "graphical_engine.h"
#include "physical_engine.h"

#ifndef NUM_MAPS
#define NUM_MAPS 2
#endif


void read_best_times(int best_times[NUM_MAPS]);
void write_best_times(int best_times[NUM_MAPS]);
void update_best_time(int map_index, int current_time, int best_times[NUM_MAPS]);

void initiate_quad(SDL_Rect* quad, int x, int y, int w, int h);





#endif

