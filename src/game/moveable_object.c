#include "moveable_object.h"

void moveable_object_update(MoveableObject *self, double dt){
    int i;
    for(i = 0;i < 3;i++){
        self->position.data[i] += self->direction.data[i] * self->speed * dt;
    }
}
