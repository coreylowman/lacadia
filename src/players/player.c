#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "player.h"

extern Vec3 VEC3_UNIT_Y;

Player *player_new(GameWorld *world){
    Player *self = malloc(sizeof(*self));
    self->base_object = game_object_new(world, GAME_OBJECT_TYPE_PLAYER);
    self->affectable.effects = set_new(effect_free);
    return self;
}

void player_free(Player *self){
    game_object_free(self->base_object);
    set_free(self->affectable.effects);
    free(self);
}

void player_update(Player *self, double dt){
	//update affectable
    affectable_object_update(&self->affectable, dt);

	//update renderable model matrix
	mat4_ident(&self->renderable.model_matrix);
	mat4_translate(&self->renderable.model_matrix, self->moveable.position);
	float rotation = atan(self->moveable.direction.x / self->moveable.direction.z);
	if (self->moveable.direction.z <= 0.0)
		rotation += 3.14159265359;
	mat4_rotate_y(&self->renderable.model_matrix, rotation);

	//update collidable
	self->collidable.bounding_box.center = self->moveable.position;
	self->collidable.bounding_box.center.y += self->collidable.bounding_box.radius.y;
	obb_rotate_y(&self->collidable.bounding_box, rotation);
}

void player_use_ability(Player *self, int i){
    self->abilities[i].on_use(self->base_object->world, self);
}

void player_affect(Player *self, Effect *e){
    affectable_object_affect(&self->affectable, e);
}

void player_move_forwards(Player *self, double dt, float direction){
    int i;
    float speed = self->moveable.speed;
    for(i = 0;i < 3;i++)
        self->moveable.position.data[i] += dt * direction * speed * self->moveable.direction.data[i];
}

void player_turn(Player *self, double side_amt){
    int i;
    Vec3 sideways;

    sideways = vec3_cross(self->moveable.direction, VEC3_UNIT_Y);
    vec3_normalize(&sideways);

    for(i = 0;i < 3;i++)
        self->moveable.direction.data[i] += side_amt * sideways.data[i];

	vec3_normalize(&self->moveable.direction);
}

void player_strafe(Player *self, double dt, float direction){
    int i;
    float speed = self->moveable.speed;
    Vec3 sideways;

    sideways = vec3_cross(self->moveable.direction, VEC3_UNIT_Y);
    vec3_normalize(&sideways);

	for (i = 0; i < 3; i++)
		self->moveable.position.data[i] += dt * speed * direction * sideways.data[i];
}

void player_handle_inputs(Player *self, double dt, Inputs inputs){
    if (inputs.space_down) { //move vertically
        // double dir = inputs.space_shift_down ? -1.0 : 1.0;
        // camera_move_vertically(camera, dt, dir);
    }

    if (inputs.left_mouse_down) {
        double dx = inputs.mouse_vel[0] / 100.0;
        player_turn(self, dx);
    }

    if (inputs.w_down) player_move_forwards(self, dt, 1.0);
    if (inputs.s_down) player_move_forwards(self, dt, -1.0);
    if (inputs.a_down) player_strafe(self, dt, -1.0);
    if (inputs.d_down) player_strafe(self, dt, 1.0);
}

