#ifndef ABILITY_H
#define ABILITY_H

#include "game/game_world.h"
#include "game/game_object.h"

//keeps track of the cooldown and what spell to cast once it is used
//each spell also has a corresponding ability
//the ability's on_use will add the corresponding spell into the game world
typedef struct Ability {
    //cooldown gets reset to max_cooldown on use
    float cooldown;
    float max_cooldown;

    void (*on_use)(GameWorld *world, GameObject *user);
} Ability;

Ability *ability_new();
void ability_free(Ability *self);

void ability_update(Ability *self, double dt);
void ability_use(Ability *self, GameWorld *world, GameObject *user);
int ability_is_ready(Ability self);

#endif
