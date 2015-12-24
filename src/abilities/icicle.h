#ifndef ICICLE_H
#define ICICLE_H

#include "util/game_world.h"
#include "util/game_object.h"
#include "ability.h"
#include "spell.h"

void icicle_use(GameWorld *world, GameObject *user);

Spell *icicle_new(GameWorld *world);
void icicle_collide(GameObject *object, GameObject *user);

#endif
