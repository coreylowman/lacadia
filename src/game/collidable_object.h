#ifndef COLLIDABLE_OBJECT_H
#define COLLIDABLE_OBJECT_H

#include "util/obb.h"
#include "util/vec3.h"
#include "game_object.h"
#include "moveable_object.h"

typedef struct CollidableObject {
    Obb bounding_box;

    int (*is_colliding)(struct CollidableObject self, struct CollidableObject other);
    void (*on_collide)(GameObject *self, GameObject *other);
    
    GameObject *container;
} CollidableObject;

int collidable_object_is_colliding(CollidableObject self, CollidableObject other);
void collidable_object_update(CollidableObject *self, MoveableObject moveable);
void collidable_object_render(CollidableObject self);

#endif
