#include <stdio.h>
#include <stdlib.h>
#include "collectable.h"
#include "engine/util/obb.h"

Collectable *collectable_new(GameWorld *world,
                             GameObjectUpdateCallback on_update,
                             GameObjectRenderCallback on_render,
                             GameObjectFreeCallback on_free) {
  Collectable *self = malloc(sizeof(*self));
  self->base_object = game_object_init(world, GAME_OBJECT_TYPE_COLLECTABLE,
                                       on_update, on_render, on_free);
  return self;
}

void collectable_free(GameObject *obj) {
  Collectable *self = (Collectable *)self;
  component_free((Component *)&self->collidable);
  component_free((Component *)&self->renderable);
  free(self);
}

void collectable_update(GameObject *obj, double dt) {
  Collectable *self = (Collectable *)self;
  component_update((Component *)&self->collidable, dt);
  component_update((Component *)&self->renderable, dt);
}

void collectable_render(GameObject *obj, Renderer *renderer) {
  Collectable *self = (Collectable *)self;
  component_render((Component *)&self->renderable, renderer);
  component_render((Component *)&self->collidable, renderer);
}
