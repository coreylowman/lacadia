#ifndef BUG_H
#define BUG_H

#include "game/game_world.h"
#include "game/game_object.h"
#include "enemy.h"
#include "util/vec3.h"

Enemy *bug_new(GameWorld *world, Vec3 position);

void bug_on_update(Enemy *self, double dt);
void bug_on_collide(GameObject *self, GameObject *object);

#endif
