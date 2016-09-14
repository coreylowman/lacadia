#include <stdlib.h>
#include "coin.h"

static void on_update(GameObject *self, double dt);
static void on_collide(GameObject *self, GameObject *other);

Collectable *coin_new(GameWorld *world, Vec3 position) {
    Collectable *self = collectable_new(world, on_update, collectable_render, collectable_free);

    self->renderable = renderable_component_init(&self->base_object, "assets/coin", world->renderer);
    self->collidable = collidable_component_init(&self->base_object, game_world_get_model_obb(world, self->renderable.model_id), on_collide);

	self->base_object.position = position;

    return self;
}

static void on_update(GameObject *obj, double dt) {
	Collectable *self = (Collectable *)obj;
    collectable_update(obj, dt);

    game_object_rotate_by(&self->base_object, dt);
}

static void on_collide(GameObject *self, GameObject *other) {
    self->destroy = 1;
}
