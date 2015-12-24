#include <stdlib.h>
#include <stdio.h>
#include "frost_nova.h"
#include "util/array_list.h"

Ability frost_nova_ability = 
{ 
    .max_cooldown = 1.5,
    .cooldown = 0,
    .on_use = frost_nova_use
};

void frost_nova_use(GameWorld *world, GameObject *user){
    array_list_push(world->spells, frost_nova_new(world));
}

Spell *frost_nova_new(GameWorld *world){
    Spell *self = spell_new(world);
    self->on_collide = frost_nova_collide;
    return self;
}

void frost_nova_collide(GameObject *object, GameObject *user){
	if (object->type == GAME_OBJECT_TYPE_ENEMY){
		Enemy *enemy = (Enemy *)object;
		enemy->stats.health -= 1;
	}
}
