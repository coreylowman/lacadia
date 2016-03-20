#ifndef WALL_H
#define WALL_H

#include "game_world.h"
#include "game_object.h"
#include "renderable_object.h"
#include "collidable_object.h"
#include "util/vec3.h"
#include "util/renderer.h"

#define MAX_WALL_LENGTH 32

typedef struct Wall {
    GameObject base_object;

    CollidableObject collidable;

    int num_renderables;
    RenderableObject renderables[MAX_WALL_LENGTH];

    Vec3 normal;
} Wall;

Vec3 wall_dimensions(GameWorld *world);

Wall *wall_new(GameWorld *world, Vec3 position, Vec3 grow_direction, int length);
void wall_free(Wall *self);

void wall_render(Wall *self, Renderer *renderer);

void wall_on_collide(GameObject *self, GameObject *other);

Vec3 wall_get_normal(Wall *self, Vec3 position);

#endif
