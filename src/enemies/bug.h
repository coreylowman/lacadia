#ifndef BUG_H
#define BUG_H

#include "util/game_world.h"
#include "util/game_object.h"
#include "abilities/ability.h"
#include "util/stats.h"
#include "enemy.h"

Enemy *bug_new(GameWorld *world);
void bug_free(Enemy *self);

void bug_update(Enemy *self, double dt);
void bug_on_collide(Enemy *self, GameObject *object);

#endif
