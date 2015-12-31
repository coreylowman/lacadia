#include <string.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glfw3.h>
#include "game_object.h"

GameObject *game_object_new(GameWorld *world, GameObjectType type){
    GameObject *self = malloc(sizeof(*self));
    self->type = type;

    self->world = world;

	self->_destroy = 0;

    return self;
}

void game_object_free(GameObject *self){
    self->world = NULL;
    free(self);
}
