#ifndef COIN_H
#define COIN_H

#include "util/vec3.h"
#include "collectable.h"

Collectable *coin_new(GameWorld *world, Vec3 position);

#endif
