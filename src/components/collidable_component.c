#include <stdlib.h>
#include <GL/glew.h>
#include "collidable_component.h"
#include "util/line.h"

extern Vec3 VEC3_ZERO;

void collidable_component_update(CollidableComponent *self, double dt);
void collidable_component_render(CollidableComponent *self, Renderer *renderer);

CollidableComponent collidable_component_init(GameObject *container, Obb bounding_box, OnCollideCallback on_collide) {
    CollidableComponent self;

    self.base_component = component_init(container, collidable_component_update, NULL);
    self.bounding_box = bounding_box;
    self.is_colliding = collidable_component_is_colliding;
    self.on_collide = on_collide;

    return self;
}

int collidable_component_is_colliding(CollidableComponent self, CollidableComponent other){
    return obb_intersects(self.bounding_box, other.bounding_box);
}

void collidable_component_update(CollidableComponent *self, double dt){
    // here we are assuming position/direction has been moved to GameObject
    GameObject *container = self->base_component.container;

    float rotation = game_object_get_y_rotation(container);
    self->bounding_box.center = container->position;
    self->bounding_box.center.y += self->bounding_box.radius.y;
    obb_rotate_y(&self->bounding_box, rotation);
}

void collidable_component_render(CollidableComponent *self, Renderer *renderer){
    int i, ti;
    Vec3 r = VEC3_ZERO;
    Vec3 rs[3];
    for (i = 0; i < 3; i++) rs[i] = vec3_scale(self->bounding_box.axis[i], 2 * self->bounding_box.radius.data[i]);
    for(i = 0;i < 3;i++) r = vec3_add(r, rs[i]);
    r = vec3_scale(r, 0.5);
    Vec3 max = vec3_add(self->bounding_box.center, r);
    Vec3 min = vec3_sub(self->bounding_box.center, r);

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
