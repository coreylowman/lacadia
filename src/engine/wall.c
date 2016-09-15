#include <stdlib.h>
#include "wall.h"
#include "engine/util/mat4.h"
#include "engine/util/random.h"
#include "engine/util/renderer.h"

Wall *wall_new(GameWorld *world, Vec3 position, Vec3 grow_direction,
               int length) {
  Wall *self = malloc(sizeof(*self));
  self->base_object =
      game_object_init(world, "wall", NULL, wall_render, wall_free);

  Vec3 dims = wall_dimensions(world);
  int which = grow_direction.x ? 0 : (grow_direction.y ? 1 : 2);
  float width =
      grow_direction.x ? dims.x : (grow_direction.y ? dims.y : dims.z);

  game_object_alloc_components(&self->base_object, length + 1);

  int model_id = game_world_get_model_id(world, "assets/wall");
  int i;
  Vec3 pos;
  Mat4 model_matrix;
  RenderableComponent *renderable;
  for (i = 0; i < length; i++) {
    pos = vec3_add(position, vec3_scale(grow_direction, width * (float)i));
    mat4_ident(&model_matrix);
    mat4_rotate_y(&model_matrix,
                  3.14159265358979323846 * 0.5 * (float)random_in_rangei(0, 4));
    mat4_translate(&model_matrix, pos);

    renderable = renderable_component_new(&self->base_object, "assets/wall",
                                          world->renderer);
    renderable_component_set_model_matrix(renderable, model_matrix);
    self->base_object.components[i] = (Component *)renderable;
  }

  Obb bounding_box = game_world_get_model_obb(world, model_id);
  bounding_box.center = position;
  bounding_box.center.y += dims.y * 0.5;
  bounding_box.radius.data[which] *= length;
  bounding_box.center.data[which] += width * (length - 1) * 0.5;
  self->base_object.components[length - 1] =
      (Component *)collidable_component_new(&self->base_object, bounding_box,
                                            wall_on_collide);

  self->normal = which == 0 ? VEC3_UNIT_Z : VEC3_UNIT_X;

  return self;
}

void wall_free(GameObject *obj) {
  Wall *self = (Wall *)obj;
  free(self);
}

void wall_render(GameObject *obj, Renderer *renderer) {
  Wall *self = (Wall *)obj;
}

void wall_on_collide(GameObject *self, GameObject *other) {
  // do nothing
}

Vec3 wall_dimensions(GameWorld *world) {
  int model_id = game_world_get_model_id(world, "assets/wall");
  Obb obb = game_world_get_model_obb(world, model_id);
  return vec3_scale(obb.radius, 2.0);
}

Vec3 wall_get_normal(Wall *self, Vec3 position) {
  CollidableComponent *collidable =
      (CollidableComponent *)
          self->base_object.components[self->base_object.num_components - 1];

  int i;
  for (i = 0; i < 3; i++) {
    if (self->normal.data[i] == 1.0) {
      if (position.data[i] < collidable->bounding_box.center.data[i]) {
        return vec3_scale(self->normal, -1.0);
      } else {
        return self->normal;
      }
    }
  }
  return VEC3_ZERO;
}
