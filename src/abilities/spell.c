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
    moveable_object_update(&self->moveable, dt);
    renderable_object_update(&self->renderable, self->moveable);
    collidable_object_update(&self->collidable, self->moveable);
}

int spell_is_colliding(CollidableObject self, CollidableObject other){
    Spell *spell = self.container;
    if(spell->caster_type != other.container->type){
        return obb_intersects(self.bounding_box, other.bounding_box);
    }
    return 0;
}

int spell_is_colliding_with_target(CollidableObject self, CollidableObject other){
    Spell *spell = self.container;
    if(spell->target == other.container){
        return obb_intersects(self.bounding_box, other.bounding_box);
    }
    return 0;
}
