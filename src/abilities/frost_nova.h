#ifndef FROST_NOVA_H
#define FROST_NOVA_H

#include "util/game_world.h"
#include "util/game_object.h"
#include "ability.h"
#include "spell.h"

void frost_nova_use(GameWorld *world, GameObject *user);

Spell *frost_nova_new(GameWorld *world);
void frost_nova_collide(GameObject *object, GameObject *user);

#endif
