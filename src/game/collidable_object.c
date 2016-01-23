#include "collidable_object.h"

void collidable_object_update(CollidableObject *self, MoveableObject moveable){
    float rotation = moveable_object_get_y_rotation(moveable);
    self->bounding_box.center = moveable.position;
    self->bounding_box.center.y += self->bounding_box.radius.y;
    obb_rotate_y(&self->bounding_box, rotation);
}

void collidable_object_render(CollidableObject self){
    obb_render(self.bounding_box);
}
