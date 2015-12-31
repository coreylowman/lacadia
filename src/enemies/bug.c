#include <stdlib.h>
#include "bug.h"

Enemy *bug_new(GameWorld *world){
    Enemy *self = enemy_new(world);

    self->target = world->player;

    //todo set up stats in affectable

    //todo set bounding box in collidable
    self->collidable.on_collide = bug_on_collide;

    //todo set position and speed in moveable

    //todo model_matrix in renderable
    self->renderable.asset_id = game_world_get_asset_id(world, "assets/bug");

    self->on_update = bug_on_update;

	return self;
}


void bug_on_update(Enemy *self, double dt){

}

void bug_on_collide(Enemy *self, GameObject *object){

}

