#include <string.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glfw3.h>
#include "game_object.h"

GameObject *game_object_new(GameWorld *world, GameObjectType type){
    GameObject *self = malloc(sizeof(*self));
    self->type = type;

    self->position.x = 0;
    self->position.y = 0;
    self->position.z = 0;
    self->direction.x = 0;
    self->direction.y = 0;
    self->direction.z = 0;

    self->world = world;

	self->destroy = 0;

    return self;
}

void game_object_free(GameObject *self){
    self->world = NULL;
    free(self);
}

void game_object_update(GameObject *self, double dt){
    self->position.x += self->direction.x * dt;
	self->position.y += self->direction.y * dt;
	self->position.z += self->direction.z * dt;
}

void game_object_render(GameObject *self){
    game_world_draw_asset(self->world, self->asset_id, self->model_matrix);
}
