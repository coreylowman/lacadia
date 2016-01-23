#ifndef FIREBALL_H
#define FIREBALL_H

#include "game_world.h"
#include "ability.h"
#include "spell.h"
#include "util/vec3.h"

void fireball_use(GameWorld *world, GameObject *user);
Spell *fireball_new(GameWorld *world, GameObject *user);
void fireball_update(Spell *self, double dt);
void fireball_on_collide(GameObject *self, GameObject *other);

#endif
