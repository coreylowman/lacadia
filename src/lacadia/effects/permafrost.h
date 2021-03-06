#ifndef PERMAFROST_H
#define PERMAFROST_H

#include "engine/game_world.h"
#include "effect.h"
#include "engine/particle_system.h"

typedef struct Permafrost {
  Effect base_effect;

  float damage;
  float slow_pct;

  ParticleSystem *particle_system;
} Permafrost;

Permafrost *permafrost_new(GameWorld *world, GameObject *target, float dmg,
                           float slow_pct, float duration);

#endif
