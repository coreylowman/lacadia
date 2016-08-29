#include <stdio.h>
#include <stdlib.h>
#include "enemy.h"

Enemy *enemy_new(GameWorld *world, GameObjectUpdateCallback on_update, GameObjectRenderCallback on_render, GameObjectFreeCallback on_free){
    Enemy *self = malloc(sizeof(*self));
    self->base_object = game_object_init(world, GAME_OBJECT_TYPE_ENEMY, on_update, on_render, on_free);
    return self;
}

void enemy_free(Enemy *self){
    self->target = NULL;
    component_free(&self->affectable);
    component_free(&self->renderable);
    component_free(&self->collidable);
    free(self);
}

void enemy_update(Enemy *self, double dt){
    component_update(&self->affectable, dt);
    component_update(&self->renderable, dt);
    component_update(&self->collidable, dt);

    if(self->affectable.health <= 0.0){
        self->base_object.destroy = 1;
    }
}

void enemy_render(Enemy *self, Renderer *renderer) {
    component_render(&self->affectable, renderer);
    component_render(&self->renderable, renderer);
    component_render(&self->collidable, renderer);
}
