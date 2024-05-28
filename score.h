#ifndef SCORE_H
#define SCORE_H

#include <stdio.h>
#include <SDL2/SDL.h>

#define NUM_MAPS 2

void read_best_times(long long int best_times[NUM_MAPS]);
void write_best_times(long long int best_times[NUM_MAPS]);
void update_best_time(int map_index, long long int current_time, long long int best_times[NUM_MAPS]);

void initiate_quad(SDL_Rect* quad, int x, int y, int w, int h);

#endif

