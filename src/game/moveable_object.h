#ifndef MOVEABLE_OBJECT_H
#define MOVEABLE_OBJECT_H

#include "util/vec3.h"

typedef struct {
    Vec3 position;
    Vec3 direction;
    float speed;

    Vec3 last_move;
} MoveableObject;

void moveable_object_update(MoveableObject *self, double dt);
void moveable_object_reverse(MoveableObject *self);
float moveable_object_get_y_rotation(MoveableObject self);
void moveable_object_move_by(MoveableObject *self, Vec3 dp);

#endif
