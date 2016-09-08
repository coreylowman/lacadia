#include <stdio.h>
#include <stdlib.h>
#include "collectable.h"
#include "util/obb.h"

Collectable *collectable_new(GameWorld *world, GameObjectUpdateCallback on_update, GameObjectRenderCallback on_render, GameObjectFreeCallback on_free){
    Collectable *self = malloc(sizeof(*self));
    self->base_object = game_object_init(world, GAME_OBJECT_TYPE_COLLECTABLE, on_update, on_render, on_free);
    return self;
}

void collectable_free(Collectable *self){
    component_free(&self->collidable);
    component_free(&self->renderable);
    free(self);
}

void collectable_update(Collectable *self, double dt){
    component_update(&self->collidable, dt);
    component_update(&self->renderable, dt);
}

void collectable_render(Collectable *self, Renderer *renderer) {
    component_render(&self->renderable, renderer);
    component_render(&self->collidable, renderer);
}