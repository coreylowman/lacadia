#include <stdlib.h>
#include <stdio.h>
#include "icicle.h"
#include "util/array_list.h"

Ability icicle_ability = 
{ 
    .max_cooldown = 1.5,
    .cooldown = 0,
    .on_use = icicle_use,
};

void icicle_use(GameWorld *world, GameObject *user){
    array_list_push(world->spells, icicle_new(world));
}

Spell *icicle_new(GameWorld *world){
    Spell *self = spell_new(world);
    self->on_collide = icicle_collide;
    return self;
}

void icicle_collide(GameObject *object, GameObject *user){
	if (object->type == GAME_OBJECT_TYPE_ENEMY){
		Enemy *enemy = (Enemy *)object;
		enemy->stats.health -= 1;
	}
}
