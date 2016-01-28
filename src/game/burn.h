#ifndef BURN_H
#define BURN_H

#include "game_world.h"
#include "moveable_object.h"
#include "effect.h"
#include "particle_system.h"

typedef struct {
    int degree;
    float dps;
    ParticleSystem *particle_system;
} BurnData;

Effect *burn_new(GameWorld *world, MoveableObject *target, float dmg, float duration);

#endif
