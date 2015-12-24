#include <stdlib.h>
#include <stdio.h>
#include "fireball.h"
#include "util/array_list.h"
#include "util/log.h"

Ability fireball_ability = 
{ 
    .max_cooldown = 1.5,
    .cooldown = 0,
    .on_use = fireball_use
};

void fireball_use(GameWorld *world, GameObject *user){
    array_list_push(world->spells, fireball_new(world));
}

Spell *fireball_new(GameWorld *world){
    Spell *self = spell_new(world);

    self->base_object->asset_id = game_world_get_asset_id(world, "assets/fireball");
    self->base_object->bounding_box = game_world_get_asset_aabb(world, self->asset_id);

    self->destroy_on_collide = 1;
    self->on_collide = fireball_collide;
    self->on_update = fireball_update;

    self->base_object->direction.x = 1;

    return self;
}

void fireball_collide(Spell *self, GameObject *object, GameObject *user){
    log(DEBUG, "in fireball_collide");

	if (object->type == GAME_OBJECT_TYPE_ENEMY){
        log(DEBUG, "fireball collided with an enemy");

		Enemy *enemy = (Enemy *)object;
		enemy->stats.health -= 1;
        array_list_push(enemy->effects, hot_new(-1.0, 5000.0));
	}
}

void fireball_update(Spell *self, double dt){
    mat4_ident(&self->model_matrix);
    mat4_translate(&self->model_matrix, &self->base_object->position.data[0]);
}
