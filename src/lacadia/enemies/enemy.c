#include <stdio.h>
#include <stdlib.h>
#include "enemy.h"

Enemy *enemy_new(GameWorld *world, GameObjectUpdateCallback on_update,
                 GameObjectRenderCallback on_render,
                 GameObjectFreeCallback on_free) {
  Enemy *self = malloc(sizeof(*self));
  self->base_object = game_object_init(world, "enemy", on_update,
                                       on_render, on_free);
  return self;
}

void enemy_free(GameObject *obj) {
  Enemy *self = (Enemy *)obj;
  self->target = NULL;
  component_free((Component *)&self->affectable);
  component_free((Component *)&self->renderable);
  component_free((Component *)&self->collidable);
  free(self);
}

void enemy_update(GameObject *obj, double dt) {
  Enemy *self = (Enemy *)obj;
  component_update((Component *)&self->affectable, dt);
  component_update((Component *)&self->renderable, dt);
  component_update((Component *)&self->collidable, dt);

  if (self->affectable.health <= 0.0) {
    self->base_object.destroy = 1;
  }
}

void enemy_render(GameObject *obj, Renderer *renderer) {
  Enemy *self = (Enemy *)obj;
  component_render((Component *)&self->affectable, renderer);
  component_render((Component *)&self->renderable, renderer);
  component_render((Component *)&self->collidable, renderer);
}
