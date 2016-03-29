#ifndef BURN_H
#define BURN_H

#include "game/game_world.h"
#include "game/moveable_object.h"
#include "effect.h"
#include "game/particle_system.h"

typedef struct {
    int degree;    
    float slow_amt;
    float amt_slowed;
    ParticleSystem *particle_system;
} FrostData;

Effect *frost_new(GameWorld *world, MoveableObject *target, float slow_amt, float duration);
void frost_increase_degree(Effect *self);

#endif
