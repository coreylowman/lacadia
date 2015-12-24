#ifndef ABILITY_H
#define ABILITY_H

#include "util/game_world.h"
#include "util/game_object.h"

typedef struct {
    float max_cooldown;
    float cooldown;
    void (*on_use)(GameWorld *world, GameObject *user);
} Ability;

Ability *ability_new();
void ability_free(Ability *self);

#endif
