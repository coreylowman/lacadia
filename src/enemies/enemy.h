#ifndef ENEMY_H
#define ENEMY_H

#include "game/game_world.h"
#include "game/game_object.h"
#include "components/renderable_component.h"
#include "components/affectable_component.h"
#include "components/collidable_component.h"
#include "abilities/ability.h"

typedef struct Enemy {
    GameObject base_object;

    //who it is following! (most likely the player :P)
    GameObject *target;

    AffectableComponent affectable;
    CollidableComponent collidable;
    RenderableComponent renderable;

    Ability attack;

    void (*on_update)(struct Enemy *self, double dt);
} Enemy;

Enemy *enemy_new(GameWorld *world);
void enemy_free(Enemy *self);

void enemy_update(Enemy *self, double dt);

#endif
