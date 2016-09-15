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

Vec3 light_position;

static void free_obj(void *ptr) { game_object_free((GameObject *)ptr); }

GameWorld *game_world_new() {
  GameWorld *self = malloc(sizeof(*self));
  self->game_objects = set_new(free_obj);
  self->collidables = set_new(NULL); // these collidables are pointers to other
                                     // objects collidables... this set doesn't
                                     // have ownership
  self->indices = set_new(free);

  self->renderer = renderer_new();

  self->inputs = inputs_init();
  camera_init(&self->camera, width, height);
  self->level = level_new(self);

  return self;
}

void game_world_free(GameWorld *self) {
  set_free(self->game_objects);
  set_free(self->collidables);
  set_free(self->indices);

  renderer_free(self->renderer);

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

static void destroy_collidable(GameWorld *self, int i) {
  int index = *(int *)self->indices->data[i];
  CollidableComponent *c = self->collidables->data[i];
  set_remove_at(self->game_objects, index);
  set_remove_at(self->collidables, i);
  set_remove_at(self->indices, i);
}

void game_world_update(GameWorld *self, double dt) {
  int i, j;
  CollidableComponent *c1, *c2;

  // if (self->inputs.l_pressed){
  //    if (!camera_is_following(self->camera))
  //        follow();
  //    else
  //        unfollow();
  //}

  // if (self->inputs.m_pressed) {
  //   game_world_add_object(self, (GameObject *)bug_new(self, VEC3_ZERO));
  // }

  if (self->inputs.r_pressed) {
    terrain_regen(&self->level->terrain);
  }

  if (!camera_is_following(self->camera))
    camera_handle_inputs(&self->camera, dt, self->inputs);
  else {
    camera_follow(&self->camera, dt, self->inputs);
  }

  self->dt = dt;

  GameObject *obj;
  for (i = 0; i < self->game_objects->length; i++) {
    if (self->game_objects->data[i] == NULL)
      continue;
    obj = self->game_objects->data[i];
    game_object_update(obj, dt);
  }

  // TODO improve this whole thing with the collidables, indices set and how
  // things are removed
  // this handles collisions as well as removing things from their sets and
  // collidables
  for (i = 0; i < self->collidables->length - 1; i++) {
    if (self->collidables->data[i] == NULL)
      continue;
    c1 = self->collidables->data[i];

    if (c1->base_component.container->destroy) {
      destroy_collidable(self, i);
      continue;
    }

    for (j = i + 1; j < self->collidables->length; j++) {
      if (self->collidables->data[j] == NULL)
        continue;
      c2 = self->collidables->data[j];

      if (c2->base_component.container->destroy) {
        destroy_collidable(self, j);
        continue;
      }

      if (c1->is_colliding(*c1, *c2) && c2->is_colliding(*c2, *c1)) {
        c1->on_collide(c1->base_component.container,
                       c2->base_component.container);
        c2->on_collide(c2->base_component.container,
                       c1->base_component.container);

        if (c1->base_component.container->destroy) {
          destroy_collidable(self, i);
          break;
        }
        if (c2->base_component.container->destroy) {
          destroy_collidable(self, j);
          continue;
        }
      }
    }
  }

  inputs_reset_frame(&self->inputs);
}

void game_world_add_object(GameWorld *self, GameObject *object) {
  set_add(self->game_objects, object);
}

void game_world_add_collidable(GameWorld *self, CollidableComponent *collidable) {
  GameObject *object = collidable->base_component.container;
  int *index = malloc(sizeof(*index));
  *index = set_add(self->game_objects, object);
  set_add(self->collidables, collidable);
  set_add(self->indices, index);
}

void game_world_render(GameWorld *self) {
  mat4_mul(&self->world_to_screen, self->camera.projection_matrix,
           self->camera.view_matrix);
  mat4_inverse(&self->screen_to_world, self->world_to_screen);

  renderer_render_sphere(self->renderer, light_position);

  // gather updates to the various things
  int i;
  GameObject *obj;
  for (i = 0; i < self->game_objects->length; i++) {
    if (self->game_objects->data[i] == NULL)
      continue;
    obj = self->game_objects->data[i];
    game_object_render(obj, self->renderer);
  }
  level_render(self->level, self->renderer);

  // actually draw stuff
  renderer_render(self->renderer, self->camera.projection_matrix,
                  self->camera.view_matrix);
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
  CollidableComponent *collidable;
  GameObject *object;
  int i;
  for (i = 0; i < self->collidables->length; i++) {
    if (self->collidables->data[i] == NULL)
      continue;
    collidable = self->collidables->data[i];
    object = collidable->base_component.container;
    if (strcmp(object->tag, tag) == 0 &&
        vec3_within_dist(collidable->bounding_box.center, user->position,
                         radius)) {
      fn(self, user, object);
    }
  }
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

  Vec3 position = self->camera.location;
  double t = -position.y / direction.y;

  return vec3_add(position, vec3_scale(direction, t));
}
