#include <stdlib.h>
#include <stdio.h>
#include "dragons_breath.h"
#include "util/array_list.h"

Ability dragons_breath_ability = 
{ 
    .max_cooldown = 1.5,
    .cooldown = 0,
    .on_use = dragons_breath_use
};

void dragons_breath_use(GameWorld *world, GameObject *user){
    array_list_push(world->spells, dragons_breath_new(world));
}

Spell *dragons_breath_new(GameWorld *world){
    Spell *self = spell_new(world);
    self->on_collide = dragons_breath_collide;
    return self;
}

void dragons_breath_collide(GameObject *object, GameObject *user){
	if (object->type == GAME_OBJECT_TYPE_ENEMY){
		Enemy *enemy = (Enemy *)object;
		enemy->stats.health -= 1;
	}
}
