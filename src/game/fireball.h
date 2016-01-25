#ifndef FIREBALL_H
#define FIREBALL_H

#include "game_world.h"
#include "ability.h"
#include "spell.h"
#include "util/vec3.h"
#include "affectable_object.h"
#include "effect.h"

void fireball_use(GameWorld *world, GameObject *user);
Spell *fireball_new(GameWorld *world, GameObject *user);
void fireball_update(Spell *self, double dt);
void fireball_on_collide(GameObject *self, GameObject *other);

Effect *burn_new(GameWorld *world, MoveableObject *target, float dmg, float duration);
void burn_on_apply(Effect *self, AffectableObject *affectable);
void burn_on_update(Effect *self, AffectableObject *affectable, double dt);
void burn_on_render(Effect *self, GameWorld *world);
void burn_on_end(Effect *self, AffectableObject *affectable);
void burn_on_free(Effect *self);
int burn_is_over(Effect *self);

#endif
