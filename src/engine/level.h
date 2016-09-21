#ifndef LEVEL_H
#define LEVEL_H

#include "game_world.h"
#include "wall.h"
#include "engine/util/terrain.h"
#include "engine/util/renderer.h"

#define MAX_NUM_WALLS 64

typedef struct Level {
  GameWorld *world;

  Terrain terrain;
} Level;

void level_terrain_callback(TerrainVertex *vert);

Level *level_new(GameWorld *world);
void level_free(Level *self);

void level_render(Level *self, Renderer *renderer);

#endif
