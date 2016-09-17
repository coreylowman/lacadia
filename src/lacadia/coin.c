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

  game_object_alloc_components(self, 2);
  self->components[0] = (Component *)renderable_component_new(
      self, "./assets/coin", world->renderer);
  self->components[1] = (Component *)collidable_component_new(
      self, game_world_get_model_obb(
                world, ((RenderableComponent *)self->components[0])->model_id),
      on_collide);

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
