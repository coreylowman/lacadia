#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "player.h"
#include "game/wall.h"

extern int width, height;

Player *player_new(GameWorld *world, GameObjectUpdateCallback on_update, GameObjectRenderCallback on_render){
    Player *self = malloc(sizeof(*self));
    self->base_object = game_object_init(world, GAME_OBJECT_TYPE_PLAYER, on_update, on_render);
    return self;
}

void player_free(Player *self){
    component_free(&self->affectable);
    free(self);
}

void player_update(Player *self, double dt){
    self->passive(self, dt);

	component_update(&self->affectable, dt);
	component_update(&self->renderable, dt);
	component_update(&self->collidable, dt);

    int i;
    for (i = 0; i < 4; i++){
        ability_update(&self->abilities[i], dt);
    }
}

void player_render(Player *self, Renderer *renderer) {
    component_render(&self->affectable, renderer);
    component_render(&self->renderable, renderer);
    component_render(&self->collidable, renderer);
}

void player_use_ability(Player *self, int i){
    if (ability_is_ready(self->abilities[i])){
        ability_use(&self->abilities[i], self->base_object.world, self);
    }
}

void player_affect(Player *self, Effect *e, double dt){
    affectable_component_affect(&self->affectable, e);
}

void player_move_forwards(Player *self, double dt, float direction){
    float speed = self->affectable.speed;
    game_object_move_by(&self->base_object, vec3_scale(VEC3_UNIT_Z, -dt * speed * direction));
}

void player_turn(Player *self, double side_amt){
    int i;
    Vec3 sideways;

	sideways = vec3_cross(self->base_object.direction, VEC3_UNIT_Y);
    vec3_normalize(&sideways);

    for (i = 0; i < 3; i++)
		self->base_object.direction.data[i] += side_amt * sideways.data[i];

	vec3_normalize(&self->base_object.direction);
}

void player_strafe(Player *self, double dt, float direction){
    float speed = self->affectable.speed;
    //Vec3 sideways;

    //sideways = vec3_cross(self->moveable.direction, VEC3_UNIT_Y);
    //vec3_normalize(&sideways);

	game_object_move_by(&self->base_object, vec3_scale(VEC3_UNIT_X, dt * speed * direction));
}

void player_handle_inputs(Player *self, double dt, Inputs inputs){
    if (inputs.tab_pressed) { //move vertically
        printf("switched stance!");
        self->on_switch_stance(self);
    }

	if (inputs.left_mouse_down) player_use_ability(self, 0);
	if (inputs.right_mouse_down) player_use_ability(self, 1);
    
    double width_2 = (double)(width)* 0.5;
    double height_2 = (double)(height)* 0.5;
    double mousex = (inputs.mouse_pos[0] - width_2) / width_2;
    double mousey = -(inputs.mouse_pos[1] - height_2) / height_2;
    Vec3 mouse_screen = (Vec3) { .data = { mousex, mousey, 1 } };
    Vec3 mouse_world = game_world_screen_coords_to_world_coords(self->base_object.world, mouse_screen);
	self->base_object.direction = vec3_sub(mouse_world, self->base_object.position);
	vec3_normalize(&self->base_object.direction);

    if (inputs.w_down) player_move_forwards(self, dt, 1.0);
    if (inputs.s_down) player_move_forwards(self, dt, -1.0);
    if (inputs.d_down) player_strafe(self, dt, 1.0);
    if (inputs.a_down) player_strafe(self, dt, -1.0);
}

void player_on_collide(GameObject *self, GameObject *other){
    Player *player = self;
    if (other->type == GAME_OBJECT_TYPE_WALL){
        Wall *wall = other;

        Vec3 wall_normal = wall_get_normal(wall, player->collidable.bounding_box.center);
        int i;
        for (i = 0; i < 3; i++){
			player->base_object.position.data[i] += self->world->dt * player->affectable.speed * wall_normal.data[i];
        }
    }
}

