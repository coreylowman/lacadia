#ifndef BUG_H
#define BUG_H

#include "engine/game_world.h"
#include "engine/game_object.h"
#include "enemy.h"
#include "engine/util/vec3.h"

Enemy *bug_new(GameWorld *world, Vec3 position);

#endif
