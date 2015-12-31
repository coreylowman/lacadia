#ifndef ENEMY_H
#define ENEMY_H

#include "game/game_world.h"
#include "game/game_object.h"
#include "game/renderable_object.h"
#include "game/collidable_object.h"
#include "game/affectable_object.h"
#include "game/moveable_object.h"

typedef struct Enemy {
    GameObject *base_object;

    //who it is following! (most likely the player :P)
    GameObject *target;

    AffectableObject affectable;
    CollidableObject collidable;
    MoveableObject moveable;
    RenderableObject renderable;

    void (*on_update)(struct Enemy *self, double dt);
} Enemy;

Enemy *enemy_new(GameWorld *world);
void enemy_free(Enemy *self);

void enemy_update(Enemy *self, double dt);

#endif
