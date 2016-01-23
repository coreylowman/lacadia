#include <stdlib.h>
#include "wall.h"

Wall *wall_new(GameWorld *world, Vec3 position, Vec3 scale){
    Wall *self = malloc(sizeof(*self));
    self->base_object = game_object_new(world, GAME_OBJECT_TYPE_WALL);
    self->base_object->container = self;
    
    //TODO set up renderable
    //TODO set up collidable
    
    return self;
}

void wall_free(Wall *self){
    game_object_free(self->base_object);
    free(self);
}

void wall_on_collide(GameObject *self, GameObject *other){
    //do nothing
}
