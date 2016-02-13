#ifndef WALL_H
#define WALL_H

#include "game_world.h"
#include "game_object.h"
#include "renderable_object.h"
#include "collidable_object.h"
#include "util/vec3.h"

#define MAX_WALL_LENGTH 32

typedef struct Wall {
    GameObject *base_object;

    CollidableObject collidable;

    int num_renderables;
    RenderableObject renderables[MAX_WALL_LENGTH];
} Wall;

Vec3 wall_dimensions(GameWorld *world);

Wall *wall_new(GameWorld *world, Vec3 position, Vec3 grow_direction, int length);
void wall_free(Wall *self);

void wall_render(Wall *self);

void wall_on_collide(GameObject *self, GameObject *other);

#endif
