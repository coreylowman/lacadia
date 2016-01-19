#ifndef COLLIDABLE_OBJECT_H
#define COLLIDABLE_OBJECT_H

#include "util/obb.h"
#include "game/game_object.h"

typedef struct {
    Obb bounding_box;
    void (*on_collide)(GameObject *self, GameObject *other);
} CollidableObject;

#endif
