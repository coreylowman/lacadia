#include <stdlib.h>
#include "engine/util/random.h"
#include "engine/renderable_component.h"
#include "engine/collidable_component.h"
#include "cloud.h"

static void on_update(GameObject *self, double dt);
static void on_free(GameObject *self);
static void on_collide(GameObject *self, GameObject *other);

GameObject *cloud_new(GameWorld *world, Vec3 position) {
  GameObject *self = malloc(sizeof(*self));

  *self = game_object_init(world, "collectable", on_update, NULL, on_free);

  RenderableComponent *renderable = game_object_add_component(
          (Component *)renderable_component_new(self, "cloud", world->renderer));

  renderable_component_set_scale(renderable, random_in_rangef(1.0, 3.0));

  self->position = position;

  return self;
}

static void on_update(GameObject *self, double dt) {
  game_object_move_by(self, vec3_scale(VEC3_UNIT_X, dt * 0.1));
}

static void on_free(GameObject *self) { free(self); }
