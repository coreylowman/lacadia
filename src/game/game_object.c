#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/glfw3.h>
#include "game_object.h"

GameObject game_object_init(GameWorld *world, GameObjectType type){
    GameObject self;

    self.type = type;

    self.world = world;

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
