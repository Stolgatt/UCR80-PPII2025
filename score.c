#include "score.h"

void read_best_times(long long int best_times[NUM_MAPS]) {
    FILE *file = fopen("best_times.txt", "r");
    if (file == NULL) {
        return;
    }
    for (int i = 0; i < NUM_MAPS; i++) {
        if (fscanf(file, "%lld", &best_times[i]) != 1) {
            printf("erreur lecture, entre 9000\n");
            best_times[i] = 9000;
        }
    }
    fclose(file);
}

void write_best_times(long long int best_times[NUM_MAPS]) {
    FILE *file = fopen("best_times.txt", "w");
    if (file == NULL) {
        perror("Could not open best_times.txt for writing");
        return;
    }
    for (int i = 0; i < NUM_MAPS; i++) {
        fprintf(file, "%lld\n", best_times[i]);
    }
    fclose(file);
}

void update_best_time(int map_index, long long int current_time, long long int best_times[NUM_MAPS]) {
    if (current_time < best_times[map_index-1]) {
        best_times[map_index-1] = current_time;
    }
}


void initiate_quad(SDL_Rect* quad, int x, int y, int w, int h){
    quad->x = x;
    quad->y = y;
    quad->h = h;
    quad->w = w;
}