#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bug.h"
#include "players/player.h"
#include "components/collidable_component.h"

extern Ability melee_hit_ability;

Enemy *bug_new(GameWorld *world, Vec3 position){
    Enemy *self = enemy_new(world, bug_on_update, enemy_render, enemy_free);

	self->base_object.position = position;
	self->base_object.direction = (Vec3) { .data = { 0, 0, -1 } };

    self->target = game_world_get_player(world);

	self->affectable = affectable_component_init(&self->base_object, 25, 6.5, 0, 0, 0);
	self->renderable = renderable_component_init(&self->base_object, "assets/bug", world->renderer);
	self->collidable = collidable_component_init(&self->base_object, game_world_get_model_obb(world, self->renderable.model_id), bug_on_collide);

    self->attack = melee_hit_ability;
    self->attack.max_cooldown = 1.0;

    return self;
}

void bug_on_update(Enemy *self, double dt){
    enemy_update(self, dt);

    ability_update(&self->attack, dt);

    Player *player = self->target;
    Vec3 pos = self->base_object.position;
    Vec3 target_pos = player->base_object.position;
    self->base_object.direction = vec3_sub(target_pos, pos);
	float dist = sqrt(vec3_dot(self->base_object.direction, self->base_object.direction));
	vec3_normalize(&self->base_object.direction);
    if(ability_is_ready(self->attack)){
        if(dist < 3){
            ability_use(&self->attack, self->base_object.world, self);
        }else{
            game_object_move(&self->base_object, self->affectable.speed * dt);
        }
    }
	vec3_normalize(&self->base_object.direction);
}

void bug_on_collide(GameObject *self, GameObject *object){
	Enemy *bug = self;
    if(object->type == GAME_OBJECT_TYPE_PLAYER){
        Player *player = bug->target;
        
    }
}

