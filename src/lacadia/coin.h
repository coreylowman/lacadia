#ifndef COIN_H
#define COIN_H

#include "engine/util/vec3.h"
#include "engine/game_world.h"
#include "engine/game_object.h"

GameObject *coin_new(GameWorld *world, Vec3 position);

#endif
