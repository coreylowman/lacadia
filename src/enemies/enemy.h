#ifndef ENEMY_H
#define ENEMY_H

#include "util/game_world.h"
#include "util/game_object.h"
#include "abilities/ability.h"
#include "util/stats.h"
#include "effects/effect.h"
#include "util/array_list.h"

typedef struct Enemy {
    GameObject *base_object;

    Stats stats;
    
    ArrayList *effects; //ArrayList<Effect *>

    GameObject *target;
} Enemy;

Enemy *enemy_new(GameWorld *world);
void enemy_free(Enemy *self);

void enemy_update(Enemy *self, double dt);
void enemy_render(Enemy *self);

#endif
