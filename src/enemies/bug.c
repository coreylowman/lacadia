#include <stdlib.h>
#include <math.h>
#include "bug.h"
#include "players/player.h"
#include "game/collidable_object.h"

extern Ability melee_hit_ability;

Enemy *bug_new(GameWorld *world, Vec3 position){
    Enemy *self = enemy_new(world);

    self->target = world->player->base_object;

    self->affectable.stats.max_health = 25;
    affectable_object_init(&self->affectable);

    self->moveable.speed = 7.5;
    self->moveable.position = position;
    self->moveable.direction = (Vec3) { .data = { 0, 0, -1 } };

    self->renderable.model_id = game_world_get_model_id(world, "assets/bug");
    renderable_object_update(&self->renderable, self->moveable);

    self->collidable.container = self->base_object;
    self->collidable.is_colliding = collidable_object_is_colliding;
    self->collidable.on_collide = bug_on_collide;
    self->collidable.bounding_box = game_world_get_model_obb(world, self->renderable.model_id);
    collidable_object_update(&self->collidable, self->moveable);

    self->on_update = bug_on_update;
    self->attack = melee_hit_ability;
    self->attack.max_cooldown = 1.0;

	return self;
}


void bug_on_update(Enemy *self, double dt){
    ability_update(&self->attack, dt);

    Player *player = self->target->container;
    Vec3 pos = self->moveable.position;
    Vec3 target_pos = player->moveable.position;
    self->moveable.direction = vec3_sub(target_pos, pos);    
    float dist = sqrt(vec3_dot(self->moveable.direction, self->moveable.direction));
    vec3_normalize(&self->moveable.direction);
    if(ability_is_ready(self->attack)){
        if(dist < 3){
            ability_use(&self->attack, self->base_object->world, self->base_object);
        }else{
            moveable_object_update(&self->moveable, dt);        
        }
    }
    vec3_normalize(&self->moveable.direction);
}

void bug_on_collide(GameObject *self, GameObject *object){
	Enemy *bug = self->container;
    if(object->type == GAME_OBJECT_TYPE_PLAYER){
        Player *player = bug->target->container;
        
    }
}

