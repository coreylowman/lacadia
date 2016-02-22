#include "melee_hit.h"
#include "ability.h"

Ability melee_hit_ability = {
	.cooldown = 0,
	.max_cooldown = 0,
	.on_use = melee_hit_use
};

void melee_hit_use(GameWorld *world, GameObject *user){

}
