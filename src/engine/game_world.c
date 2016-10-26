#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <float.h>
#include <GL/glew.h>

#include "engine/util/array_list.h"
#include "engine/util/obb.h"
#include "game_object.h"
#include "game_world.h"
#include "engine/collidable_component.h"
#include "engine/particle_system.h"
#include "wall.h"
#include "engine/util/camera.h"
#include "engine/util/inputs.h"
#include "colors.h"

#define VEC3(_x, _y, _z)                                                       \
  { .x = _x, .y = _y, .z = _z }

extern int width, height;

double DELTA_TIME;

Vec3 light_position;

static void free_obj(void *ptr) { game_object_free((GameObject *)ptr); }

static Vec3 snowy = VEC3(245.0f / 255.0f, 245.0f / 255.0f, 245.0f / 255.0f);
static Vec3 grassy = VEC3(0, 179.0f / 255.0f, 0);
static Vec3 rocky = VEC3(156.0f / 255.0f, 143.0f / 255.0f, 124.0f / 255.0f);
static Vec3 sandy = VEC3(242.0f / 255.0f, 245.0f / 255.0f, 198.0f / 255.0f);

static void terrain_callback(TerrainVertex *vert) {
  if (vert->position[1] >= 40) {
    // snowy
    vert->color[0] = snowy.x;
    vert->color[1] = snowy.y;
    vert->color[2] = snowy.z;
  } else if (vert->position[1] >= 25) {
    vert->color[0] = rocky.x;
    vert->color[1] = rocky.y;
    vert->color[2] = rocky.z;
  } else if (vert->position[1] >= 5) {
    Vec3 color = vec3_mix(rocky, grassy, (vert->position[1] - 5) / 20);
    vert->color[0] = color.x;
    vert->color[1] = color.y;
    vert->color[2] = color.z;
  } else {
    // sandy
    vert->color[0] = sandy.x;
    vert->color[1] = sandy.y;
    vert->color[2] = sandy.z;
  }
}

static void color_terrain_red(TerrainVertex *vert) {
  vert->color[0] = 1.0;
  vert->color[1] = 0.0;
  vert->color[2] = 0.0;
}

GameWorld *game_world_new() {
  GameWorld *self = malloc(sizeof(*self));
  self->game_objects = sparse_array_new(free_obj);

  self->asset_manager = asset_manager_new("./assets/");
  self->renderer = renderer_new(self->asset_manager);

  self->terrain = terrain_new(terrain_callback, 100, 50, 100, 5);

  self->inputs = inputs_init();
  camera_init(&self->camera, width, height);

  return self;
}

void game_world_free(GameWorld *self) {
  sparse_array_free(self->game_objects);

  renderer_free(self->renderer);
  asset_manager_free(self->asset_manager);

  terrain_free(self->terrain);

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
    terrain_regen(&self->terrain, terrain_callback);
  }

  if (self->inputs.l_pressed) {
    camera_set_follow(&self->camera, NULL, VEC3_ZERO);
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

  {
	  double width_d = (double)width;
	  double height_d = (double)height;
	  double mousex = 2 * self->inputs.mouse_pos[0] / width_d - 1;
	  double mousey = 2 * (height_d - self->inputs.mouse_pos[1]) / height_d - 1;

	  Vec3 mouse_world = game_world_screen_to_world(self, mousex, mousey, 0.0001);

    if (self->inputs.right_mouse_down) {
      terrain_apply_at(&self->terrain, color_terrain_red, mouse_world.x, mouse_world.z);
    }
  }

  // gather updates to the various things
  SPARSE_ARRAY_FOREACH(GameObject *, obj, self->game_objects,
  { game_object_render(obj, self->renderer); })
  renderer_render_terrain(self->renderer, self->terrain);

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

Vec3 game_world_world_to_screen(GameWorld *self, Vec3 world_coords) {
  Vec3 screen;
  float w = mat4_mul_vec3(&screen, self->world_to_screen, world_coords);
  screen.x /= w;
  screen.y /= w;
  screen.z /= w;
  return screen;
}

#define SIGN(a) ((a) < 0 ? -1 : ((a) > 0 ? 1 : 0))

Vec3 game_world_screen_to_world(GameWorld *self, float mousex, float mousey, double eps) {
  Vec3 world;
  float w = mat4_mul_vec3(&world, self->screen_to_world, (Vec3){ .data = { mousex, mousey, 1 } });
  world.x /= w;
  world.y /= w;
  world.z /= w;

  // note if we didn't want to get terrain height we could return here

  Vec3 direction = vec3_sub(world, self->camera.position);
  vec3_normalize(&direction);

  double min_t = 0, max_t = 0;
  double terrain_height;
  Vec3 position = VEC3_ZERO;
  int sign;

  bool found = false;

  position = vec3_add(self->camera.position, self->camera.forward);
  terrain_height = terrain_get_height(&self->terrain, position.x, position.z);
  sign = SIGN(position.y - terrain_height);

  double t;
  for (t = 2; t < 1000; t += 1) {
	  position = vec3_add(self->camera.position, vec3_scale(direction, t));
	  terrain_height = terrain_get_height(&self->terrain, position.x, position.z);

	  if (terrain_height == FLT_MAX) {
		  // we are off the terrain... we won't find it
		  break;
	  }

	  if (sign != SIGN(position.y - terrain_height)) {
		  // if the sign has changed that means we went through the terrain... we found it
		  max_t = t;
		  found = true;
		  break;
	  } else {
		  min_t = t;
		  sign = SIGN(position.y - terrain_height);
	  }
  }

  if (!found) {
	  return VEC3_ZERO;
  }

  // narrow down the exact t value
  while (max_t - min_t > eps) {
	  t = (max_t + min_t) * 0.5;
	  position = vec3_add(self->camera.position, vec3_scale(direction, t));
	  terrain_height = terrain_get_height(&self->terrain, position.x, position.z);

	  if (sign != SIGN(position.y - terrain_height)) {
		  // if sign doesn't equal then we are still through the terrain... make max_t less to try to get it before the terrain
		  max_t = t;
	  } else {
		  min_t = t;
	  }
  }

  t = (max_t + min_t) * 0.5;
  return vec3_add(self->camera.position, vec3_scale(direction, t));
}
