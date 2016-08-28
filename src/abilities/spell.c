#include <stdio.h>
#include <stdlib.h>
#include "spell.h"
#include "util/obb.h"

Spell *spell_new(GameWorld *world){
    Spell *self = malloc(sizeof(*self));
    self->base_object = game_object_init(world, GAME_OBJECT_TYPE_SPELL);
    return self;
}

void spell_free(Spell *self){
    free(self);
}

void spell_update(Spell *self, double dt){
    self->on_update(self, dt);
	component_update(&self->collidable, dt);
	component_update(&self->renderable, dt);
    game_object_move(&self->base_object, self->speed * dt);
}

int spell_is_colliding(CollidableComponent self, CollidableComponent other){
    Spell *spell = self.base_component.container;
	if (spell->caster_type != other.base_component.container->type){
        return obb_intersects(self.bounding_box, other.bounding_box);
    }
    return 0;
}

int spell_is_colliding_with_target(CollidableComponent self, CollidableComponent other){
    Spell *spell = self.base_component.container;
    if(spell->target == other.base_component.container){
        return obb_intersects(self.bounding_box, other.bounding_box);
    }
    return 0;
}
