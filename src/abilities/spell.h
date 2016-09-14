#ifndef SPELL_H
#define SPELL_H

#include "game/game_world.h"
#include "game/game_object.h"
#include "components/collidable_component.h"
#include "components/renderable_component.h"
#include "util/renderer.h"

typedef struct Spell {
  GameObject base_object;
  float speed;

  CollidableComponent collidable;
  RenderableComponent renderable;

  GameObjectType caster_type;
  GameObject *target;
} Spell;

Spell *spell_new(GameWorld *world, GameObjectUpdateCallback on_update,
                 GameObjectRenderCallback on_render,
                 GameObjectFreeCallback on_free);
void spell_free(GameObject *obj);

void spell_update(GameObject *obj, double dt);
void spell_render(GameObject *obj, Renderer *renderer);

int spell_is_colliding(CollidableComponent self, CollidableComponent other);
int spell_is_colliding_with_target(CollidableComponent self,
                                   CollidableComponent other);

#endif
