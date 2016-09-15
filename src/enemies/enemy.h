#ifndef ENEMY_H
#define ENEMY_H

#include "engine/util/renderer.h"
#include "engine/game_world.h"
#include "engine/game_object.h"
#include "engine/renderable_component.h"
#include "engine/affectable_component.h"
#include "engine/collidable_component.h"
#include "abilities/ability.h"

typedef struct Enemy {
  GameObject base_object;

  // who it is following! (most likely the player :P)
  GameObject *target;

  AffectableComponent affectable;
  CollidableComponent collidable;
  RenderableComponent renderable;

  Ability attack;
} Enemy;

Enemy *enemy_new(GameWorld *world, GameObjectUpdateCallback on_update,
                 GameObjectRenderCallback on_render,
                 GameObjectFreeCallback on_free);
void enemy_free(GameObject *self);

void enemy_update(GameObject *self, double dt);
void enemy_render(GameObject *self, Renderer *renderer);

#endif
