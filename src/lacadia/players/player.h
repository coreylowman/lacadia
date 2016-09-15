#ifndef PLAYER_H
#define PLAYER_H

#include "engine/game_world.h"
#include "engine/game_object.h"
#include "engine/renderable_component.h"
#include "engine/collidable_component.h"
#include "lacadia/effects/affectable_component.h"
#include "lacadia/abilities/ability.h"
#include "lacadia/effects/effect.h"
#include "engine/util/array_list.h"
#include "engine/util/inputs.h"
#include "engine/util/renderer.h"

typedef struct Player {
  GameObject base_object;

  // abilities
  Ability abilities[4];

  RenderableComponent *renderable;
  AffectableComponent *affectable;
  CollidableComponent *collidable;

  // generic object methods
  int stance;
  void (*on_switch_stance)(struct Player *self);
  void (*on_collide)(GameObject *self, GameObject *other);
  void (*passive)(struct Player *self, double dt);
} Player;

Player *player_new(GameWorld *world, GameObjectUpdateCallback on_update,
                   GameObjectRenderCallback on_render,
                   GameObjectFreeCallback on_free);
void player_free(GameObject *self);

void player_update(GameObject *self, double dt);
void player_render(GameObject *self, Renderer *renderer);

void player_use_ability(Player *self, int ability_index);

void player_affect(Player *self, Effect *e, double dt);

void player_move_forwards(Player *self, double dt, float direction);
void player_turn(Player *self, double side_amt);
void player_strafe(Player *self, double dt, float direction);

void player_on_collide(GameObject *self, GameObject *other);

#endif
