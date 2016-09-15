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
  free(self);
}

void enemy_update(GameObject *obj, double dt) {
  Enemy *self = (Enemy *)obj;

  if (self->affectable->health <= 0.0) {
    self->base_object.destroy = 1;
  }
}

void enemy_render(GameObject *obj, Renderer *renderer) {
  Enemy *self = (Enemy *)obj;
}
