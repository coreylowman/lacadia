#ifndef SOULBURN_H
#define SOULBURN_H

#include "engine/game_world.h"
#include "effect.h"
#include "engine/particle_system.h"

typedef struct Soulburn {
  Effect base_effect;

  float dps;
  float leech_pct;
  GameObject *user;

  ParticleSystem *particle_system;
} Soulburn;

Soulburn *soulburn_new(GameWorld *world, GameObject *target, GameObject *user,
                       float dps, float leech_pct, float duration);

#endif
