#include <stdlib.h>
#include "engine/renderable_component.h"
#include "engine/collidable_component.h"
#include "coin.h"

static void on_update(GameObject *self, double dt);
static void on_free(GameObject *self);
static void on_collide(GameObject *self, GameObject *other);

GameObject *coin_new(GameWorld *world, Vec3 position) {
  GameObject *self = malloc(sizeof(*self));

  *self = game_object_init(world, "collectable", on_update, NULL, on_free);

  RenderableComponent *renderable = game_object_add_component(
          (Component *)renderable_component_new(self, "coin", world->renderer));
  game_object_add_collidable(collidable_component_new(
                self, game_world_get_model_obb(world, renderable->model_id),
                on_collide));

  self->position = position;

  return self;
}

static void on_update(GameObject *self, double dt) {
  game_object_rotate_by(self, dt);
}

static void on_free(GameObject *self) { free(self); }

static void on_collide(GameObject *self, GameObject *other) {
  self->destroy = 1;
}
