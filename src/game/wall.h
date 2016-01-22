#ifndef WALL_H
#define WALL_H

#include "game_world.h"
#include "game_object.h"
#include "collidable_object.h"

typedef struct {
    GameObject *base;
    CollidableObject collidable;
    RenderableObject renderable;
} Wall;

Wall *wall_new(GameWorld *world);
void wall_free(World *self);

void wall_on_collide(GameObject *self, GameObject *other);

#endif
