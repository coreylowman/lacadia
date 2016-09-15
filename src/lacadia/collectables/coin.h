#ifndef COIN_H
#define COIN_H

#include "engine/util/vec3.h"
#include "collectable.h"

Collectable *coin_new(GameWorld *world, Vec3 position);

#endif
