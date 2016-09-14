#ifndef BUG_SPAWNER_H
#define BUG_SPAWNER_H

#include "util/vec3.h"
#include "game/game_object.h"
#include "game/game_world.h"

typedef struct BugSpawner {
  GameObject base_object;

  double time_till_next;
  double time_to_spawn;
} BugSpawner;

GameObject *bug_spawner_new(GameWorld *world, Vec3 position,
                            double time_to_spawn);

#endif
