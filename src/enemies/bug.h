#ifndef BUG_H
#define BUG_H

#include "game/game_world.h"
#include "game/game_object.h"
#include "enemy.h"

Enemy *bug_new(GameWorld *world);

void bug_on_update(Enemy *self, double dt);
void bug_on_collide(Enemy *self, GameObject *object);

#endif
