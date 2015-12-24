#include <stdlib.h>
#include <stdio.h>
#include "spread_burn.h"
#include "util/array_list.h"

Ability spread_burn_ability = 
{ 
    .max_cooldown = 1.5,
    .cooldown = 0,
    .on_use = spread_burn_use
};

void spread_burn_use(GameWorld *world, GameObject *user){
    array_list_push(world->spells, spread_burn_new(world));
}

Spell *spread_burn_new(GameWorld *world){
    Spell *self = spell_new(world);
    self->on_collide = spread_burn_collide;
    return self;
}

void spread_burn_collide(GameObject *object, GameObject *user){
	if (object->type == GAME_OBJECT_TYPE_ENEMY){
		Enemy *enemy = (Enemy *)object;
		enemy->stats.health -= 1;
	}
}
