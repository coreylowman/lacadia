#ifndef SPELL_H
#define SPELL_H

#include "game/game_world.h"
#include "game/game_object.h"
#include "game/collidable_object.h"
#include "game/moveable_object.h"
#include "game/renderable_object.h"

typedef struct Spell {
    GameObject *base_object;

    CollidableObject collidable;
    MoveableObject moveable;
    RenderableObject renderable;

    void (*on_update)(struct Spell *self, double dt);
} Spell;

Spell *spell_new(GameWorld *world);
void spell_free(Spell *self);

void spell_update(Spell *self, double dt);

#endif
