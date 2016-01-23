#include <stdio.h>
#include <stdlib.h>
#include "enemy.h"

Enemy *enemy_new(GameWorld *world){
    Enemy *self = malloc(sizeof(*self));
    self->base_object = game_object_new(world, GAME_OBJECT_TYPE_ENEMY);
    self->base_object->container = self;

	self->affectable.effects = set_new(effect_free);    
    return self;
}

void enemy_free(Enemy *self){
    self->target = NULL;
    game_object_free(self->base_object);
	set_free(self->affectable.effects);
    free(self);
}

void enemy_update(Enemy *self, double dt){
    self->on_update(self, dt);
    affectable_object_update(&self->affectable, dt);
    moveable_object_update(&self->moveable, dt);
    renderable_object_update(&self->renderable, self->moveable);
    collidable_object_update(&self->collidable, self->moveable);
}
