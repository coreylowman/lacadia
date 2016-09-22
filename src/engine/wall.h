#ifndef WALL_H
#define WALL_H

#include "game_world.h"
#include "game_object.h"
#include "engine/renderable_component.h"
#include "engine/collidable_component.h"
#include "engine/util/vec3.h"
#include "engine/graphics/renderer.h"

#define MAX_WALL_LENGTH 32

typedef struct Wall {
  GameObject base_object;

  Vec3 normal;
} Wall;

Vec3 wall_dimensions(GameWorld *world);

Wall *wall_new(GameWorld *world, Vec3 position, Vec3 grow_direction,
               int length);
void wall_free(GameObject *self);

void wall_render(GameObject *self, Renderer *renderer);

void wall_on_collide(GameObject *self, GameObject *other);

Vec3 wall_get_normal(Wall *self, Vec3 position);

#endif
