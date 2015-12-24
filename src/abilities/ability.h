#ifndef ABILITY_H
#define ABILITY_H

#include "util/game_world.h"
#include "util/game_object.h"

//keeps track of the cooldown and what spell to cast once it is used
//each spell also has a corresponding ability
//the ability's on_use will add the corresponding spell into the game world
typedef struct {
    //cooldown gets reset to max_cooldown on use
    float cooldown;
    float max_cooldown;

    void (*on_use)(GameWorld *world, GameObject *user);
} Ability;

Ability *ability_new();
void ability_free(Ability *self);

#endif
