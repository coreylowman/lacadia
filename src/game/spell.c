#include <stdio.h>
#include <stdlib.h>
#include "spell.h"

Spell *spell_new(GameWorld *world){
    Spell *self = malloc(sizeof(*self));
    self->base_object = game_object_new(world, GAME_OBJECT_TYPE_SPELL);
    
    
    return self;
}

void spell_free(Spell *self){
    game_object_free(self->base_object);
    free(self);
}

void spell_update(Spell *self, double dt){
    self->on_update(self, dt);
    moveable_object_update(&self->moveable, dt);
    //TODO update renderable.model_matrix
}
