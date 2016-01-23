#ifndef COLLIDABLE_OBJECT_H
#define COLLIDABLE_OBJECT_H

#include "util/obb.h"
#include "util/vec3.h"
#include "game_object.h"
#include "moveable_object.h"

typedef struct {
    Obb bounding_box;
    void (*on_collide)(GameObject *self, GameObject *other);
    GameObject *self;
} CollidableObject;

void collidable_object_update(CollidableObject *self, MoveableObject moveable);
void collidable_object_render(CollidableObject self);

#endif
