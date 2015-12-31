#ifndef MOVEABLE_OBJECT_H
#define MOVEABLE_OBJECT_H

#include "util/vec3.h"

typedef struct {
    Vec3 position;
    Vec3 direction;
    float speed;
} MoveableObject;

void moveable_object_update(MoveableObject *self, double dt);

#endif
