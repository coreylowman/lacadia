#include <stdio.h>
#include "engine/util/random.h"
#include "engine/util/mat4.h"
#include "bug.h"
#include "bug_spawner.h"

static void on_update(GameObject *self, double dt);
static void on_render(GameObject *self, Renderer *renderer);
static void on_free(GameObject *self);

GameObject *bug_spawner_new(GameWorld *world, Vec3 position,
                            double time_to_spawn) {
  BugSpawner *self = malloc(sizeof(*self));

  self->base_object =
      game_object_init(world, "spawner", on_update, on_render, on_free);
  self->base_object.position = position;

  self->time_to_spawn = time_to_spawn;
  self->time_till_next = time_to_spawn;

  return (GameObject *)self;
}

static void scale_bug(Enemy *bug, float scale) {
  affectable_component_scale_max(bug->affectable, scale);
  renderable_component_set_scale(bug->renderable, scale);
  collidable_component_set_scale(bug->collidable, scale);
}

static void on_update(GameObject *obj, double dt) {
  BugSpawner *self = (BugSpawner *)obj;

  self->time_till_next -= dt;
  if (self->time_till_next <= 0.0) {
    Enemy *bug = bug_new(obj->world, self->base_object.position);
    scale_bug(bug, random_in_rangef(0.6, 1.4));

    game_world_add_object(obj->world, (GameObject *)bug);

    self->time_till_next = self->time_to_spawn;
  }
}

static void on_render(GameObject *obj, Renderer *renderer) {
  BugSpawner *self = (BugSpawner *)obj;
}

static void on_free(GameObject *obj) {
  BugSpawner *self = (BugSpawner *)obj;
  free(self);
}
