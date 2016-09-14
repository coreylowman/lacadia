#ifndef GAME_LEVEL_H
#define GAME_LEVEL_H

#include "util/array_list.h"
#include "wall.h"

typedef struct {
  ArrayList *walls; // ArrayList<Wall *>
} GameLevel;

GameLevel *game_level_new();
void game_level_free(GameLevel *self);

#endif
