#include <stdlib.h>
#include "bug.h"

Enemy *bug_new(GameWorld *world, Vec3 position){
    Enemy *self = enemy_new(world);

    self->target = world->player;

    //TODO set up stats in affectable

    self->moveable.speed = 0.0;
    self->moveable.position = position;
    self->moveable.direction = (Vec3) { .data = { 0, 0, -1 } };

    self->renderable.asset_id = game_world_get_asset_id(world, "assets/bug");
    renderable_object_update(&self->renderable, self->moveable);

    self->collidable.self = self->base_object;
    self->collidable.on_collide = bug_on_collide;
    self->collidable.bounding_box = game_world_get_asset_obb(world, self->renderable.asset_id);
    collidable_object_update(&self->collidable, self->moveable);

    self->on_update = bug_on_update;

	return self;
}


void bug_on_update(Enemy *self, double dt){

}

void bug_on_collide(GameObject *self, GameObject *object){

}

