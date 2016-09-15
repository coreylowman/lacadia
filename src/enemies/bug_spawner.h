#ifndef BUG_SPAWNER_H
#define BUG_SPAWNER_H

#include "engine/util/vec3.h"
#include "engine/game_object.h"
#include "engine/game_world.h"

typedef struct BugSpawner {
  GameObject base_object;

  double time_till_next;
  double time_to_spawn;
} BugSpawner;

GameObject *bug_spawner_new(GameWorld *world, Vec3 position,
                            double time_to_spawn);

#endif
