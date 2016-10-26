#ifndef CLOUD_H
#define CLOUD_H

#include "engine/util/vec3.h"
#include "engine/game_world.h"
#include "engine/game_object.h"

GameObject *cloud_new(GameWorld *world, Vec3 position);

#endif
