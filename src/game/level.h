#ifndef LEVEL_H
#define LEVEL_H

#include "game_world.h"
#include "wall.h"

#define MAX_NUM_WALLS 64

typedef struct Level {
    int num_walls;
    Wall *walls[MAX_NUM_WALLS];
} Level;

Level *level_new(GameWorld *world);
void level_free(Level *self);

void level_render(Level *self);

#endif
