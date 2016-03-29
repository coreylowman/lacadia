#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "player.h"
#include "game/wall.h"

extern Vec3 VEC3_UNIT_Y;

Player *player_new(GameWorld *world){
    Player *self = malloc(sizeof(*self));
    self->base_object = game_object_init(world, GAME_OBJECT_TYPE_PLAYER);
    self->affectable.effects = set_new(effect_free);
    return self;
}

void player_free(Player *self){
    set_free(self->affectable.effects);
    free(self);
}

void player_update(Player *self, double dt){
    self->passive(self, dt);

    //note: don't update moveable here, because movement is controlled through
    //  player input. we just assume moveable is up to date
    affectable_object_update(&self->affectable, dt);
    renderable_object_update(&self->renderable, self->moveable);
    collidable_object_update(&self->collidable, self->moveable);
    
    int i;
    for(i = 0;i < 4;i++){
        ability_update(&self->abilities[i], dt);
    }
}

void player_use_ability(Player *self, int i){
    if(ability_is_ready(self->abilities[i])){
        ability_use(&self->abilities[i], self->base_object.world, self);
    }
}

void player_affect(Player *self, Effect *e, double dt){
    affectable_object_affect(&self->affectable, e);
}

void player_move_forwards(Player *self, double dt, float direction){
    float speed = self->moveable.speed;
	moveable_object_move_by(&self->moveable, vec3_scale(self->moveable.direction, dt * speed * direction));
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
    float speed = self->moveable.speed;
    Vec3 sideways;

    sideways = vec3_cross(self->moveable.direction, VEC3_UNIT_Y);
    vec3_normalize(&sideways);

	moveable_object_move_by(&self->moveable, vec3_scale(sideways, dt * speed * direction));
}

void player_handle_inputs(Player *self, double dt, Inputs inputs){
    if (inputs.space_down) { //move vertically
        // double dir = inputs.space_shift_down ? -1.0 : 1.0;
        // camera_move_vertically(camera, dt, dir);
    }

    int i;
    for(i = 1;i < 5;i++)
        if(inputs.numbers_pressed[i]) player_use_ability(self, i - 1);

    if (inputs.left_mouse_down) {
        double dx = inputs.mouse_vel[0] / 100;
        player_turn(self, dx);
    }

    int num_moves = 0;
    //need to capture num of directions of movement, so we don't move faster
    //than the current speed
    if (inputs.w_down) num_moves++;
    if (inputs.s_down)
        if(inputs.w_down) num_moves--;
        else num_moves++;
    if (inputs.a_down) num_moves++;
    if (inputs.d_down)
        if(inputs.a_down) num_moves--;
        else num_moves++;

    float t_dt = dt / (float)num_moves;
    if (inputs.w_down) player_move_forwards(self, t_dt, 1.0);
    if (inputs.s_down) player_move_forwards(self, t_dt, -1.0);
	if (inputs.d_down) player_strafe(self, t_dt, 1.0);
	if (inputs.a_down) player_strafe(self, t_dt, -1.0);    
}

void player_on_collide(GameObject *self, GameObject *other){
	Player *player = self;
    if(other->type == GAME_OBJECT_TYPE_WALL){
        Wall *wall = other;

        Vec3 wall_normal = wall_get_normal(wall, player->collidable.bounding_box.center);
        int i;
        for(i = 0;i < 3;i++){
            player->moveable.position.data[i] += self->world->dt * player->moveable.speed * wall_normal.data[i];
        }
    }
}

