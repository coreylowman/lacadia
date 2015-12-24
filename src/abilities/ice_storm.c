#include <stdlib.h>
#include <stdio.h>
#include "ice_storm.h"
#include "util/array_list.h"

Ability ice_storm_ability = 
{ 
    .max_cooldown = 1.5,
    .cooldown = 0,
    .on_use = ice_storm_use
};

void ice_storm_use(GameWorld *world, GameObject *user){
    array_list_push(world->spells, ice_storm_new(world));
}

Spell *ice_storm_new(GameWorld *world){
    Spell *self = spell_new(world);
    self->on_collide = ice_storm_collide;
    return self;
}

void ice_storm_collide(GameObject *object, GameObject *user){
	if (object->type == GAME_OBJECT_TYPE_ENEMY){
		Enemy *enemy = (Enemy *)object;
		enemy->stats.health -= 1;
	}
}
