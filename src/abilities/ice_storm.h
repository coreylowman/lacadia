#ifndef ICE_STORM_H
#define ICE_STORM_H

#include "util/game_world.h"
#include "util/game_object.h"
#include "ability.h"
#include "spell.h"

void ice_storm_use(GameWorld *world, GameObject *user);

Spell *ice_storm_new(GameWorld *world);
void ice_storm_collide(GameObject *object, GameObject *user);

#endif
