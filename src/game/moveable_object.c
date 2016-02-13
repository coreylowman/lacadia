#include <math.h>
#include "moveable_object.h"

void moveable_object_update(MoveableObject *self, double dt){
    int i;
    for(i = 0;i < 3;i++){
        self->last_move.data[i] = self->direction.data[i] * self->speed * dt;
    }

    self->position = vec3_add(self->position, self->last_move);
}

void moveable_object_reverse(MoveableObject *self){
    self->position = vec3_sub(self->position, self->last_move);
}

float moveable_object_get_y_rotation(MoveableObject self){
    float rotation = atan(self.direction.x / self.direction.z);
    if (self.direction.z <= 0.0)
        rotation += 3.14159265358979323846;
    return rotation;
}

void moveable_object_move_by(MoveableObject *self, Vec3 dp){
    int i;
    self->last_move = dp;
    self->position = vec3_add(self->position, self->last_move);
}
