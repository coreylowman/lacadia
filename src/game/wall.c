#include <stdlib.h>
#include "wall.h"

Wall *wall_new(GameWorld *world){
    Wall *self = malloc(sizeof(*self));
    self->base = game_object_new(world);
    
    //TODO set up renderable
    //TODO set up collidable
    
    return self;
}

void wall_free(Wall *self){
    game_object_free(self->base);
    free(self);
}

void wall_on_collide(GameObject *self, GameObject *other){
    //do nothing
}
