#ifndef FROST_H
#define FROST_H

#include "game/game_world.h"
#include "effect.h"
#include "game/particle_system.h"

typedef struct Frost {
    Effect base_effect;

    int degree;
    float slow_pct_per_degree;

    ParticleSystem *particle_system;
} Frost;

Frost *frost_new(GameWorld *world, GameObject *target, float slow_amt_per_degree, float duration);
void frost_increase_degree(Effect *self);

#endif
