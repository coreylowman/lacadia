#ifndef BURN_H
#define BURN_H

#include "game/game_world.h"
#include "effect.h"
#include "game/particle_system.h"

typedef struct {
    int degree;
    float slow_amt_per_degree;
    float amt_slowed;
    ParticleSystem *particle_system;
} FrostData;

Effect *frost_new(GameWorld *world, GameObject *target, float slow_amt_per_degree, float duration);
void frost_increase_degree(Effect *self);

#endif
