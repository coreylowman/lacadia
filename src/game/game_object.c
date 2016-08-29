#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/glfw3.h>
#include "game_object.h"

GameObject game_object_init(GameWorld *world, GameObjectType type, GameObjectUpdateCallback on_update, GameObjectRenderCallback on_render){
    GameObject self;

    self.type = type;

    self.world = world;

    self.on_update =  on_update;
    self.on_render = on_render;

    self.destroy = 0;

    return self;
}

float game_object_get_y_rotation(GameObject *self) {
	float rotation = atan(self->direction.x / self->direction.z);
	if (self->direction.z <= 0.0)
		rotation += 3.14159265358979323846;
	return rotation;
}

void game_object_move_by(GameObject *self, Vec3 velocity) {
    self->position = vec3_add(self->position, velocity);
}

void game_object_move(GameObject *self, double scalar) {
    self->position = vec3_add(self->position, vec3_scale(self->direction, scalar));
}

void game_object_update(GameObject *self, double dt) {
    if (self->on_update != NULL) {
        self->on_update(self, dt);
    }
}

void game_object_render(GameObject *self, Renderer *renderer) {
    if (self->on_render != NULL) {
        self->on_render(self, renderer);
    }
}
