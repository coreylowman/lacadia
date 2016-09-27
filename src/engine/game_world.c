#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>

#include "level.h"
#include "engine/util/array_list.h"
#include "engine/util/obb.h"
#include "engine/util/string_helpers.h"
#include "game_object.h"
#include "game_world.h"
#include "engine/collidable_component.h"
#include "engine/particle_system.h"
#include "wall.h"
#include "engine/util/camera.h"
#include "engine/util/inputs.h"
#include "colors.h"

extern int width, height;

double DELTA_TIME;

Vec3 light_position;

static void free_obj(void *ptr) { game_object_free((GameObject *)ptr); }

GameWorld *game_world_new() {
  GameWorld *self = malloc(sizeof(*self));
  self->game_objects = sparse_array_new(free_obj);

  self->asset_manager = asset_manager_new("./assets/");
  self->renderer = renderer_new(self->asset_manager);

  self->inputs = inputs_init();
  camera_init(&self->camera, width, height);
  self->level = level_new(self);

  return self;
}

void game_world_free(GameWorld *self) {
  sparse_array_free(self->game_objects);

  
  renderer_free(self->renderer);
  asset_manager_free(self->asset_manager);

  level_free(self->level);

  free(self);
}

GameObject *game_world_get_first_tagged(GameWorld *self, const char *tag) {
  int i;
  GameObject *obj;
  for (i = 0; i < self->game_objects->length; i++) {
    obj = self->game_objects->data[i];
    if (obj != NULL && strcmp(obj->tag, tag) == 0) {
      return obj;
    }
  }

  return NULL;
}

void game_world_update(GameWorld *self, double dt) {
  self->dt = dt;
  DELTA_TIME = dt;

  if (self->inputs.r_pressed) {
    terrain_regen(&self->level->terrain, level_terrain_callback);
  }

  camera_handle_inputs(&self->camera, dt, self->inputs);
  camera_follow(&self->camera, dt, self->inputs);

  int i;
  GameObject *obj;
  for (i = 0; i < self->game_objects->length; i++) {
    if (self->game_objects->data[i] == NULL)
      continue;
    obj = self->game_objects->data[i];

    game_object_update(obj, dt);
    if (obj->destroy) {
      sparse_array_remove_at(self->game_objects, i);
    }
  }

  int j;
  CollidableComponent *c1, *c2;
  GameObject *obj1, *obj2;
  for (i = 0; i < self->game_objects->length - 1; i++) {
    obj1 = self->game_objects->data[i];
    if (obj1 == NULL || obj1->collidable_index == -1)
      continue;
    c1 = (CollidableComponent *)obj1->components[obj1->collidable_index];

    for (j = i + 1; j < self->game_objects->length; j++) {
      obj2 = self->game_objects->data[j];
      if (obj2 == NULL || obj2->collidable_index == -1)
        continue;
      c2 = (CollidableComponent *)obj2->components[obj2->collidable_index];

      if (c1->is_colliding(*c1, *c2) && c2->is_colliding(*c2, *c1)) {
        c1->on_collide(obj1, obj2);
        c2->on_collide(obj2, obj1);

        if (obj1->destroy) {
          sparse_array_remove_at(self->game_objects, i);
          break;
        }
        if (obj2->destroy) {
          sparse_array_remove_at(self->game_objects, j);
          continue;
        }
      }
    }
  }

  inputs_reset_frame(&self->inputs);
}

void game_world_add_object(GameWorld *self, GameObject *object) {
  sparse_array_add(self->game_objects, object);
}

void game_world_render(GameWorld *self) {
  mat4_mul(&self->world_to_screen, self->camera.projection_matrix,
           self->camera.view_matrix);
  mat4_inverse(&self->screen_to_world, self->world_to_screen);

  renderer_render_sphere(self->renderer, light_position);

  // gather updates to the various things
  SPARSE_ARRAY_FOREACH(GameObject *, obj, self->game_objects,
                       { game_object_render(obj, self->renderer); })
  level_render(self->level, self->renderer);

  renderer_render_text(self->renderer, "lacadia", 7,
                       (Vec3){.x = 0, .y = 0, .z = 1}, VEC3_ZERO);
  renderer_render_water(self->renderer, (Vec3){.x = 0, .y = 6, .z = 0}, 500, 500);

  // actually draw stuff
  renderer_render(self->renderer, self->camera);
}

int game_world_get_model_id(GameWorld *self, const char *name) {
  return renderer_get_model_id(self->renderer, name);
}

Obb game_world_get_model_obb(GameWorld *self, int model_id) {
  return renderer_get_model_obb(self->renderer, model_id);
}

// todo change to take a GameObjectType parameter instead of just enemies?
void game_world_apply(GameWorld *self, const char *tag, GameObject *user,
                      float radius,
                      void (*fn)(GameWorld *world, GameObject *obj,
                                 GameObject *target)) {
  SPARSE_ARRAY_FOREACH(GameObject *, obj, self->game_objects, {
    if (strcmp(obj->tag, tag) == 0 &&
        vec3_within_dist(obj->position, user->position, radius)) {
      fn(self, user, obj);
    }
  })
}

Vec3 game_world_world_coords_to_screen_coords(GameWorld *self,
                                              Vec3 world_coords) {
  Vec3 output;
  float w = mat4_mul_vec3(&output, self->world_to_screen, world_coords);
  output.x /= w;
  output.y /= w;
  output.z /= w;
  return output;
}

Vec3 game_world_screen_coords_to_world_coords(GameWorld *self,
                                              Vec3 screen_coords) {
  Vec3 direction;
  float w = mat4_mul_vec3(&direction, self->screen_to_world, screen_coords);
  direction.x /= w;
  direction.y /= w;
  direction.z /= w;
  vec3_normalize(&direction);

  double t = -self->camera.position.y / direction.y;
  Vec3 position = vec3_add(self->camera.position, vec3_scale(direction, t));

  return position;
}
