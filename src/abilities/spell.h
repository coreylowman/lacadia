#ifndef SPELL_H
#define SPELL_H

#include "util/game_world.h"
#include "util/game_object.h"
#include "util/vec3.h"
#include "util/mat4.h"
#include "enemies/enemy.h"

typedef struct Spell {
    GameObject *base_object;

    int asset_id;
    Mat4 model_matrix;

    int destroy_on_collide;
    void (*on_collide)(struct Spell *self, GameObject *object, GameObject *user);
    void (*on_update)(struct Spell *self, double dt);
} Spell;

Spell *spell_new(GameWorld *world);
void spell_free(Spell *self);

void spell_update(Spell *self, double dt);
void spell_render(Spell *self);

#endif
