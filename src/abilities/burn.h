#ifndef BURN_H
#define BURN_H

#include "game/game_world.h"
#include "game/moveable_object.h"
#include "effect.h"
#include "game/particle_system.h"

typedef struct {
    int degree;
    float dps;
    ParticleSystem *particle_system;
} BurnData;

Effect *burn_new(GameWorld *world, MoveableObject *target, float dmg, float duration);
void burn_increase_degree(Effect *self);


#endif
