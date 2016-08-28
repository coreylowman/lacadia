#include <stdio.h>
#include <stdlib.h>
#include "enemy.h"

Enemy *enemy_new(GameWorld *world){
    Enemy *self = malloc(sizeof(*self));
    self->base_object = game_object_init(world, GAME_OBJECT_TYPE_ENEMY);
    return self;
}

void enemy_free(Enemy *self){
    self->target = NULL;
    component_free(&self->affectable);
    free(self);
}

void enemy_update(Enemy *self, double dt){
    self->on_update(self, dt);

    component_update(&self->affectable, dt);
    component_update(&self->renderable, dt);
    component_update(&self->collidable, dt);

    if(self->affectable.health <= 0.0){
        self->base_object.destroy = 1;
    }
}
