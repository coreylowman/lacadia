#ifndef WALL_H
#define WALL_H

#include "game_world.h"
#include "game_object.h"
#include "renderable_object.h"
#include "collidable_object.h"
#include "util/vec3.h"

typedef struct {
    GameObject *base_object;
    CollidableObject collidable;
    RenderableObject renderable;
} Wall;

Wall *wall_new(GameWorld *world, Vec3 position, Vec3 scale);
void wall_free(Wall *self);

void wall_on_collide(GameObject *self, GameObject *other);

#endif
