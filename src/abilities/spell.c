#include <stdio.h>
#include <stdlib.h>
#include "spell.h"

Spell *spell_new(GameWorld *world){
    Spell *self = malloc(sizeof(*self));
    self->base_object = game_object_new(world, GAME_OBJECT_TYPE_SPELL);

    mat4_ident(&self->model_matrix);
    
    return self;
}

void spell_free(Spell *self){
    game_object_free(self->base_object);
    free(self);
}

void spell_update(Spell *self, double dt){
    game_object_update(self->base_object, dt);
    self->on_update(self, dt);
}

void spell_render(Spell *self){
    game_world_draw_asset(self->base_object->world, self->asset_id, self->model_matrix);
}
