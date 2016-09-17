#ifndef SPELL_H
#define SPELL_H

#include "engine/game_world.h"
#include "engine/game_object.h"
#include "engine/collidable_component.h"
#include "engine/renderable_component.h"
#include "engine/util/renderer.h"

typedef struct Spell {
  GameObject base_object;
  float speed;

  CollidableComponent *collidable;
  RenderableComponent *renderable;

  char caster_tag[32];

  GameObject *target;
} Spell;

Spell *spell_new(GameWorld *world, GameObject *user, const char *asset_name,
                 OnCollideCallback on_collide);
void spell_free(GameObject *obj);

void spell_update(GameObject *obj, double dt);
void spell_render(GameObject *obj, Renderer *renderer);

int spell_is_colliding(CollidableComponent self, CollidableComponent other);
int spell_is_colliding_with_target(CollidableComponent self,
                                   CollidableComponent other);

#endif
