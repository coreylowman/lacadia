#ifndef PLAYER_H
#define PLAYER_H

#include "game/game_world.h"
#include "game/game_object.h"
#include "components/renderable_component.h"
#include "components/collidable_component.h"
#include "components/affectable_component.h"
#include "abilities/ability.h"
#include "abilities/effect.h"
#include "util/array_list.h"
#include "util/inputs.h"

typedef struct Player {
    GameObject base_object;
    
    //abilities
    Ability abilities[4];

    RenderableComponent renderable;
    AffectableComponent affectable;
    CollidableComponent collidable;

    //generic object methods
    int stance;
    void (*on_switch_stance)(struct Player *self);
    void (*on_collide)(GameObject *self, GameObject *other);
    void (*passive)(struct Player *self, double dt);
} Player;

Player *player_new(GameWorld *world);
void player_free(Player *self);

void player_update(Player *self, double dt);

void player_use_ability(Player *self, int ability_index);

void player_affect(Player *self, Effect *e, double dt);

void player_move_forwards(Player *self, double dt, float direction);
void player_turn(Player *self, double side_amt);
void player_strafe(Player *self, double dt, float direction);

void player_handle_inputs(Player *self, double dt, Inputs inputs);
void player_on_collide(GameObject *self, GameObject *other);

#endif
