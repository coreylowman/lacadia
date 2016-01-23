#ifndef PLAYER_H
#define PLAYER_H

#include "game/game_world.h"
#include "game/game_object.h"
#include "game/renderable_object.h"
#include "game/moveable_object.h"
#include "game/collidable_object.h"
#include "game/affectable_object.h"
#include "game/ability.h"
#include "game/effect.h"
#include "util/array_list.h"
#include "util/inputs.h"

typedef struct Player {
    GameObject *base_object;
    
    //abilities
    Ability abilities[4];

    RenderableObject renderable;
    MoveableObject moveable;
    AffectableObject affectable;
    CollidableObject collidable;

    //generic object methods
    int stance;
    void (*on_switch_stance)(struct Player *self);
    void (*on_collide)(GameObject *self, GameObject *other);
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
