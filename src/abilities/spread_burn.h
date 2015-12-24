#ifndef SPREAD_BURN_H
#define SPREAD_BURN_H

#include "util/game_world.h"
#include "util/game_object.h"
#include "ability.h"
#include "spell.h"

void spread_burn_use(GameWorld *world, GameObject *user);

Spell *spread_burn_new(GameWorld *world);
void spread_burn_collide(GameObject *object, GameObject *user);

#endif
