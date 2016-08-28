#ifndef BURN_H
#define BURN_H

#include "game/game_world.h"
#include "effect.h"
#include "game/particle_system.h"

typedef struct Burn {
    Effect base_effect;

    int degree;
    float dps;

    ParticleSystem *particle_system;
} Burn;

Burn *burn_new(GameWorld *world, GameObject *target, float dmg, float duration);
void burn_increase_degree(Effect *self);

#endif
