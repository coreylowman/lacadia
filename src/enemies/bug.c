#include <stdlib.h>
#include "util/log.h"
#include "bug.h"

Enemy *bug_new(GameWorld *world){
    Enemy *self = enemy_new(world);
    self->target = world->player;
    
    self->base_object->asset_id = game_world_get_asset_id(world, "assets/box");
    self->base_object->bounding_box = game_world_get_aabb(world, self->base_object->asset_id);

	return self;
}

void bug_free(Enemy *self){
    enemy_free(self);
}

void bug_update(Enemy *self, double dt){
    enemy_update(self, dt);

    //todo update direction
    //todo update position
    //todo try to attack
}

void bug_on_collide(Enemy *self, GameObject *object){
    log(DEBUG, "in bug_on_collide");
}

