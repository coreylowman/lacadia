#include <stdlib.h>
#include "bug.h"

Enemy *bug_new(GameWorld *world){
    Enemy *self = enemy_new(world);
    self->target = world->player;
	self->base_object->bounding_box = (Rect) { .x = 4.5, .y = -0.5, .z = -0.5, .width = 1, .height = 1, .length = 1};

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
