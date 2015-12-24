#ifndef ENEMY_H
#define ENEMY_H

#include "util/game_world.h"
#include "util/game_object.h"
#include "abilities/ability.h"
#include "util/stats.h"
#include "effects/effect.h"
#include "util/array_list.h"
#include "abilities/spell.h"

typedef struct Enemy {
    GameObject *base_object;

    //who it is following! (most likely the player :P)
    GameObject *target;

    //stats stuff and things that affect them
    Stats stats;
    ArrayList *effects; //ArrayList<Effect *>

    //generic object methods
    void (*on_collide)(struct Enemy *self, GameObject *object);
    void (*update)(struct Enemy *self, double dt);
} Enemy;

Enemy *enemy_new(GameWorld *world);
void enemy_free(Enemy *self);

#endif
