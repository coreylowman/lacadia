#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "game_object.h"
#include "component.h"

GameObject game_object_init(GameWorld *world, const char *tag,
                            GameObjectUpdateCallback on_update,
                            GameObjectRenderCallback on_render,
                            GameObjectFreeCallback on_free) {
  GameObject self;

  strcpy(self.tag, tag);

  self.world = world;

  self.position = VEC3_ZERO;
  self.direction = VEC3_UNIT_X;

  self.num_components = 0;
  self.components = NULL;

  self.on_update = on_update;
  self.on_render = on_render;
  self.on_free = on_free;

  self.destroy = 0;

  return self;
}

void game_object_alloc_components(GameObject *self, int num_components) {
  self->num_components = num_components;
  self->components = malloc(num_components * sizeof(*self->components));
}

float game_object_get_y_rotation(GameObject *self) {
  float rotation = atan(self->direction.x / self->direction.z);
  if (self->direction.z <= 0.0)
    rotation += 3.14159265358979323846;
  return rotation;
}

void game_object_rotate_by(GameObject *self, float radians) {
  Mat4 rotation_matrix = MAT4_IDENT;
  mat4_rotate_y(&rotation_matrix, radians);
  mat4_mul_vec3(&self->direction, rotation_matrix, self->direction);
}

void game_object_move_by(GameObject *self, Vec3 velocity) {
  self->position = vec3_add(self->position, velocity);
}

void game_object_move(GameObject *self, double scalar) {
  self->position =
      vec3_add(self->position, vec3_scale(self->direction, scalar));
}

void game_object_update(GameObject *self, double dt) {
  int i;
  for (i = 0; i < self->num_components; i++) {
    component_update(self->components[i], dt);
  }

  if (self->on_update != NULL) {
    self->on_update(self, dt);
  }
}

void game_object_render(GameObject *self, Renderer *renderer) {
  int i;
  for (i = 0; i < self->num_components; i++) {
    component_render(self->components[i], renderer);
  }

  if (self->on_render != NULL) {
    self->on_render(self, renderer);
  }
}

void game_object_free(GameObject *self) {
  int i;
  for (i = 0; i < self->num_components; i++) {
    component_free(self->components[i]);
    self->components[i] = NULL;
  }

  if (self->on_free != NULL) {
    self->on_free(self);
  }
}
