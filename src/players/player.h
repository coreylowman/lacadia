#ifndef PLAYER_H
#define PLAYER_H

#include "util/game_world.h"
#include "util/game_object.h"
#include "util/stats.h"
#include "util/array_list.h"
#include "abilities/ability.h"
#include "effects/effect.h"

typedef struct Player {
    GameObject *base_object;
    
    //abilities
    Ability abilities[4];

    //stats stuff and things that affect them
    Stats stats;
    ArrayList *effects; //ArrayList<Effect *>

    //generic object methods
    int stance;
    void (*on_switch_stance)(struct Player *self);
} Player;

Player *player_new(GameWorld *world);
void player_free(Player *self);

void player_update(Player *self, double dt);

void player_use_ability(Player *self, int ability_index);

void player_apply_effect(Player *self, Effect *e);

#endif
