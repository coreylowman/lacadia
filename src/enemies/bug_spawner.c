#include <stdio.h>
#include "util/random.h"
#include "util/mat4.h"
#include "bug.h"
#include "bug_spawner.h"

static void on_update(GameObject *self, double dt);
static void on_render(GameObject *self, Renderer *renderer);
static void on_free(GameObject *self);

GameObject *bug_spawner_new(GameWorld *world, Vec3 position, double time_to_spawn) {
    BugSpawner *self = malloc(sizeof(*self));

    self->base_object = game_object_init(world, GAME_OBJECT_TYPE_SPAWNER, on_update, on_render, on_free);
    self->base_object.position = position;

    self->time_to_spawn = time_to_spawn;
    self->time_till_next = time_to_spawn;

    return self;
}

static void scale_bug(Enemy *bug, float scale) {
    renderable_component_set_scale(&bug->renderable, scale);
    collidable_component_set_scale(&bug->collidable, scale);
    affectable_component_scale_max(&bug->affectable, scale);
}

static void on_update(GameObject *obj, double dt) {
    BugSpawner *self = obj;

    self->time_till_next -= dt;
    if (self->time_till_next <= 0.0) {
        Enemy *bug = bug_new(obj->world, self->base_object.position);
		scale_bug(bug, random_in_rangef(0.6, 1.4));

        game_world_add_object(obj->world, bug);

        self->time_till_next = self->time_to_spawn;
    }
}

static void on_render(GameObject *obj, Renderer *renderer) {
    BugSpawner *self = obj;
}

static void on_free(GameObject *obj) {
    BugSpawner *self = obj;
    free(self);
}
