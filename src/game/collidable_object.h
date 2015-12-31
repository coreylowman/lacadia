#ifndef COLLIDABLE_OBJECT_H
#define COLLIDABLE_OBJECT_H

#include "util/rect.h"
#include "game/game_object.h"

typedef struct {
    Rect bounding_box;
    void (*on_collide)(GameObject *self, GameObject *other);
} CollidableObject;

#endif
