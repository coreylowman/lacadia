#ifndef ENEMY_H
#define ENEMY_H

#include "engine/graphics/renderer.h"
#include "engine/game_world.h"
#include "engine/game_object.h"
#include "engine/renderable_component.h"
#include "lacadia/effects/affectable_component.h"
#include "engine/collidable_component.h"
#include "lacadia/abilities/ability.h"

typedef struct Enemy {
  GameObject base_object;

  // who it is following! (most likely the player :P)
  GameObject *target;

  // these are contained in base_object, but pointers for ease of reference
  AffectableComponent *affectable;
  CollidableComponent *collidable;
  RenderableComponent *renderable;

  Ability attack;
} Enemy;

Enemy *enemy_new(GameWorld *world, const char *asset_name,
                 GameObjectUpdateCallback on_update,
                 GameObjectRenderCallback on_render,
                 GameObjectFreeCallback on_free);
void enemy_free(GameObject *self);

void enemy_update(GameObject *self, double dt);
void enemy_render(GameObject *self, Renderer *renderer);

#endif
