#ifndef SPELL_H
#define SPELL_H

#include "game/game_world.h"
#include "game/game_object.h"
#include "components/collidable_component.h"
#include "components/renderable_component.h"

typedef struct Spell {
    GameObject base_object;
    float speed;

    CollidableComponent collidable;
    RenderableComponent renderable;

    GameObjectType caster_type;
    GameObject *target;

    void (*on_update)(struct Spell *self, double dt);
} Spell;

Spell *spell_new(GameWorld *world);
void spell_free(Spell *self);

void spell_update(Spell *self, double dt);

int spell_is_colliding(CollidableComponent self, CollidableComponent other);
int spell_is_colliding_with_target(CollidableComponent self, CollidableComponent other);

#endif
