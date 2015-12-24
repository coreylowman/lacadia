#ifndef DRAGONS_BREATH_H
#define DRAGONS_BREATH_H

#include "util/game_world.h"
#include "util/game_object.h"
#include "ability.h"
#include "spell.h"

void dragons_breath_use(GameWorld *world, GameObject *user);

Spell *dragons_breath_new(GameWorld *world);
void dragons_breath_collide(GameObject *object, GameObject *user);

#endif
