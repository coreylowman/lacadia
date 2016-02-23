#include <GL/glew.h>
#include "collidable_object.h"
#include "util/line.h"

extern Vec3 VEC3_ZERO;

int collidable_object_is_colliding(CollidableObject self, CollidableObject other){
    return obb_intersects(self.bounding_box, other.bounding_box);
}

void collidable_object_update(CollidableObject *self, MoveableObject moveable){
    float rotation = moveable_object_get_y_rotation(moveable);
    self->bounding_box.center = moveable.position;
    self->bounding_box.center.y += self->bounding_box.radius.y;
    obb_rotate_y(&self->bounding_box, rotation);
}

void collidable_object_render(CollidableObject self, Renderer *renderer){
    int i, ti;
    Vec3 r = VEC3_ZERO;
    Vec3 rs[3];
	for (i = 0; i < 3; i++) rs[i] = vec3_scale(self.bounding_box.axis[i], 2 * self.bounding_box.radius.data[i]);
    for(i = 0;i < 3;i++) r = vec3_add(r, rs[i]);
    r = vec3_scale(r, 0.5);
	Vec3 max = vec3_add(self.bounding_box.center, r);
	Vec3 min = vec3_sub(self.bounding_box.center, r);

    Line l;

    for (i = 0; i < 3; i++){
        ti = (i + 1) % 3;

        l.start = min;
        l.end = vec3_add(min, rs[i]);
        renderer_render_line(renderer, l);

        l.start = max;
        l.end = vec3_sub(max, rs[i]);
        renderer_render_line(renderer, l);

        l.start = vec3_add(min, rs[i]);
        l.end = vec3_sub(max, rs[ti]);
        renderer_render_line(renderer, l);

        l.start = vec3_add(min, rs[ti]);
        l.end = vec3_sub(max, rs[i]);
        renderer_render_line(renderer, l);
    }
}
