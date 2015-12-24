#ifndef SPELL_H
#define SPELL_H

#include "util/game_world.h"
#include "util/game_object.h"

typedef struct Spell {
    GameObject *base_object;

    int destroy_on_collide;
    void (*on_collide)(struct Spell *self, GameObject *object, GameObject *user);
    void (*on_update)(struct Spell *self, double dt);
} Spell;

Spell *spell_new(GameWorld *world);
void spell_free(Spell *self);

void spell_update(Spell *self, double dt);

#endif
