#include <stdlib.h>
#include <GL/glew.h>
#include "collidable_component.h"
#include "engine/util/line.h"

static void on_update(Component *self, double dt);
static void on_render(Component *self, Renderer *renderer);

CollidableComponent *collidable_component_new(GameObject *container,
                                              Obb bounding_box,
                                              OnCollideCallback on_collide) {
  CollidableComponent *self = malloc(sizeof(*self));

  *self = collidable_component_init(container, bounding_box, on_collide);

  self->base_component.allocated = 1;

  return self;
}

CollidableComponent collidable_component_init(GameObject *container,
                                              Obb bounding_box,
                                              OnCollideCallback on_collide) {
  CollidableComponent self;

  self.base_component = component_init(container, on_update, NULL, NULL);
  self.bounding_box = bounding_box;
  self.is_colliding = collidable_component_is_colliding;
  self.on_collide = on_collide;

  return self;
}

int collidable_component_is_colliding(CollidableComponent self,
                                      CollidableComponent other) {
  return obb_intersects(self.bounding_box, other.bounding_box);
}

void collidable_component_set_scale(CollidableComponent *self, float scale) {
  self->bounding_box.radius.x *= scale;
  self->bounding_box.radius.y *= scale;
  self->bounding_box.radius.z *= scale;
}

static void on_update(Component *component, double dt) {
  CollidableComponent *self = (CollidableComponent *)component;

  GameObject *container = self->base_component.container;

  float rotation = game_object_get_y_rotation(container);
  self->bounding_box.center = container->position;
  self->bounding_box.center.y += self->bounding_box.radius.y;
  obb_rotate_y(&self->bounding_box, rotation);
}

static void on_render(Component *component, Renderer *renderer) {
  CollidableComponent *self = (CollidableComponent *)component;
  int i, ti;
  Vec3 r = VEC3_ZERO;
  Vec3 rs[3];
  for (i = 0; i < 3; i++)
    rs[i] = vec3_scale(self->bounding_box.axis[i],
                       2 * self->bounding_box.radius.data[i]);
  for (i = 0; i < 3; i++)
    r = vec3_add(r, rs[i]);
  r = vec3_scale(r, 0.5);
  Vec3 max = vec3_add(self->bounding_box.center, r);
  Vec3 min = vec3_sub(self->bounding_box.center, r);

  for (i = 0; i < 3; i++) {
    ti = (i + 1) % 3;

    renderer_render_line(renderer, min, vec3_add(min, rs[i]));
    renderer_render_line(renderer, max, vec3_sub(max, rs[i]));
    renderer_render_line(renderer, vec3_add(min, rs[i]), vec3_sub(max, rs[ti]));
    renderer_render_line(renderer, vec3_add(min, rs[ti]), vec3_sub(max, rs[i]));
  }
}
