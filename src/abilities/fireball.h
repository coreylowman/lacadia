#ifndef FIREBALL_H
#define FIREBALL_H

#include "util/game_world.h"
#include "util/game_object.h"
#include "ability.h"
#include "spell.h"

void fireball_use(GameWorld *world, GameObject *user);

Spell *fireball_new(GameWorld *world);
void fireball_collide(Spell *self, GameObject *object, GameObject *user);
void fireball_update(Spell *self, double dt);

#endif
